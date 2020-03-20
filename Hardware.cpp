#include "Hardware.h"

Ticker inputTicker;
bool buttonPushed = false;
float filteredBrightness = 128;
int16_t brightness8 = 255;
CRGB rgb_data[4][4][4];
gamma_corrected_rgb gamma_corrected_rgb_data[4][4][4];
XYZ current_led = XYZ{0, 1, 0};

void betterShow(int16_t bright8)
{
  if (bright8 != -1)
    brightness8 = bright8;

  for (uint16_t x = 0; x < 4; x++)
  {
    for (uint16_t y = 0; y < 4; y++)
    {
      for (uint16_t z = 0; z < 4; z++)
      {
        CRGB darkenedColor = rgb_data[x][y][z];
        darkenedColor.nscale8(brightness8);
        gamma_corrected_rgb_data[x][y][z].red = pgm_read_dword(gamma_correction_table + darkenedColor.red);
        gamma_corrected_rgb_data[x][y][z].green = pgm_read_dword(gamma_correction_table + darkenedColor.green);
        gamma_corrected_rgb_data[x][y][z].blue = pgm_read_dword(gamma_correction_table + darkenedColor.blue);
      }
    }
  }
}

void betterClear()
{
  for (uint16_t x = 0; x < 4; x++)
  {
    for (uint16_t y = 0; y < 4; y++)
    {
      for (uint16_t z = 0; z < 4; z++)
      {
        rgb_data[x][y][z] = CRGB::Black;
      }
    }
  }

  betterShow();
}

void fill_solid(CRGB color)
{
  for (uint16_t x = 0; x < 4; x++)
  {
    for (uint16_t y = 0; y < 4; y++)
    {
      for (uint16_t z = 0; z < 4; z++)
      {
        rgb_data[x][y][z] = color;
      }
    }
  }
}

void fill_gradient(CRGB color1, CRGB color2)
{
  // 4 * 4 * 4 * multiplier ~= 1.0
  float multiplier = 0.0156;
  uint8_t x, y, z;
  uint16_t red, green, blue;

  for (x = 0; x < 4; x++)
  {
    for (y = 0; y < 4; y++)
    {
      for (z = 0; z < 4; z++)
      {
        red = multiplier * (x + 1) * (y + 1) * (z + 1) * color1.red + multiplier * (4 - x) * (4 - y) * (4 - z) * color2.red;
        green = multiplier * (x + 1) * (y + 1) * (z + 1) * color1.green + multiplier * (4 - x) * (4 - y) * (4 - z) * color2.green;
        blue = multiplier * (x + 1) * (y + 1) * (z + 1) * color1.blue + multiplier * (4 - x) * (4 - y) * (4 - z) * color2.blue;

        if (red > 255)
          red = 255;
        if (green > 255)
          green = 0;
        if (blue > 255)
          blue = 0;

        rgb_data[x][y][z] = CRGB(red, green, blue);
      }
    }
  }
}

CRGB betterHue(uint16_t fract1535, int16_t sat, uint8_t val)
{
  // fract1535 is a number from 0 to 1535 (MAX_BETTER_HUE)
  uint8_t fractHue = fract1535 / 6;
  uint8_t fractBlend = fract1535 % 6;
  if (sat == -1)
    sat = 255;

  CRGB leftColor, rightColor;
  hsv2rgb_rainbow(CHSV(fractHue, sat, val), leftColor);
  hsv2rgb_rainbow(CHSV(fractHue + 1, sat, val), rightColor);

  return blend(leftColor, rightColor, 42 * fractBlend);
}

void initHardware()
{
  pinMode(BUTTON_PIN, INPUT);
  inputTicker.attach_ms(10, handleInput);

  SPI.begin();

  pinMode(PLANE_1_PIN, OUTPUT);
  pinMode(PLANE_2_PIN, OUTPUT);
  pinMode(PLANE_3_PIN, OUTPUT);
  pinMode(PLANE_4_PIN, OUTPUT);
  pinMode(BLANK_AND_LATCH_PIN, OUTPUT);

  // Start transmitter timer (timer1 interrupt)
  noInterrupts();
  timer1_isr_init();
  timer1_attachInterrupt(transmit);
  timer1_write(TIMER1_TICKS_PER_US / 16 * TRANSMIT_INTERVAL_US);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
  interrupts();
}

uint16_t getPotiBrightness()
{
  int16_t value = (1023 - analogRead(A0) - 29) * 1.06;

  if (value > 1023)
    value = 1023;
  if (value < 0)
    value = 0;

  return value;
}

