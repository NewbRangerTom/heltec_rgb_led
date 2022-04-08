//+--------------------------------------------------------------------------
//
// NightDriver - (c) 2020 Dave Plummer.  All Rights Reserved.
//
// File:                  
//
// Description:
//
//   
//
// History:     Sep-28-2020     davepl      Created
//
//---------------------------------------------------------------------------
#pragma once
#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

extern CRGB h_LEDs[];

void DrawComet(){
    
    // FastLED.clear(false);              // Uncomment this for cylon eye/knight rider effect

    const byte fadeAmt = 64;            // Fraction of 256 to fade a pixel by if it is chosen to be faded
    const int cometSize = 5;            // Size of the comet in pixels
    const int deltaHue  = 4;            // How far to step the cycling hue each draw cycle
    const double cometSpeed = 0.5;      // How far to advance the comet every frame

    static byte hue = HUE_RED;          // Current color
    static int iDirection = 1;          // Current direction (-1 or +1)
    static double iPos = 0.0;           // Current comet position on strip

    hue += deltaHue;                    // Update the comet color
    iPos += iDirection * cometSpeed;    // Update the comet position

    //  Flip the comet direction when it hits either end
    if (iPos == (NUM_LEDS - cometSize) || iPos == 0)
        iDirection *= -1;

    //  Draw the comet at its current position
    for (int i = 0; i < cometSize; i++)
        h_LEDs[(int)iPos + i].setHue(hue);
    
    // Randomly fade the LEDs  --  comment this section for cylon eye/knight rider effect 
    for (int j = 0; j < NUM_LEDS; j++)
        // if (random(10) > 5)                              //  Adds randomness to the fade of the comet tail
            h_LEDs[j] = h_LEDs[j].fadeToBlackBy(fadeAmt);

    delay(20);
}

// Uses the ledgfx.h header
void DrawCometGfx(){
    
    const byte fadeAmt = 128;
    const int cometSize = 5;
    const int deltaHue  = 4;

    static byte hue = HUE_RED;
    static int iDirection = 1;
    static int iPos = 0;

    hue += deltaHue;

    iPos += iDirection;
    if (iPos == (FastLED.count() - cometSize) || iPos == 0)
        iDirection *= -1;
    
    for (int i = 0; i < cometSize; i++)
        FastLED.leds()[iPos + i].setHue(hue);
    
    // Randomly fade the LEDs
    for (int j = 0; j < FastLED.count(); j++)
        if (random(10) > 5)
            FastLED.leds()[j] = FastLED.leds()[j].fadeToBlackBy(fadeAmt);  

    delay(30);
}

void DrawComet3(){
    
    EVERY_N_MILLISECONDS(20){
        
        fadeToBlackBy(h_LEDs, NUM_LEDS, 64);
        int cometSize = 15;
        int iPos = beatsin16(32, 0, NUM_LEDS - cometSize);
        byte hue = beatsin8(30);
        for (int i = iPos; i < iPos + cometSize; i++)
        h_LEDs[i] = CHSV(hue, 194, 127);      //  (hue, saturation, value)  Blue = hue 160
    }
}