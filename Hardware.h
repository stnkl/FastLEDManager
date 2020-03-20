#pragma once

#include <Arduino.h>
#include <Ticker.h>
// Suppress FastLED warnings
#define FASTLED_INTERNAL
#include "FastLED.h"
#include "SPI.h"

#include "Tables/gamma_correction_table.h"
#include "Animation.h"
#include "Fade.h"

#define NUM_LEDS 6
#define LIGHTSTRIP_PIN 5
#define BUTTON_PIN 4

#define MAX_BETTER_HUE 1535

#define TIMER1_TICKS_PER_US (APB_CLK_FREQ / 1000000L)
#define TRANSMIT_INTERVAL_US 1700 //1250 // 800Hz (4*200Hz)
#define PLANE_1_PIN 16
#define PLANE_2_PIN 4
#define PLANE_3_PIN 5
#define PLANE_4_PIN 3
#define BLANK_AND_LATCH_PIN 12

struct gamma_corrected_rgb
{
  uint16_t red;
  uint16_t green;
  uint16_t blue;
};

struct XYZ
{
  int8_t x;
  int8_t y;
  int8_t z;
};

extern CRGB rgb_data[4][4][4];
extern int16_t brightness8;
extern float filteredBrightness;
extern gamma_corrected_rgb gamma_corrected_rgb_data[4][4][4];

void initHardware();
void betterShow(int16_t newBrightness = -1);
void betterClear();
CRGB betterHue(uint16_t fract1535, int16_t sat = -1, uint8_t val = 255);
void fill_gradient(CRGB color1, CRGB color2);
void fill_solid(CRGB color);
uint16_t getPotiBrightness();
void handleInput();
void transmit();
void select_next_led();

struct FastLEDDummy
{
  static void show() { betterShow(); };
};
#define FastLED \
  FastLEDDummy {}