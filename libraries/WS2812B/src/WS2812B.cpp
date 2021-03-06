/*-----------------------------------------------------------------------------------------------
  Arduino library to control WS2812B RGB Led strips using the Arduino STM32 LibMaple core
  -----------------------------------------------------------------------------------------------
 
  Note. 
  This library has only been tested on the WS2812B LED. It may not work with the older WS2812 or
  other types of addressable RGB LED, becuase it relies on a division multiple of the 72Mhz clock 
  frequence on the STM32 SPI to generate the correct width T0H pulse, of 400ns +/- 150nS
  SPI DIV32 gives a pulse width of 444nS which is well within spec for the WS2812B but
  is probably too long for the WS2812 which needs a 350ns pulse for T0H
 
  This WS2811B library is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  It is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  See   <http://www.gnu.org/licenses/>.
  -----------------------------------------------------------------------------------------------*/

#include "WS2812B.h"
#include "pins_arduino.h"
#include "wiring_private.h"
#include <SPI.h>


// Constructor when n is the number of LEDs in the strip
WS2812B::WS2812B(uint16_t number_of_leds):
  brightness(32), pixels(NULL)
{
  updateLength(number_of_leds);
}


WS2812B::~WS2812B() 
{
  if(pixels)   
  {
	  free(pixels);
  }
  SPI.end();
}


// becouse clk div >= 32 generate unwanted spikes on MOSI line (maybe fake stm32f103c8 chips?) Yes, only GD32f103 chips produce this strange thing
//therefore use div 16 or DIV 8 and generate bit pattern of 6 bit /symbol
//this version not optimized for speed

void WS2812B::begin(CRGB * CRGB_buffer)
 {
	if (!begun)
	{
		if(CRGB_buffer != NULL)
			actual_CRGB_buffer = CRGB_buffer;
	
		if( F_CPU==72000000)
		   SPI.setClockDivider(SPI_CLOCK_DIV16);
		else  //48MHz
		   SPI.setClockDivider(SPI_CLOCK_DIV8);
		SPI.setBitOrder(MSBFIRST);
		SPI.begin();
		if(!check_fake_chip())  //if genuine chip  disable spi clk alt func and PA5 work as R button input 
								//if gd32f103 chip detected do not disable clk pin otherwise SPI dont work WTF?? later R button rewired to other pins
		{
			 pinMode(PA5, INPUT_PULLUP);
		}
		begun = true;
	}
}

void WS2812B::begin()
 {
	 begin(NULL);
 }

void WS2812B::updateLength(uint16_t n)
{
  if(doubleBuffer) 
  {
	  free(doubleBuffer); 
  }

  numBytes = (n<<4) + n + n + 2; // 18 encoded bytes per pixel. 1 byte empty peamble to fix issue with SPI MOSI and on byte at the end to clear down MOSI 
							// Note. (n<<3) +n is a fast way of doing n*9
  if((doubleBuffer = (uint8_t *)malloc(numBytes*1)))
  {
    numLEDs = n;	 
	pixels = doubleBuffer;
	// Only need to init the part of the double buffer which will be interacted with by the API e.g. setPixelColor
	*pixels=0;//clear the preamble byte
	*(pixels+numBytes-1)=0;// clear the post send cleardown byte.
	clear();// Set the encoded data to all encoded zeros 
  } 
  else 
  {
    numLEDs = numBytes = 0;
	Serial.println("WS2812B malloc failed!!!");
  }
}



void WS2812B::show(CRGB * CRGB_buffer) //write  CRGB buffer to leds
{
	if(!canShow())  //time betwen show() less than 300us
	  return;   
	  
	if(CRGB_buffer == NULL)
		return;

	for(uint16_t i= 0; i< numLEDs; i++)
	{
		setPixelColor(i, CRGB_buffer[i].r, CRGB_buffer[i].g, CRGB_buffer[i].b);
	}
	
	//send_data_to_leds();
	SPI.dmaSendAsync(pixels,numBytes);
	
	endTime = micros();
}

void WS2812B::show() //write actual CRGB buffer to leds
{
	show(actual_CRGB_buffer);
}
 
 
 
