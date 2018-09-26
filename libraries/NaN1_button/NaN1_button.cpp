#include "NaN1_button.h"


NaN1_button::NaN1_button(uint32_t button_)
{
	  button = button_;
	 // pinMode(button,  INPUT_PULLUP);
}

void inline NaN1_button::init_pin()
{
	if(first)
	{
		first = false;
		pinMode(button,  INPUT_PULLUP);
		delay(10);
	}
	
}

void NaN1_button::refresh()
{
	init_pin();
	uint32_t curr_state = digitalRead(button);
	
	if(curr_state != button_prev_state)
	{
		if(curr_state == button_pressed) //press 
		{
			if(press_cb != NULL)
			{
				(*press_cb)();	
			}
			press_event = true;
		}
		else if(curr_state == button_released) //release
		{
			if(release_cb != NULL)
			{
				(*release_cb)();	
			}
			release_event = true;
		}
	}
	
	button_prev_state = curr_state;
}

bool NaN1_button::state()  //return true if pressed else false
{
	init_pin();
	if(digitalRead(button) == button_pressed)
		return true;
	else
		return false;
}

bool NaN1_button::press_detect()
{
	if(press_event) 
	{
		press_event = false;
		return true;
	}
	else
		return false;
}

bool NaN1_button::release_detect()
{
	if(release_event) 
	{
		release_event = false;
		return true;
	}
	else
		return false;
}
