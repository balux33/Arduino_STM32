//require bt module and library!!!!
//use with Ryan Chen Color led controller android app

#include <bt_stack.h>
#include "NaN1_battery.h"

#include <WS2812B.h>

#define NUM_LEDS 6

NaN1_battery bat;


WS2812B strip = WS2812B(NUM_LEDS);
CRGB leds[NUM_LEDS];


bt_stack bt = bt_stack();



void rx_callback__(const char *data, uint16_t size)
{
  //char * data = (char*)data_;
  Serial.print(data);
  if(strstr(data, "red"))
  {
    leds[2] = CRGB::Red;
    strip.show();
  }
  else if(strstr(data, "green"))
  {
    leds[2] = CRGB::Green;
    strip.show();
  }
  else if(strstr(data, "blue"))
  {
    leds[2] = CRGB::Blue;
    strip.show();
  }
    
}

CRGB color = CRGB::Black;
void rx_callback(const char *data, uint16_t size)
{
  Serial.print(data);

  if(strstr(data, "OFF"))
  {
    leds[2] = CRGB::Black;
    strip.show();
  }
  else if(strstr(data, "ON"))
  {
    leds[2] = color;
    strip.show();
  }
  else if(size > 5)
  {
    sscanf(data, "%hhu.%hhu.%hhu)", &color.red, &color.green, &color.blue);
    leds[2] = color;
    strip.show();
  }
  
}


void setup()
{
 delay(7000);
 Serial.begin(115200);
 strip.begin(leds);
 fill_solid(leds, NUM_LEDS, CRGB::Black);
 strip.setBrightness(128);
 
 strip.show();
 Serial.print("hello\n");

// bt.set_name("NAN1 RGB LED example");
 bt.register_rx_callback(rx_callback);
 bt.begin();

}

char buff[64];

uint64_t t=0;
void loop()
{
  if(t+1000 < millis())
  {
    if(! bt.get_connection_state())
       Serial.print("not connected\n");
    else
   {
       uint32_t voltage = bat.read_bat_voltage();
       Serial.printf("voltage:%d\n", voltage);
       bt.printf("voltage:%d\n",voltage);
    }
    t = millis();
  }
  
  bt.loop();
}
