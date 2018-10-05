#include "check_chip.h"


bool check_fake_chip()  //return true if gd32f103 detected  or false if genuine stm32f103
{
  uint64_t loop_time = millis();
  for(int i=0;i<200000;i++)
  {
    asm("NOP");
    asm("NOP");
    asm("NOP");
  }

  loop_time = millis() - loop_time;

  

  if(loop_time < 20)
   {
      Serial.printf("loop_time:%d  fake chip detected!!\n");
      return true;
   }
   else
   {
      Serial.printf("loop_time:%d  genuine chip detected!!\n");
    return false;
   }
  
}