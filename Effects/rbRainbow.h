#ifndef _RBRAINBOW_H
#define _RBRAINBOW_H

#include <Arduino.h>
#include <Time.h>

#include "../EffectController.h"

extern EffectConfiguration rbRainbow;

namespace rbRainbowNamespace
{
	CRGB get_cyclic_color(uint16_t index);
	extern void reset();
	extern void tick();
}

#endif