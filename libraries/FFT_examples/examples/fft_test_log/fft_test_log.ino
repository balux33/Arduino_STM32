// Real-time audio spectrum analyzer with LCD support
// By Beherith (mysterme at gmail dot com) license: Public Domain
//modified to NaN1 board

//FFT stuff------------------------------------------------------------------
#define FFTLEN 1024
#include "cr4_fft_1024_stm32.h"
uint16_t data16[FFTLEN];
uint32_t data32[FFTLEN];
uint32_t y[FFTLEN];
const uint16_t fft_len = FFTLEN;


#include <libmaple/dma.h>

//WS2812B stuff---------------------------------------------------------------
#include <WS2812B.h>
#define NUM_LEDS 6

WS2812B strip = WS2812B(NUM_LEDS);
CRGB leds[NUM_LEDS];

//DMA--------------------------------------------------------------------------
volatile static bool dma1_ch1_Active;
#include <libmaple/pwr.h>
#include <libmaple/scb.h>
#include <libmaple/rcc.h>
#include <libmaple/adc.h>

//Other stuff------------------------------------------------------------------
const int8_t analogInPin = PA4; // CONNECT YOUR ANALOG SOURCE HERE!
uint32_t tick = 0;
const uint32_t sampleRate = (35714*2);
const int bin_size_in_Hz = sampleRate / FFTLEN; 
#define BTN1 PB7
#define BTN2 PA5

//#define debug_time
//#define debug_time_led



uint16_t asqrt(uint32_t x) { //good enough precision, 10x faster than regular sqrt
  /*      From http://medialab.freaknet.org/martin/src/sqrt/sqrt.c
   *   Logically, these are unsigned. We need the sign bit to test
   *   whether (op - res - one) underflowed.
   */
  int32_t op, res, one;

  op = x;
  res = 0;
  /* "one" starts at the highest power of four <= than the argument. */
  one = 1 << 30;   /* second-to-top bit set */
  while (one > op) one >>= 2;
  while (one != 0) {
    if (op >= res + one) {
      op = op - (res + one);
      res = res +  2 * one;
    }
    res /= 2;
    one /= 4;
  }
  return (uint16_t) (res);
}



void perform_fft(uint32_t * indata, uint32_t * outdata,const int len)
{
  cr4_fft_1024_stm32(outdata,indata,len);
  inplace_magnitude(outdata,len);
}

void fill(uint32_t * data, uint32_t value, int len)
{
  for (int i =0; i< len;i++) data[i]=value;
}

void fill(uint16_t * data, uint32_t value, int len)
{
  for (int i =0; i< len;i++) data[i]=value;
}

void real_to_complex(uint16_t * in, uint32_t * out, int len)
{
  for (int i = 0;i<len;i++) out[i]=in[i]<<3;
}


void setADCs ()
{
  rcc_set_prescaler(RCC_PRESCALER_ADC,RCC_ADCPRE_PCLK_DIV_4 );

  int pinMapADCin = PIN_MAP[analogInPin].adc_channel;
  adc_set_sample_rate(ADC1, ADC_SMPR_239_5);  //~37.65 khz sample rate??? 9MHz/(239.5+12.5) = 35.71kHz !!!!!
 
  adc_set_reg_seqlen(ADC1, 1);
  ADC1->regs->SQR3 = pinMapADCin;
  ADC1->regs->CR2 |= ADC_CR2_CONT; // | ADC_CR2_DMA; // Set continuous mode and DMA
  ADC1->regs->CR2 |= ADC_CR2_SWSTART;

  //Serial.println("Calibrating ADC1");
  //adc_calibrate(ADC1); //TODO: Calibration doesnt ever return for some odd reason...
}

static void DMA1_CH1_Event()
{
  dma1_ch1_Active = 0;
}

void adc_dma_enable(const adc_dev * dev)
{
  bb_peri_set_bit(&dev->regs->CR2, ADC_CR2_DMA_BIT, 1);
}


void takeSamples()
{
#ifdef debug_time 
  Serial.print("tick:");
  Serial.println(micros()-tick);
  tick = micros();
#endif 
 // real_to_complex(data16,data32,FFTLEN);//clear inputs
  // perform DMA, 
  dma_init(DMA1);
  dma_attach_interrupt(DMA1, DMA_CH1, DMA1_CH1_Event);

  adc_dma_enable(ADC1);
  dma_setup_transfer(DMA1, DMA_CH1, &ADC1->regs->DR, DMA_SIZE_16BITS, data16, DMA_SIZE_16BITS, (DMA_MINC_MODE | DMA_TRNS_CMPLT));// Receive buffer DMA
  dma_set_num_transfers(DMA1, DMA_CH1, FFTLEN );
  dma1_ch1_Active = 1;
  
  dma_enable(DMA1, DMA_CH1); // Enable the channel and start the transfer.

 
    while (dma1_ch1_Active){};    //Wait for the DMA to complete
    dma_disable(DMA1, DMA_CH1); //End of trasfer, disable DMA and Continuous mode.
    real_to_complex(data16,data32,FFTLEN);//clear inputs
    perform_fft(data32, y, FFTLEN);
  
 
  if (digitalRead(BTN1) == LOW)
  {  //print everything to serial, very slow obviously
    printdataset(data32, FFTLEN, 0);
    printdataset(y, FFTLEN, sampleRate); 
  }   

  if (digitalRead(BTN2) == LOW)
  {  //print everything to serial, very slow obviously
    printdataset(data32, FFTLEN, 0);
   // printdataset(y, FFTLEN, sampleRate); 
  } 
  
}




