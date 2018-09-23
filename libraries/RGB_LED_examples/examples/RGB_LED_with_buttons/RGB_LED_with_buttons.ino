#include <WS2812B.h>

#define NUM_LEDS 6

#define button_pressed  0
#define button_released 1

WS2812B strip = WS2812B(NUM_LEDS);
CRGB leds[NUM_LEDS];

CRGB color = CRGB::Black;

void setup()
{
 Serial.begin(115200);
 strip.begin(leds);
 fill_solid(leds, NUM_LEDS, CRGB::Black);
 strip.setBrightness(64);
 
 strip.show();

 pinMode(left_button,  INPUT_PULLUP);
 pinMode(right_button, INPUT_PULLUP);
 pinMode(midle_button, INPUT_PULLUP);

 delay(500);
}

void loop()
{
  
 if(digitalRead(left_button) == button_pressed)
 {
  color.red = color.red + 8;
 }

 if(digitalRead(right_button) == button_pressed)
 {
  color.green = color.green + 8;
 }

 if(digitalRead(midle_button) == button_pressed)
 {
  color.blue = color.blue + 8;
 }

 leds[0] = color;
 leds[3] = color;

 strip.show();

 delay(200);

}
