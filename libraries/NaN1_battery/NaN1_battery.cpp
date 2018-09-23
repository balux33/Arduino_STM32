#include "NaN1_battery.h"



uint32_t NaN1_battery::read_bat_voltage(uint32_t averaging_number)
{
	uint32_t sum = 0;
	
	if(averaging_number == 0) return 0;
	
	for(uint32_t i=0;i<averaging_number;i++)
	{
		sum += analogRead(PB0);
	}
	sum /= averaging_number;
	
	return (sum*(3300*80))/(4096*33);  //vref 3300mV   33k/47k divider
}

bool NaN1_battery::get_power_source()  //return true if battery return false if usb
{
	if(read_bat_voltage() > 4300) 
		return false;  //usb
	else
		return true;  // battery
}

uint8_t NaN1_battery::get_battery_status()  //return 0-5 0 empty 4 fully charged 5 usb power
{
	uint32_t voltage = read_bat_voltage(8);
	
	if(voltage > 4250)   
		return 5; //usb power battery 
	
	else if(voltage > 4000)
		return 4;  // battery full charge
	
	else if(voltage > 3700)
		return 3;  // battery half charge
	
	else if(voltage > 3600)
		return 2;  // battery quarter charge
	
	else if(voltage > 3500)
		return 1;  // battery almost empty
	
	else           // <3500
		return 0;  //battery empty
}