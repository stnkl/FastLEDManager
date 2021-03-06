#include "Spectroscope.h"

uint8_t spectroscopeData[22] = {0};
CRGB color_1, color_2 = CRGB::Red;

void updateSpectroscope(uint8_t *arr, bool isSymmetrical)
{
  stopFade();
  if (currentAnimation)
    currentAnimation->stop();

  // Spectrum
  for (uint8_t i = 0; i < 16; i++)
  {
    spectroscopeData[i] = arr[i];
  }
  // Colors
  color_1 = CRGB(arr[16], arr[17], arr[18]);
  color_2 = CRGB(arr[19], arr[20], arr[21]);

  uint16_t stripe_width = isSymmetrical ? NUM_LEDS / 32 : NUM_LEDS / 16;
  uint16_t current_led = 0;
  for (int i = 0; i < 16; i++)
  {
    for (int j = 0; j < stripe_width; j++)
    {
      leds[current_led].red = color_1.red * spectroscopeData[i] / 255 * (15 - i) / 15 + color_2.red * spectroscopeData[i] / 255 * i / 15;
      leds[current_led].green = color_1.green * spectroscopeData[i] / 255 * (15 - i) / 15 + color_2.green * spectroscopeData[i] / 255 * i / 15;
      leds[current_led].blue = color_1.blue * spectroscopeData[i] / 255 * (15 - i) / 15 + color_2.blue * spectroscopeData[i] / 255 * i / 15;
      // Copy to second half
      if (isSymmetrical)
      {
        leds[NUM_LEDS - current_led].red = leds[current_led].red;
        leds[NUM_LEDS - current_led].green = leds[current_led].green;
        leds[NUM_LEDS - current_led].blue = leds[current_led].blue;
      }
      current_led++;
    }
  }
}

void symmetricalSpectroscope(uint8_t *arr)
{
  updateSpectroscope(arr, true);
}

void linearSpectroscope(uint8_t *arr)
{
  updateSpectroscope(arr, false);
}