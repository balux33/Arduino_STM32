#include <WS2812B.h>

#define NUM_LEDS 6


WS2812B strip = WS2812B(NUM_LEDS);
CRGB leds[NUM_LEDS];

void setup() 
{
  strip.begin(leds);
  strip.show();// Clears the strip, as by default the strip data is set to all LED's off.
  strip.setBrightness(64);
}

void loop() 
{ 
  colorWipe(CRGB::Red,100); // Red
  colorWipe(CRGB::Green, 100); // Green
  colorWipe(CRGB::Blue, 100); // Blue
  colorWipe(CRGB::DarkCyan, 100); 
  colorWipe(CRGB::GreenYellow, 100); 
  //colors from https://github.com/FastLED/FastLED/wiki/Pixel-reference
}

// Fill the dots one after the other with a color
void colorWipe(CRGB color, uint8_t wait) 
{
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
      leds[i] = color;
      strip.show();
      delay(wait);
  }

   delay(wait*2);
   
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
      leds[i] = CRGB::Black;
      strip.show();
      delay(wait);
  }
}
