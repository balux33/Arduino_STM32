#include <WS2812B.h>
#include "NaN1_button.h"

#define NUM_LEDS 6

WS2812B strip = WS2812B(NUM_LEDS);
CRGB leds[NUM_LEDS];

CRGB color = CRGB::Black;

NaN1_button left  = NaN1_button(left_button);
NaN1_button right = NaN1_button(right_button);
NaN1_button middle = NaN1_button(middle_button);

void setup()
{
 Serial.begin(115200);
 strip.begin(leds);
 fill_solid(leds, NUM_LEDS, CRGB::Black);
 strip.setBrightness(64);
 
 strip.show();

 delay(500);
}

void loop()
{
 left.refresh();
 right.refresh();
 middle.refresh();
  
 if(left.press_detect())
 {
  color.red = color.red + 8;
 }

 if(right.press_detect())
 {
  color.green = color.green + 8;
 }

 if(middle.release_detect())
 {
  color.blue = color.blue + 8;
 }

 leds[0] = color;
 leds[3] = color;

 strip.show();

 delay(200);

}
