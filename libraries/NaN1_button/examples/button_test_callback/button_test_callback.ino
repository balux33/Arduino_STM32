#include <WS2812B.h>
#include "NaN1_button.h"

#define NUM_LEDS 6

WS2812B strip = WS2812B(NUM_LEDS);
CRGB leds[NUM_LEDS];

CRGB color = CRGB::Black;

NaN1_button left  = NaN1_button(left_button);
NaN1_button right = NaN1_button(right_button);
NaN1_button midle = NaN1_button(midle_button);

void left_press_cb()
{
   color.red = color.red + 8;
}

void right_press_cb()
{
   color.green = color.green + 8;
}

void midle_release_cb()
{
   color.blue = color.blue + 8;
}

void setup()
{
 Serial.begin(115200);
 strip.begin(leds);
 fill_solid(leds, NUM_LEDS, CRGB::Black);
 strip.setBrightness(64);
 
 strip.show();

 left.register_press_callback(left_press_cb);
 right.register_press_callback(right_press_cb);
 midle.register_release_callback(midle_release_cb);

 delay(500);
}



void loop()
{
 left.refresh();
 right.refresh();
 midle.refresh();
  
 leds[0] = color;
 leds[3] = color;

 strip.show();

 delay(200);

}
