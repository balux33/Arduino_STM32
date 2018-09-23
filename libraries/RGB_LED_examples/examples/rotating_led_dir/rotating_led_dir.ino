#include <WS2812B.h>

#define NUM_LEDS 6


WS2812B strip = WS2812B(NUM_LEDS);
CRGB leds[NUM_LEDS];

void setup() 
{
  strip.begin(leds);
  strip.show();// Clears the strip, as by default the strip data is set to all LED's off.
  strip.setBrightness(20);
  pinMode(left_button, INPUT_PULLUP);
  pinMode(right_button, INPUT_PULLUP);
}

const CRGB background = CRGB::Black;
const CRGB color     = CRGB::Red;
const int speed = 150;
int i = 0;
bool direction = true;

void loop() 
{ 
  fill_solid(leds, NUM_LEDS, background);

  leds[i] = color;
  leds[i+3] = color;
  
  if(direction == false)
  {
    if(i == 2) i = 0;
    else i++;
  }
  else
  {
    if(i== 0) i = 2;
    else i--;
  }

  if(digitalRead(left_button) == button_pressed)
  {
    direction = false;
  }
  else if(digitalRead(right_button) == button_pressed)
  {
    direction = true;
  }

   strip.show();
  delay(speed);
}





