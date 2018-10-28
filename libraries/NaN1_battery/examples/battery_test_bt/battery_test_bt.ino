
//require bt module and library!!!!


#include "NaN1_battery.h"
#include <WS2812B.h>
#include <bt_stack.h>

#define NUM_LEDS 6


NaN1_battery bat;
WS2812B strip = WS2812B(NUM_LEDS);
CRGB leds[NUM_LEDS];

bt_stack bt = bt_stack();

void init_leds()
{
  for(int i=0;i<NUM_LEDS;i++)
  {
    leds[i] = CRGB::White;
  }
   strip.show();
}

void setup()
{
 Serial.begin(115200);
 delay(7000);
 strip.begin(leds);
 //clear_leds();
 strip.setBrightness(64);
 init_leds();

 //bt.register_rx_callback(rx_callback);
 bt.begin();
 
 Serial.print("battery test started\n");

}

const uint32_t send_time = 1000;

uint64_t t=0;
bool first = true;
uint32_t elapsed_time = 0;
uint32_t start_voltage;
uint8_t batt_status = 0;
uint8_t batt_level = 0;

void loop()
{
  bt.loop();

 if(bt.get_connection_state() || !first)
 {
  if(first)
  {
    for(int i=0;i<100;i++)  //run bt loop after connect to avoid first message lost
     {
        bt.loop();
        delay(1);
     }
    start_voltage =  bat.read_bat_voltage(256);
    bt.printf("start voltage:%d\n", start_voltage);
    first = false;
    t = millis();
  }
  
 else if(t+send_time < millis())
  {
     uint32_t voltage = bat.read_bat_voltage();
     batt_status = bat.get_battery_status();
     batt_level = bat.get_battery_level();
     bt.printf("voltage:%d state:%d level:%d elapsed time:%d\n", voltage, batt_status, batt_level, elapsed_time);

     if(batt_level <= 20)
     {
      voltage = bat.read_bat_voltage();
      bt.printf("battery drain cycle completed start voltage:%d end voltage:%d dv:%d elapsed time:%d", start_voltage, voltage, start_voltage - voltage, elapsed_time);
      while(1)
      {
       bt.loop();
      }
     }

     t = millis();
     elapsed_time++;
   
  }
 }
  
}
