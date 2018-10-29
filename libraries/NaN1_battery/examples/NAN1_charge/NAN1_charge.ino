/*
 * Hold down the middle button before turning on your NAN1 board to enter 'Charge indicator' mode
 */

#include "NaN1_battery.h"
#include <WS2812B.h>

#define NUM_LEDS 6

NaN1_battery bat;
WS2812B strip = WS2812B(NUM_LEDS);
CRGB leds[NUM_LEDS];


void init_leds()
{
  for(int i=0;i<NUM_LEDS;i++)
  {
    leds[i] = CRGB::Black;
  }
   strip.show();
}

void charge()
{
  for(int i=0;i<NUM_LEDS;i++)
      leds[i] = CRGB::Blue;
  strip.show();
  while(digitalRead(middle_button)==0)
    delay(1);
  while(digitalRead(middle_button)==1) //You can exit charge indicator mode if you hold the middle button down
  {
    uint8_t batt_level = 0;
    batt_level = bat.get_battery_level();
    //Serial.println("batt_level "+ String(batt_level) + " voltage: "+String(bat.read_bat_voltage()));
    int fullLedsCount = ((batt_level+1)/16.66);
    //Serial.println("full_LEDS"+ String(fullLedsCount));
    for(int i=0;i<NUM_LEDS;i++)
      leds[i] = CRGB::Black;
    int i = 0;
    for(i=0;i<fullLedsCount;i++)
      leds[i] = CRGB::Green;
    if(i<6)
    {
      double left = batt_level-(fullLedsCount*16.66);
      //Serial.println("left"+ String(left));
      if(left<5.55)
        leds[i] = CRGB::Red;
      else if(left<11,11)
        leds[i] = CRGB::Orange;
    }
    strip.show();
    delay(5000);
  }
  for(int i=0;i<NUM_LEDS;i++)
      leds[i] = CRGB::Black;
  strip.show();
}


void setup()
{
  //Serial.begin(115200);
  pinMode(middle_button, INPUT_PULLUP);
  strip.begin(leds);
  strip.setBrightness(5);
  init_leds();
  delay(200);
  if(digitalRead(middle_button)==0)
   {
    charge(); 
   }

   //PLACE ADDITIONAL CODE HERE IF NEEDED. THIS CODE WILL NOT RUN UNTIL THE BOARD IS IN 'Charge indicator' MODE
}

void loop()
{
  //YOUR MAIN CODE
}
