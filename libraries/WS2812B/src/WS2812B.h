/*--------------------------------------------------------------------
  The WS2812B library is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  It is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  See <http://www.gnu.org/licenses/>.
  --------------------------------------------------------------------*/

#ifndef WS2812B_H
#define WS2812B_H

#include <Arduino.h>
#include "pixeltypes.h"
#include "colorutils.h"
#include "colorpalettes.h"

static uint8_t encoded_black[6]  = {0xC3,0x0C,0x30,0xC3,0x0C,0x30};		
									
class WS2812B {
public:

  // Constructor: number of LEDs
  WS2812B (uint16_t number_of_leds);// Constuctor 
    ~WS2812B();
  
  void begin(void); 
  void begin(CRGB * CRGB_buffer);
  void setBrightness(uint8_t); //brightness 0-255  0 min 255 max  default max
  void clear(); //clear leds to black
  void updateLength(uint16_t n);
  uint8_t getBrightness(void) const;
  uint16_t numPixels(void) const;
  inline bool canShow(void) { return (micros() - endTime) >= 300L; }
  
  //new FastLED style api
  void set_actual_buffer(CRGB * CRGB_buffer) { actual_CRGB_buffer = CRGB_buffer; }
  void show();
  void show(CRGB * CRGB_buffer);
   
  
private:
    //old style api only internal use
  void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
  void setPixelColor(uint16_t n, uint32_t c);
  static uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
  void send_data_to_leds(void);  
  
  
  uint64_t encode(uint8_t c);
  boolean begun;             // true if begin() previously called
  uint16_t numLEDs;          // Number of RGB LEDs in strip
  uint16_t numBytes;         // Size of 'pixels' buffer
	
  uint8_t brightness;
  uint8_t *pixels;          // Holds the current LED color values, which the external API calls interact with 9 bytes per pixel + start + end empty bytes
  uint8_t *doubleBuffer;	// Holds the start of the double buffer (1 buffer for async DMA transfer and one for the API interaction.
  CRGB    *actual_CRGB_buffer = NULL;
  
  uint32_t endTime;         // Latch timing reference
	
};




#endif // WS2812B_H