/*Sets a specific pixel to a specific r,g,b colour 
* Because the pixels buffer contains the encoded bitstream, which is in triplets
* the lookup table need to be used to find the correct pattern for each byte in the 3 byte sequence.
*/
void WS2812B::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
 {
   uint8_t *bptr = pixels + (n<<4) + n +n +1;  
   uint64_t encoded;
   
    if(brightness) 
	{ 
      r = (((uint16_t)r)*((uint16_t)brightness)) >> 8;
      g = (((uint16_t)g)*((uint16_t)brightness)) >> 8;
      b = (((uint16_t)b)*((uint16_t)brightness)) >> 8;
	}
   
   encoded = encode(g);
   for(int8_t i=5;i>=0;i--)
   {
	   *bptr++ = (uint8_t)(encoded >> (i<<3));
   }
   
   
   encoded = encode(r);
   for(int8_t i=5;i>=0;i--)
   {
	   *bptr++ = (uint8_t)(encoded >> (i<<3));
   }
   
   
	encoded = encode(b);
	for(int8_t i=5;i>=0;i--)
   {
	   *bptr++ = (uint8_t)(encoded >> (i<<3));
   }
 }

 
 uint64_t WS2812B::encode(uint8_t c)
 {
   uint64_t Encoding = 0;
   uint8_t Index=0;
   
    while (Index < 8)
    {
        Encoding = Encoding << 6;
        if (c & (1 << 7))
        {
            Encoding |= 0x3C;  // 0b111100 
        }
        else
        {
            Encoding |= 0x30; //0b110000  
        }
        c = c << 1;
        Index++; 
    }
		return Encoding;
 }
 
 
void WS2812B::setPixelColor(uint16_t n, uint32_t c)
  {
	  if( n > numLEDs -1)
		  return;
	  
     uint8_t r,g,b;
   
    if(brightness) 
	{ 
      r = ((int)((uint8_t)(c >> 16)) * (int)brightness) >> 8;
      g = ((int)((uint8_t)(c >>  8)) * (int)brightness) >> 8;
      b = ((int)((uint8_t)c) * (int)brightness) >> 8;
	}
	else
	{
      r = (uint8_t)(c >> 16),
      g = (uint8_t)(c >>  8),
	  b = (uint8_t)c;		
	}
	
	
  	
   uint8_t *bptr = pixels + (n<<4) + n + n +1;
   
   uint64_t encoded;
   
   
   encoded = encode(g);
   for(int8_t i=5;i>=0;i--)
   {
	   *bptr++ = (uint8_t)(encoded >> (i<<3));
   }
   
   
   encoded = encode(r);
   for(int8_t i=5;i>=0;i--)
   {
	   *bptr++ = (uint8_t)(encoded >> (i<<3));
   }
   
   
	encoded = encode(b);
	for(int8_t i=5;i>=0;i--)
   {
	   *bptr++ = (uint8_t)(encoded >> (i<<3));
   }
}

// Convert separate R,G,B into packed 32-bit RGB color.
// Packed format is always RGB, regardless of LED strand color order.
uint32_t WS2812B::Color(uint8_t r, uint8_t g, uint8_t b) 
{
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}


uint16_t WS2812B::numPixels(void) const 
{
  return numLEDs;
}

// Adjust output brightness; 0=darkest (off), 255=brightest.  This does
// NOT immediately affect what's currently displayed on the LEDs.  The
// next call to show() will refresh the LEDs at this level.  However,
// this process is potentially "lossy," especially when increasing
// brightness.  The tight timing in the WS2811/WS2812 code means there
// aren't enough free cycles to perform this scaling on the fly as data
// is issued.  So we make a pass through the existing color data in RAM
// and scale it (subsequent graphics commands also work at this
// brightness level).  If there's a significant step up in brightness,
// the limited number of steps (quantization) in the old data will be
// quite visible in the re-scaled version.  For a non-destructive
// change, you'll need to re-render the full strip data.  C'est la vie.
void WS2812B::setBrightness(uint8_t b) 
{
  // Stored brightness value is different than what's passed.
  // This simplifies the actual scaling math later, allowing a fast
  // 8x8-bit multiply and taking the MSB.  'brightness' is a uint8_t,
  // adding 1 here may (intentionally) roll over...so 0 = max brightness
  // (color values are interpreted literally; no scaling), 1 = min
  // brightness (off), 255 = just below max brightness.
  uint8_t newBrightness = b + 1;
  if(newBrightness != brightness) { // Compare against prior value
    // Brightness has changed -- re-scale existing data in RAM
    uint8_t  c,
            *ptr           = pixels,
             oldBrightness = brightness - 1; // De-wrap old brightness value
    uint16_t scale;
    if(oldBrightness == 0) scale = 0; // Avoid /0
    else if(b == 255) scale = 65535 / oldBrightness;
    else scale = (((uint16_t)newBrightness << 8) - 1) / oldBrightness;
    for(uint16_t i=0; i<numBytes; i++) 
	{
      c      = *ptr;
      *ptr++ = (c * scale) >> 8;
    }
    brightness = newBrightness;
  }
}

//Return the brightness value
uint8_t WS2812B::getBrightness(void) const
{
  return brightness - 1;
}

/*
*	Sets the encoded pixel data to turn all the LEDs off.
*/
void WS2812B::clear() 
{
	
	uint8_t * bptr= pixels+1;// Note first byte in the buffer is a preable and is always zero. hence the +1
	uint8_t *tPtr;
	
	for(int i=0;i< (numLEDs);i++)
	{
		setPixelColor(i, 0x00000000);
	}
	
}
