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
}

void loop()
{
 leds[0] = CRGB::Red;
 strip.show();
 delay(500);

 leds[0] = CRGB::Black;
 strip.show();
 delay(500);
}
