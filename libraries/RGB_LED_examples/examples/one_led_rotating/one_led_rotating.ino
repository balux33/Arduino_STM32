#include <WS2812B.h>

#define NUM_LEDS 6

WS2812B strip = WS2812B(NUM_LEDS);
CRGB leds[NUM_LEDS];

void setup()
{
 strip.begin(leds);
 fill_solid(leds, NUM_LEDS, CRGB::Black);
 strip.setBrightness(32);
 
 strip.show();
 delay(100);
}

int i = 0;

void loop()
{
 fill_solid(leds, NUM_LEDS, CRGB::Black);
 leds[i] = CRGB::Red;
 strip.show();
 delay(200);

 if(i == 5)
 {
    i=0;
 }
 else
 {
   i = i + 1;
 }
}