void handleInput()
{
  if (!currentFade)
  {
    // Adjust brightness calculation if needed
    uint16_t potiBrightness = getPotiBrightness();

    filteredBrightness = filteredBrightness - 0.01 * (filteredBrightness - potiBrightness);

    // Only set brightness if it's not near the filtered brightness value which will lag behind
    if (!(filteredBrightness - 1 < potiBrightness && potiBrightness < filteredBrightness + 1))
    {
      brightness8 = (float)potiBrightness * potiBrightness / 1023 / 4;
    }
  }

  // Push button
  if (!digitalRead(BUTTON_PIN) && !buttonPushed)
  {
    // button pushed
    stopFade();
    beginNextAnimation();

    buttonPushed = true;
  }
  else if (digitalRead(BUTTON_PIN) && buttonPushed)
  {
    // button released

    buttonPushed = false;
  }
}

void transmit()
{
  // Write SPI data
  uint8_t data_array[72];
  for (uint8_t i = 0; i < 8; i++)
  {
    // byte 0
    data_array[i * 9 + 0] = gamma_corrected_rgb_data[current_led.x][current_led.y][current_led.z].blue >> 4;
    // byte 1
    data_array[i * 9 + 1] = gamma_corrected_rgb_data[current_led.x][current_led.y][current_led.z].blue << 4;
    data_array[i * 9 + 1] |= gamma_corrected_rgb_data[current_led.x][current_led.y][current_led.z].green >> 8;
    // byte 2
    data_array[i * 9 + 2] = gamma_corrected_rgb_data[current_led.x][current_led.y][current_led.z].green;
    // byte 3
    data_array[i * 9 + 3] = gamma_corrected_rgb_data[current_led.x][current_led.y][current_led.z].red >> 4;
    // byte 4
    data_array[i * 9 + 4] = gamma_corrected_rgb_data[current_led.x][current_led.y][current_led.z].red << 4;
    select_next_led();
    data_array[i * 9 + 4] |= gamma_corrected_rgb_data[current_led.x][current_led.y][current_led.z].blue >> 8;
    // byte 5
    data_array[i * 9 + 5] = gamma_corrected_rgb_data[current_led.x][current_led.y][current_led.z].blue;
    // byte 6
    data_array[i * 9 + 6] = gamma_corrected_rgb_data[current_led.x][current_led.y][current_led.z].green >> 4;
    // byte 7
    data_array[i * 9 + 7] = gamma_corrected_rgb_data[current_led.x][current_led.y][current_led.z].green << 4;
    data_array[i * 9 + 7] |= gamma_corrected_rgb_data[current_led.x][current_led.y][current_led.z].red >> 8;
    // byte 8
    data_array[i * 9 + 8] = gamma_corrected_rgb_data[current_led.x][current_led.y][current_led.z].red;
    select_next_led();
  }
  SPI.writeBytes(data_array, 72);

  // Reset enable pins
  digitalWrite(PLANE_1_PIN, 0);
  digitalWrite(PLANE_2_PIN, 0);
  digitalWrite(PLANE_3_PIN, 0);
  digitalWrite(PLANE_4_PIN, 0);

  // Latch
  digitalWrite(BLANK_AND_LATCH_PIN, 1);
  digitalWrite(BLANK_AND_LATCH_PIN, 0);

  // Set enable pin
  switch (current_led.z)
  {
  case 0:
    digitalWrite(PLANE_1_PIN, 1);
    current_led.z = 1;
    break;
  case 1:
    digitalWrite(PLANE_2_PIN, 1);
    current_led.z = 2;
    break;
  case 2:
    digitalWrite(PLANE_3_PIN, 1);
    current_led.z = 3;
    break;
  case 3:
    digitalWrite(PLANE_4_PIN, 1);
    current_led.z = 0;
    break;
  }
}

void select_next_led()
{
  switch (current_led.y)
  {
  case 0:
    current_led.x--;
    if (current_led.x < 0)
    {
      current_led.x = 0;
      current_led.y = 3;
    }
    break;
  case 1:
    current_led.x++;
    if (current_led.x > 3)
    {
      current_led.x = 3;
      current_led.y = 0;
    }
    break;
  case 2:
    current_led.x--;
    if (current_led.x < 0)
    {
      current_led.x = 0;
      current_led.y = 1;
    }
    break;
  case 3:
    current_led.x++;
    if (current_led.x > 3)
    {
      current_led.x = 3;
      current_led.y = 2;
    }
    break;
  }
}