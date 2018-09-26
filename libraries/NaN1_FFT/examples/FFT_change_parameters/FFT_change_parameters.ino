#include "NaN1_FFT.h"
#include "NaN1_button.h"
#include <WS2812B.h>

#define NUM_LEDS 6

WS2812B strip = WS2812B(NUM_LEDS);
CRGB leds[NUM_LEDS];

uint16_t band_table[7] = {0,4000,1200,2200,3200,5200,6200};
CRGB color_table[6] = {CRGB::Green, CRGB::GreenYellow, CRGB::Yellow, CRGB::Orange, CRGB::DarkOrange,  CRGB::Red};
float treshold_table[band_table_size-1] = {27,27,20,20,20,20};

float max_ampl_table1[band_table_size-1] = {47,41,41,41,41,41};
float max_ampl_table2[band_table_size-1] = {35,35,30,30,30,30};

NaN1_FFT fft = NaN1_FFT(leds, band_table, color_table, treshold_table, max_ampl_table1);
//NaN1_FFT fft = NaN1_FFT(leds);

NaN1_button left  = NaN1_button(left_button);
NaN1_button right = NaN1_button(right_button);


void setup()
{
 delay(500);
   
 Serial.begin(115200);
 strip.begin(leds);
 fill_solid(leds, NUM_LEDS, CRGB::Black);
 strip.setBrightness(64);
 
 strip.show();

 fft.begin();

 delay(500);
}

void loop()
{
   left.refresh();
   right.refresh();

   if(left.press_detect())
   {
      fft.change_max_ampl_table(max_ampl_table1);
   }

   if(right.press_detect())
   {
      fft.change_max_ampl_table(max_ampl_table2);
   }
 
  fft.do_FFT();
  fft.print_to_led_one_band(0);
  strip.show();
}


