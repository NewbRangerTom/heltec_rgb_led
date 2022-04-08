//+--------------------------------------------------------------------------
//
// NightDriver - (c) 2020 Dave Plummer.  All Rights Reserved.
//
// File:        marque.h
//
// Description:
//
//   Draws a theatre-style marquee
//
// History:     Sep-15-2020     davepl      Created
//
//---------------------------------------------------------------------------

#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include "ledgfx.h"

void DrawMarquee(){
    
    static byte j = HUE_BLUE;
    j += 4;
    byte k = j;

    // Roughly equivalent to fill_rainbow(h_LEDs, NUM_LEDS, j, 8);

    CRGB c;
    for (int i = 0; i < NUM_LEDS; i ++)
        h_LEDs[i] = c.setHue(k+=8);

    static int scroll = 0;
    scroll++;

    for (int i = scroll % 5; i < NUM_LEDS - 1; i += 5)
    {
        h_LEDs[i] = CRGB::Black;
    }
    delay(50);
}

void DrawMarqueeComparison(){

  FastLED.clear();
  static float scroll = 0.0f;
  scroll += 0.1f;
  if (scroll > 5.0)
      scroll -= 5.0;
  
  for (float i = scroll; i < NUM_LEDS/2 -1; i += 5){
      // from ledgfx.h
      DrawPixels(i, 3, CRGB::Green);                        // scroll with fade starting at led 0 to led 29
    //   DrawPixels(NUM_LEDS-1-i, 3, CRGB::Blue);              // scroll with fade starting at led 58 to led 30
      DrawPixels(NUM_LEDS-1-(int)i, 3, CRGB::Red);          // scroll without fade strating at led 58 to led 30
  }
    
}