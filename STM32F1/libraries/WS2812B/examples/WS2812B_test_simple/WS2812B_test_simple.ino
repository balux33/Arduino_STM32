#include <WS2812B.h>

#define NUM_LEDS 16
/*
 * Note. Library uses SPI1
 * Connect the WS2812B data input to MOSI on your board. (PA7)
 * 
 */
WS2812B strip = WS2812B(NUM_LEDS);

void setup() 
{
  strip.begin();// Sets up the SPI
  strip.show();// Clears the strip, as by default the strip data is set to all LED's off.
 // strip.setBrightness(8);
}

void loop() 
{ 
  colorWipe(strip.Color(50, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 50, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 50), 50); // Blue
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) 
{
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }

   delay(wait*2);
   
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
      strip.show();
      delay(wait);
  }
}