void inplace_magnitude(uint32_t * target, uint16_t len)
{
  for (int i=0;i<len;i++)
 {
     int16_t real = target[i] & 0xFFFF;
     int16_t imag = target[i] >> 16;
     uint32_t magnitude = asqrt(real*real + imag*imag);
     target[i] = magnitude; 
  }
}



float bin_frequency(uint32_t samplerate, uint32_t binnumber, uint32_t len)
{
  //Serial.printf("srate:%d n:%d, len:%d\n",samplerate,binnumber, len);
   return (((float)binnumber*samplerate)/((float)len));
}

uint16_t frequency_to_bin(uint32_t freq)  //use global fft_len and samplerate variables
{
   if(freq > sampleRate)
     freq = sampleRate;
   
   uint16_t bin = (unsigned int)(((float)(freq * fft_len) /sampleRate) + (float)0.5); //round to nearest
   //Serial.printf("bin:%d\n",bin);
   return bin;
   
}

float band_rms(uint32_t * fft_data, uint16_t start_freq, uint16_t end_freq)
{
  uint16_t start_bin = frequency_to_bin(start_freq);
  uint16_t end_bin   = frequency_to_bin(end_freq);

 if(start_bin == 0) start_bin +=1;

 //Serial.printf("start:%d end:%d\n",start_bin, end_bin);
  
  float rms = 0.0;
  
  for(int i = start_bin; i < end_bin; i++)  //centered bins??
  {
    rms += (fft_data[i]*fft_data[i]);
  }

  rms /= (end_bin - start_bin) ;
  rms = sqrt(rms);

  return rms;
}

#define color_steps 7
CRGB c_lut[16] = {CRGB::Green, CRGB::Yellow, CRGB::Yellow, CRGB::DarkOrange, CRGB::DarkOrange, CRGB::OrangeRed,  CRGB::Red};

CRGB ampl_to_color(float ampl, float max_ampl)
{
  float step = max_ampl/7;

  if(ampl != 0.0)
  {
    if(ampl > max_ampl) ampl = max_ampl;
    uint8_t i = ampl/step;
    return c_lut[i];
  }
  else
  {
    return CRGB::Black;
  }
}

const uint16_t band_table[] = {0,400,1400,2800,4300,5800,7200};

void print_to_led(uint32_t * data)
{
  float ampl_db ;

  const float treshold = 27.0;
  const float max_ampl = 43.0;
  const float max_ampl_first_bin = 48;
  
  for(uint8_t i=0;i<6;i++)
  {
    ampl_db = 20.0*log10(band_rms(data,band_table[i], band_table[i+1]));
    ampl_db -= treshold;
    ampl_db = ampl_db < 0.0 ? 0.0 : ampl_db;
    if(i != 0)
      leds[i] = ampl_to_color(ampl_db, max_ampl);
    else
      leds[i] = ampl_to_color(ampl_db, max_ampl_first_bin);
  }

  strip.show();
}


void printdataset(uint32_t * data, int len, int samplerate)
{
  int max = 0;
	Serial.print("Printing dataset at ");
	Serial.println((long long unsigned int) data, HEX);
	if (samplerate > 0)
	{
		Serial.println("Bin#	freq	mag");
		for (int i =0; i< len/2; i++)  //print only first half
		{
			Serial.print(i);
			Serial.print("	");
			Serial.print(bin_frequency(samplerate,i,len));
			Serial.print("	");
			Serial.println(data[i]);
		}
	}
	else
	{
		Serial.println("i	value");
		for (int i =0; i< len; i++)
		{
			Serial.print(i);
			Serial.print("	");
			Serial.println(data[i]);
      if(data[i] > max) max = data[i];
		}
   //Serial.print("maximum:");
   //Serial.println(max-16384);
	} 
}

//-----------------------setup, loop----------------------------------------

void setup() 
{
  Serial.begin(115200);
  Serial.println("Testing cr4_fft_1024_stm32");

  strip.begin(leds);// Sets up the SPI
  strip.setBrightness(25);
  strip.show();
 
  //initialize FFT variables
  fill(y,0,FFTLEN);
  fill(data32,1,FFTLEN);
  fill(data16,1,FFTLEN);

  setADCs();
  Serial.println("ADCs set");
   
  
  digitalWrite(BTN1, HIGH);
  pinMode(BTN1, INPUT_PULLUP);
  digitalWrite(BTN2, HIGH);
  pinMode(BTN2, INPUT_PULLUP);
}


void loop()
{ 
 while(1)
 { // loop seems to run only once, hence the while 
	takeSamples();
  print_to_led(y);
 }
}
