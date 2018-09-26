#ifndef nan1_button_h
#define nan1_button_h

#include "Arduino.h"

typedef void (*button_cb)();

class NaN1_button
{
  public:
  
  NaN1_button(uint32_t button_);
  
  void refresh();
  bool state();  //return true if pressed else false
  bool press_detect();  
  bool release_detect();
  
  void register_press_callback(button_cb f) {press_cb = f;};
  void register_release_callback(button_cb f) {release_cb = f;};
  
  private:
  
  void inline init_pin();
  
  uint32_t button;

  uint8_t button_prev_state = 1;
  
  button_cb press_cb = NULL;
  button_cb release_cb = NULL;
  
  bool press_event   = false;
  bool release_event = false;
  
  bool first = true;
};




#endif