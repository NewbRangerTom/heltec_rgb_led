#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include "ledgfx.h"

// basic lighting of full strip
void lightFullStrip(){

  for (int i = 0; i < NUM_LEDS; i++)
    h_LEDs[i] = CRGB::Red;

};


void UkrainFlag(){

  FastLED.clear();
  for (int i = 0; i < UK_LEDS; i++){
    h_LEDs[i] = CRGB::Yellow;
  }
  for (int j = UK_LEDS; j < NUM_LEDS; j++){
    h_LEDs[j] = CRGB::Blue;
  }
  FastLED.show(h_Brightness);

};