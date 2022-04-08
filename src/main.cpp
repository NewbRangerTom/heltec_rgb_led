//---------------------------------------------------------------------------------
//
//  Arduino LED Tutorial - (c) 2020 Dave Plummer, All Rights Reserved.
//
//  File:         RGB_FANS/main.cpp
//
//  Description:
//
//    Draw sample effects on an addressable RGB LED strip and prints stats to
//    the OLED display on a Heltec ESP32 module.
//  History:      Feb-7-2022    tomwer    Copied for personal use and learning
//                Feb-22-2022   me        refactored with EVERY_N_MILLISECONDS
//                                        refactored comet using beat timers
//
//---------------------------------------------------------------------------------

#include <Arduino.h>
#include <U8g2lib.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include "BluetoothSerial.h"

BluetoothSerial ESP_BT;

#define OLED_SCL 15
#define OLED_SDA 4
#define OLED_RST 16
U8G2_SSD1306_128X64_NONAME_F_HW_I2C h_oled(U8G2_R2, OLED_RST, OLED_SCL, OLED_SDA);  //  Constructor for OLED display

// For FastLED
#define NUM_LEDS        60          // number of LEDs - using 60 out of 60
#define UK_LEDS         30
#define LED_PIN         5
CRGB h_LEDs[NUM_LEDS] = {0};       // Frame buffer for FastLED

int h_lineHeight = 0;              //  Line height based on current font
int h_Brightness = 128;             //  brightness range 0 - 255  !!! CAUTION: setting to 255 could have negative effects if underpowered
int h_PowerLimit = 3000;           //  900mW Power Limit

#define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))
#define TIMES_PER_SECOND(x) EVERY_N_MILLISECONDS(1000 / x)

// LED effect headers
#include "ledgfx.h"
#include "marquee.h"
#include "twinkle.h"
#include "comet.h"
#include "bounce.h"
#include "fire.h"
#include "lightmystrip.h"

//-----------------------------------------------------------------------------------------------------------------------------
// FramesPerSecond  ->  depricated
//
//  Tracks a weighted average in order to smooth out the values that are given.  Computes the FPS as the simple reciprocal
//  of the amount of time taken specified by the called.  So 1/3 of a second is 3 fps.  It takes about 10 frames to stabilize.
// double FramesPerSecond(double seconds){

//   static double framesPerSecond;
//   framesPerSecond = (framesPerSecond * 0.9) + (1.0 / seconds * 0.1);
//   return framesPerSecond;
// }
//------------------------------------------------------------------------------------------------------------------------------

//  Bouncing Ball Effect
//
// (length, count, fade, mirrored)
// Creating instance of BouncingBallEffect called balls
BouncingBallEffect balls(NUM_LEDS, 8, 32, true);
IceFireEffect ice(NUM_LEDS, 30, 100, 3, 4, true, true);           // f-f = end -> 0 : t-f = 0 -> end : f-t = center -> out : t-t = ends -> center
FireEffect fire(NUM_LEDS, 30, 100, 3, 4, true, true);             // f-f = end -> 0 : t-f = 0 -> end : f-t = center -> out : t-t = ends -> center

void MyIceFire(){
    FastLED.clear();
    ice.DrawIceFire();
    FastLED.show(h_Brightness);
};

void MyFire(){
    FastLED.clear();
    fire.DrawFire();
    FastLED.show(h_Brightness);
};

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);                                   //  Builtin LED mode declaration
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  ESP_BT.begin("LED_Light_Show");
  while (!Serial){}
  Serial.println("ESP32 Startup");

  h_oled.begin();
  h_oled.clear();
  h_oled.setFont(u8g2_font_profont15_tf);
  h_lineHeight = h_oled.getFontAscent() - h_oled.getFontDescent();        //  Descent is a negative number so we add it to the total

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(h_LEDs, NUM_LEDS);               //  Add our LED strip to the FastLED library
  FastLED.setBrightness(h_Brightness);
  set_max_power_indicator_LED(LED_BUILTIN);                               //  Light the builtin LED if we power throttle
  FastLED.setMaxPowerInMilliWatts(h_PowerLimit);                          //  Set the power limit, above which brightness will be throttled
  FastLED.clear();

}

