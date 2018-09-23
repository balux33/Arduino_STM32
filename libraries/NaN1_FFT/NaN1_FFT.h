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
   void print_to_led();
   CRGB ampl_to_color(float ampl, float max_ampl);
   float get_band_rms(uint16_t start_freq, uint16_t end_freq);
   
  private:
    //fft buffers
	uint16_t data16[FFTLEN];
	uint32_t data32[FFTLEN];
	uint32_t fft_result[FFTLEN];
	
	//fft tables
	uint16_t band_table[7] = {0,400,1400,2800,4300,5800,7200};
	CRGB c_lut[6] = {CRGB::Green, CRGB::Green, CRGB::Yellow, CRGB::Yellow, CRGB::Red,  CRGB::Red};
	float treshold_table[band_table_size-1] = {27,27,27,27,27,27};
	float max_ampl_table[band_table_size-1] = {43,41,41,41,41,41};
	
	//led buffer
	CRGB *leds = NULL;
	
	//private functions
	void  printdataset(uint32_t * data, int len, int samplerate);
	
    
  
	
	
};

#endif