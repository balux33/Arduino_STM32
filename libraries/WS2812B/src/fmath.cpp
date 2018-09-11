#include "fmath.h"

uint8_t scale8(uint8_t i, uint8_t sc)
{
	return (uint8_t)(((uint16_t)i*sc)>>8);
}

//8bit saturated add
uint8_t qadd8(uint8_t i, uint8_t j)
{
	uint16_t sum = i + j;
	if(sum > 255)
		return 255;
	else
		return sum;
}



uint8_t qsub8(uint8_t i, uint8_t j)
{
	if(j > i)
		return 0;
	else
		return i - j;
}

uint8_t qmul8(uint8_t i, uint8_t j)
{
	uint16_t mul = i * j;
	if(mul > 255)
		return 255;
	else
		return mul;
}

void nscale8x3_video( uint8_t& r, uint8_t& g, uint8_t& b, uint8_t scale)
{
    uint8_t nonzeroscale = (scale != 0) ? 1 : 0;
    r = (r == 0) ? 0 : (((int)r * (int)(scale) ) >> 8) + nonzeroscale;
    g = (g == 0) ? 0 : (((int)g * (int)(scale) ) >> 8) + nonzeroscale;
    b = (b == 0) ? 0 : (((int)b * (int)(scale) ) >> 8) + nonzeroscale;

}

uint16_t scale16( uint16_t i, fract16 scale )
{
    uint16_t result;
    result = ((uint32_t)(i) * (uint32_t)(scale)) / 65536;
    return result;
}

void nscale8x3( uint8_t& r, uint8_t& g, uint8_t& b, uint8_t scale)
{
    r = ((int)r * (int)(scale) ) >> 8;
    g = ((int)g * (int)(scale) ) >> 8;
    b = ((int)b * (int)(scale) ) >> 8;
}

/// linear interpolation between two unsigned 8-bit values,
/// with 8-bit fraction
uint8_t lerp8by8( uint8_t a, uint8_t b, uint8_t frac)
{
    uint8_t result;
    if( b > a) {
        uint8_t delta = b - a;
        uint8_t scaled = scale8( delta, frac);
        result = a + scaled;
    } else {
        uint8_t delta = a - b;
        uint8_t scaled = scale8( delta, frac);
        result = a - scaled;
    }
    return result;
}

/// linear interpolation between two unsigned 16-bit values,
/// with 16-bit fraction
 uint16_t lerp16by16( uint16_t a, uint16_t b, fract16 frac)
{
    uint16_t result;
    if( b > a ) {
        uint16_t delta = b - a;
        uint32_t scaled = scale16(delta, frac);
        result = a + scaled;
    } else {
        uint16_t delta = a - b;
        uint16_t scaled = scale16( delta, frac);
        result = a - scaled;
    }
    return result;
}


uint8_t scale8_video_LEAVING_R1_DIRTY( uint8_t i, fract8 scale)
{
    uint8_t j = (((int)i * (int)scale) >> 8) + ((i&&scale)?1:0);
    return j;
}

uint8_t scale8_LEAVING_R1_DIRTY( uint8_t i, fract8 scale)
{
    return ((int)i * (int)(scale) ) >> 8;
}

uint8_t sqrt16(uint16_t x)
{
    if( x <= 1) {
        return x;
    }

    uint8_t low = 1; // lower bound
    uint8_t hi, mid;

    if( x > 7904) {
        hi = 255;
    } else {
        hi = (x >> 5) + 8; // initial estimate for upper bound
    }

    do {
        mid = (low + hi) >> 1;
        if ((uint16_t)(mid * mid) > x) {
            hi = mid - 1;
        } else {
            if( mid == 255) {
                return 255;
            }
            low = mid + 1;
        }
    } while (hi >= low);

    return low - 1;
}

uint8_t blend8( uint8_t a, uint8_t b, uint8_t amountOfB)
{
    uint16_t partial;
    uint8_t result;
    
    uint8_t amountOfA = 255 - amountOfB;
    
    partial = (a * amountOfA);

    partial += (b * amountOfB);
    
    result = partial >> 8;
    
    return result;
}


uint8_t scale8_video( uint8_t i, fract8 scale)
{
    uint8_t j = (((int)i * (int)scale) >> 8) + ((i&&scale)?1:0);
    return j;
}