void loop() {

  // Set LED pin state
  // bool bLED = 0;
  // double fps = 0;  // dpericated

  uint8_t initialHue = 0;
  const uint8_t deltaHue = 16;
  const uint8_t hueDensity = 4;

  while (true){
    
    // bLED = !bLED;                                                           //  Toggles the LED state for each frame
    // digitalWrite(LED_BUILTIN, bLED);

    // double dStart = millis() / 1000.0;                                      //  Record the start time -> depricated by FastLED.getFPS method
    
    //  Handle OLED drawing

    // static unsigned long msLastUpdate = millis();                          //  depcicated by EVERY_N_MILLISECONDS
    // if (millis() - msLastUpdate > 250)
  
    EVERY_N_MILLISECONDS(250){
      
      h_oled.clearBuffer();
      h_oled.setCursor(0, h_lineHeight);
      // h_oled.printf("FPS: %.1lf", fps);
      h_oled.printf("FPS: %.u", FastLED.getFPS());                              //  calculate frames per second
      h_oled.setCursor(0, h_lineHeight * 2);
      // h_oled.printf("Power: %u mW", milliwatts);
      h_oled.printf("Power: %u mW", calculate_unscaled_power_mW(h_LEDs, NUM_LEDS));
      h_oled.setCursor(0, h_lineHeight * 3);
      h_oled.printf("Bright: %d", calculate_max_brightness_for_power_mW(h_Brightness, h_PowerLimit));
      h_oled.sendBuffer();
      // msLastUpdate = millis();  // depricated
    }
  
  //----------------------------------------------------------------------------------------------------
    // LED strip patern functions
    // uncomment the effect to use
    // This system works  for all functions 

    // EVERY_N_MILLISECONDS(20){ DrawMarqueeComparison(); }  // see annotations in marquee.h for options
    // balls.Draw();                                         //  Bouncing balls using BouncingBallEffect from bounce.h; comment out to use different effect
    // DrawComet();
    // DrawCometGfx();
    // DrawComet3();
    // DrawMarquee();
    // DrawPixels(h_Leds, NUM_LEDS, CRGB::Green);
    // DrawTwinkle();
    // fill_solid(h_LEDs, NUM_LEDS, CRGB::Green);
    // fill_rainbow(h_LEDs, NUM_LEDS, initialHue += hueDensity, deltaHue);
    // h_LEDs[0] = CRGB::Red;   // light LED 0
    // lightFullStrip();
    // MyFire();
    // MyIceFire();
    // UkrainFlag();

    // this system only works for the static (single iteration) functions.
    char btRead = ESP_BT.read();
    if (btRead == 'a') DrawComet();                                         // dynamic
    if (btRead == 'b') DrawCometGfx();                                      // dynamic
    if (btRead == 'c') DrawComet3();                                        // dynamic
    if (btRead == 'd') balls.Draw();                                        // dynamic
    if (btRead == 'e') DrawMarquee();                                       // dynamic
    if (btRead == 'f') DrawPixels(h_LEDs[0], NUM_LEDS, CRGB::Green);        // static
    if (btRead == 'g') DrawTwinkle();                                       // dynamic
    if (btRead == 'h') fill_solid(h_LEDs, NUM_LEDS, CRGB::Green);           // static
    if (btRead == 'i') DrawTwinkleOne();                                    // dynamic
    if (btRead == 'j') h_LEDs[0] = CRGB::Red;                               // static
    if (btRead == 'k') lightFullStrip();                                    // static
    if (btRead == 'l') MyFire();                                            // dynamic
    if (btRead == 'm') MyIceFire();                                         // dynamic
    if (btRead == 'n') UkrainFlag();                                        // static
    if (btRead == 'o') FastLED.clear();                                     // sets all to HIGH / OFF / 0

  //----------------------------------------------------------------------------------------------------

    FastLED.setBrightness(h_Brightness);
    FastLED.delay(10);                                    // Show and delay

    // double dEnd = millis() / 1000.0;                    //  Record the completion time
    // fps = FramesPerSecond(dEnd - dStart);               //  Calculate the FPS rate
  
  }

}