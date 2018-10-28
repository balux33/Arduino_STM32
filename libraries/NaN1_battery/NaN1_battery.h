#ifndef nan1_battery_h
#define nan1_battery_h

#include "Arduino.h"

class NaN1_battery
{
  public:
   NaN1_battery() { };//pinMode(PB0, INPUT_ANALOG);}
    
  uint32_t read_bat_voltage(uint32_t averaging_number = 4); //return battery voltage in mV
  bool     get_power_source();                                  //return true if battery return false if usb
  uint8_t  get_battery_status();  //return 0-5 0 empty 4 fully charged 5 usb power
  uint8_t  get_battery_level(); //return 0-100(%)
	
};

#endif
