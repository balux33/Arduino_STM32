#include <WS2812B.h>

#define NUM_LEDS 6

WS2812B strip = WS2812B(NUM_LEDS);
CRGB leds[NUM_LEDS];

void setup() 
{
  strip.begin(leds);
  //strip.set_actual_buffer(leds);  //able to change buffer
  strip.show();// Clears the strip, as by default the strip data is set to all LED's off.
  strip.setBrightness(128);
}

void loop() 
{ 
  palette_test(40);
 colorWipe(CRGB::Green, 200); 
 colorWipe(CRGB::Crimson,200); 
 colorWipe(CRGB::Blue,200); 

 hsv_test2(200);
 confetti(10, 200);
 rainbow(20);
 
 fade_test(CHSV(10, 255, 120), 500);
 
 
}

DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
  0,     0,  0,  0,   //black
80,   255,  0,  0,   //red
170,   255,255,  0,   //bright yellow
255,   255,255,255 }; //full white

CRGBPalette16 myPal = heatmap_gp;

void palette_test(uint16_t wait)
{
  fill_solid( leds, NUM_LEDS, CRGB::Black );
  for(int i=0;i<255;i++)
  {
     leds[0] = ColorFromPalette( myPal, i); 
     strip.show(); 
     delay(wait);
  }
}



void confetti(int runs, uint16_t wait) 
{
  for(int i=0;i<runs;i++)
  {
     // random colored speckles that blink in and fade smoothly
     fadeToBlackBy( leds, NUM_LEDS, 100);
    int pos = random(NUM_LEDS);
    leds[pos] += CHSV((i/32)%128 + random(64), 200, 255);
    strip.show(); 
    delay(wait);
  }
}


void rainbow(uint16_t wait) 
{
  for(int i=0;i<240;i++)
  {
    // FastLED's built-in rainbow generator
    fill_rainbow( leds, NUM_LEDS, i, 15);
    strip.show(); 
    delay(wait);
  }
}

void fade_test(CRGB color, uint16_t wait)
{
  uint16_t step_size = 255 / NUM_LEDS;

  fill_solid( leds, NUM_LEDS, color );
   strip.show(); 
  
  for(uint16_t i=0; i<NUM_LEDS; i++) 
  {
    leds[i].nscale8((255 - (i+1)*step_size)+10);
     strip.show(); 
    delay(wait);
  }

  delay(wait*10);
}

void hsv_test(uint16_t wait)
{
 static uint8_t hue = 0;
 
  // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS; i++)
  {
    // Set the i'th led to red 
    leds[i] = CHSV(hue=hue+16, 255, 180);
    // Show the leds
    strip.show(); 
    delay(wait);
  }

  // Now go in the other direction.  
  for(int i = (NUM_LEDS)-1; i >= 0; i--)
  {
    // Set the i'th led to red 
    leds[i] = CHSV(hue=hue+16, 255, 180);
    // Show the leds
    strip.show();
    delay(wait);
  }
}

void hsv_test2(uint16_t wait)
{
 uint16_t hue = 0;
 uint8_t i = 0;
 
  for(hue = 0; hue < 255; hue= hue+4)
  {
    leds[i] = CHSV(hue, 255, 180);
    i++;
    i = i%NUM_LEDS;
    // Show the leds
    strip.show(); 
    delay(wait);
  }

  
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) 
{
  for(uint16_t i=0; i<NUM_LEDS; i++) 
  {
      leds[i] = c;
      strip.show();
      delay(wait);
  }

   delay(wait*2);
   
  for(uint16_t i=0; i<NUM_LEDS; i++) 
  {
      leds[i] = 0x00;
      strip.show();
      delay(wait);
  }
}
