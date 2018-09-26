#ifndef nan1_fft_h
#define nan1_fft_h

#include "Arduino.h"


#include "cr4_fft_1024_stm32.h"

#include <WS2812B.h>

#include <libmaple/dma.h>
#include <libmaple/pwr.h>
#include <libmaple/scb.h>
#include <libmaple/rcc.h>
#include <libmaple/adc.h>

#define BTN1 PB7
#define BTN2 PA5

//configs
#define FFTLEN 1024

#define analogInPin PA4 // CONNECT YOUR ANALOG SOURCE HERE!
#define sampleRate  ((uint32_t)(35714*2))
#define bin_size_in_Hz  ((uint32_t)(sampleRate / FFTLEN)) 

#define band_table_size 7
#define c_lut_size      6







class NaN1_FFT
{
  public:
   NaN1_FFT(CRGB *leds_, uint16_t *band_table_ = NULL, CRGB *c_lut_ = NULL, float *treshold_table_ = NULL , float *max_ampl_table_ = NULL);
   
   void begin();
   void do_FFT();
   void print_to_led(bool mode = true);  //default rms
   void print_to_led_one_band(uint8_t band = 0, bool mode = false);  //dafault max
   CRGB ampl_to_color(float ampl, float max_ampl);
   float get_band_rms(uint16_t start_freq, uint16_t end_freq);
   float get_band_max(uint16_t start_freq, uint16_t end_freq);
   void change_treshold_table(float *treshold_table_) {if(treshold_table_ != NULL) treshold_table = treshold_table_;}
   void change_max_ampl_table(float *max_ampl_table_) {if(max_ampl_table_ != NULL) max_ampl_table = max_ampl_table_;}
   void change_band_table(uint16_t *band_table_) {if(band_table_ != NULL) band_table = band_table_;}
   void change_color_table(CRGB *c_lut_) {if(c_lut_ != NULL) c_lut = c_lut_;}
   
  private:
    //fft buffers
	uint16_t data16[FFTLEN];
	uint32_t data32[FFTLEN];
	uint32_t fft_result[FFTLEN];
	
	//fft tables
	const uint16_t band_table_default[7] = {0,400,1400,2800,4300,5800,7200};
	const CRGB c_lut_default[6] = {CRGB::Green, CRGB::Green, CRGB::Yellow, CRGB::Yellow, CRGB::Red,  CRGB::Red};
	const float treshold_table_default[band_table_size-1] = {27,27,27,27,27,27};
	const float max_ampl_table_default[band_table_size-1] = {43,41,41,41,41,41};
	
	 uint16_t *band_table = (uint16_t*)band_table_default;
	 CRGB *c_lut = (CRGB*)c_lut_default;
	 float *treshold_table = (float*)treshold_table_default;
	 float *max_ampl_table = (float*)max_ampl_table_default;
	 
	//led buffer
	CRGB *leds = NULL;
	
	//private functions
	void  printdataset(uint32_t * data, int len, int samplerate);
	void draw_to_bar(float ampl, float max_ampl);
	CRGB ampl_to_color_bar(float ampl, float max_ampl, uint8_t row);
	
    
  
	
	
};

#endif