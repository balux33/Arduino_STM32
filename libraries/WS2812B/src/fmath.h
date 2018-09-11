#ifndef __INC_fmath_H
#define __INC_fmath_H

#include <stdint.h>

typedef uint8_t fract8;
typedef uint16_t fract16;

uint8_t scale8(uint8_t i, uint8_t sc);

//8bit saturated add
uint8_t qadd8(uint8_t i, uint8_t j);

uint8_t qsub8(uint8_t i, uint8_t j);

uint8_t qmul8(uint8_t i, uint8_t j);


void nscale8x3_video( uint8_t& r, uint8_t& g, uint8_t& b, uint8_t scale);

void nscale8x3( uint8_t& r, uint8_t& g, uint8_t& b, uint8_t scale);

uint8_t lerp8by8( uint8_t a, uint8_t b, uint8_t frac);

uint16_t lerp16by16( uint16_t a, uint16_t b, fract16 frac);

uint16_t scale16( uint16_t i, fract16 scale );

uint8_t scale8_video_LEAVING_R1_DIRTY( uint8_t i, fract8 scale);

uint8_t scale8_LEAVING_R1_DIRTY( uint8_t i, fract8 scale);

uint8_t sqrt16(uint16_t x);

uint8_t blend8( uint8_t a, uint8_t b, uint8_t amountOfB);

uint8_t scale8_video( uint8_t i, fract8 scale);


#endif