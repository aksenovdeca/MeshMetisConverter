// ==========================================================
// Utility functions
//
// Design and implementation by
// - Floris van den Berg (flvdberg@wxs.nl)
// - Herve Drolon <drolon@infonie.fr>
// - Ryan Rubley (ryan@lostreality.org)
// - Mihail Naydenov (mnaydenov@users.sourceforge.net)
//
// This file is part of FreeImage 3
//
// COVERED CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY
// OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES
// THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE
// OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED
// CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT
// THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY
// SERVICING, REPAIR OR CORRECTION. THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL
// PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER
// THIS DISCLAIMER.
//
// Use at your own risk!
// ==========================================================

#ifndef FREEIMAGE_UTILITIES_H
#define FREEIMAGE_UTILITIES_H

// ==========================================================
//   Standard includes used by the library
// ==========================================================

#include <math.h>
#include <stdlib.h> 
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>
#include <float.h>
#include <limits.h>

#include <string>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <stack>
#include <sstream>
#include <algorithm>
#include <limits>
#include <memory>

// ==========================================================
//   Bitmap palette and pixels alignment
// ==========================================================

#define FIBITMAP_ALIGNMENT	16	// We will use a 16 bytes alignment boundary

// Memory allocation on a specified alignment boundary
// defined in BitmapAccess.cpp

void* FreeImage_Aligned_Malloc(size_t amount, size_t alignment);
void FreeImage_Aligned_Free(void* mem);



// ==========================================================
//   File I/O structs
// ==========================================================

// these structs are for file I/O and should not be confused with similar
// structs in FreeImage.h which are for in-memory bitmap handling

#ifdef _WIN32
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif // _WIN32

typedef struct tagFILE_RGBA {
  unsigned char r,g,b,a;
} FILE_RGBA;

typedef struct tagFILE_BGRA {
  unsigned char b,g,r,a;
} FILE_BGRA;

typedef struct tagFILE_RGB {
  unsigned char r,g,b;
} FILE_RGB;

typedef struct tagFILE_BGR {
  unsigned char b,g,r;
} FILE_BGR;

#ifdef _WIN32
#pragma pack(pop)
#else
#pragma pack()
#endif // _WIN32

// ==========================================================
//   Template utility functions
// ==========================================================

/// Max function
template <class T> T MAX(const T &a, const T &b) {
	return (a > b) ? a: b;
}

/// Min function
template <class T> T MIN(const T &a, const T &b) {
	return (a < b) ? a: b;
}

/// INPLACESWAP adopted from codeguru.com 
template <class T> void INPLACESWAP(T& a, T& b) {
	a ^= b; b ^= a; a ^= b;
}

/// Clamp function
template <class T> T CLAMP(const T &value, const T &min_value, const T &max_value) {
	return ((value < min_value) ? min_value : (value > max_value) ? max_value : value);
}

/** This procedure computes minimum min and maximum max
 of n numbers using only (3n/2) - 2 comparisons.
 min = L[i1] and max = L[i2].
 ref: Aho A.V., Hopcroft J.E., Ullman J.D., 
 The design and analysis of computer algorithms, 
 Addison-Wesley, Reading, 1974.
*/
template <class T> void 
MAXMIN(const T* L, long n, T& max, T& min) {
	long i1, i2, i, j;
	T x1, x2;
	long k1, k2;

	i1 = 0; i2 = 0; min = L[0]; max = L[0]; j = 0;
	if((n % 2) != 0)  j = 1;
	for(i = j; i < n; i+= 2) {
		k1 = i; k2 = i+1;
		x1 = L[k1]; x2 = L[k2];
		if(x1 > x2)	{
			k1 = k2;  k2 = i;
			x1 = x2;  x2 = L[k2];
		}
		if(x1 < min) {
			min = x1;  i1 = k1;
		}
		if(x2 > max) {
			max = x2;  i2 = k2;
		}
	}
}

// ==========================================================
//   Utility functions
// ==========================================================

#ifndef _WIN32
inline char*
i2a(unsigned i, char *a, unsigned r) {
	if (i/r > 0) a = i2a(i/r,a,r);
	*a = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[i%r];
	return a+1;
}

/** 
 Transforms integer i into an ascii string and stores the result in a; 
 string is encoded in the base indicated by r.
 @param i Number to be converted
 @param a String result
 @param r Base of value; must be in the range 2 - 36
 @return Returns a
*/
inline char *
_itoa(int i, char *a, int r) {
	r = ((r < 2) || (r > 36)) ? 10 : r;
	if(i < 0) {
		*a = '-';
		*i2a(-i, a+1, r) = 0;
	}
	else *i2a(i, a, r) = 0;
	return a;
}

#endif // !_WIN32

inline unsigned char
HINIBBLE (unsigned char byte) {
	return byte & 0xF0;
}

inline unsigned char
LOWNIBBLE (unsigned char byte) {
	return byte & 0x0F;
}

inline int
CalculateUsedBits(int bits) {
	int bit_count = 0;
	unsigned bit = 1;

	for (unsigned i = 0; i < 32; i++) {
		if ((bits & bit) == bit) {
			bit_count++;
		}

		bit <<= 1;
	}

	return bit_count;
}

inline unsigned
CalculateLine(const unsigned width, const unsigned bitdepth) {
	return (unsigned)( ((unsigned long long)width * bitdepth + 7) / 8 );
}

inline unsigned
CalculatePitch(const unsigned line) {
	return (line + 3) & ~3;
}

inline unsigned
CalculateUsedPaletteEntries(const unsigned bit_count) {
	if ((bit_count >= 1) && (bit_count <= 8)) {
		return 1 << bit_count;
	}

	return 0;
}

inline uint8_t*
CalculateScanLine(uint8_t *bits, const unsigned pitch, const int scanline) {
	return bits ? (bits + ((size_t)pitch * scanline)) : NULL;
}

// ----------------------------------------------------------

/**
Fast generic assign (faster than for loop)
@param dst Destination pixel
@param src Source pixel
@param bytesperpixel # of bytes per pixel
*/
inline void 
AssignPixel(uint8_t* dst, const uint8_t* src, unsigned bytesperpixel) {
	switch (bytesperpixel) {
		case 1:	// FIT_BITMAP (8-bit)
			*dst = *src;
			break;

		case 2: // FIT_UINT16 / FIT_INT16 / 16-bit
			*(reinterpret_cast<uint16_t*>(dst)) = *(reinterpret_cast<const uint16_t*> (src));
			break;

		case 3: // FIT_BITMAP (24-bit)
			*(reinterpret_cast<uint16_t*>(dst)) = *(reinterpret_cast<const uint16_t*> (src));
			dst[2] = src[2];
			break;

		case 4: // FIT_BITMAP (32-bit) / FIT_UINT32 / FIT_INT32 / FIT_FLOAT
			*(reinterpret_cast<uint32_t*>(dst)) = *(reinterpret_cast<const uint32_t*> (src));
			break;

		case 6: // FIT_RGB16 (3 x 16-bit)
			*(reinterpret_cast<uint32_t*>(dst)) = *(reinterpret_cast<const uint32_t*> (src));
			*(reinterpret_cast<uint16_t*>(dst + 4)) = *(reinterpret_cast<const uint16_t*> (src + 4));	
			break;

		// the rest can be speeded up with int64
			
		case 8: // FIT_RGBA16 (4 x 16-bit)
			*(reinterpret_cast<uint32_t*>(dst)) = *(reinterpret_cast<const uint32_t*> (src));
			*(reinterpret_cast<uint32_t*>(dst + 4)) = *(reinterpret_cast<const uint32_t*> (src + 4));	
			break;
		
		case 12: // FIT_RGBF (3 x 32-bit IEEE floating point)
			*(reinterpret_cast<float*>(dst)) = *(reinterpret_cast<const float*> (src));
			*(reinterpret_cast<float*>(dst + 4)) = *(reinterpret_cast<const float*> (src + 4));
			*(reinterpret_cast<float*>(dst + 8)) = *(reinterpret_cast<const float*> (src + 8));
			break;
		
		case 16: // FIT_RGBAF (4 x 32-bit IEEE floating point)
			*(reinterpret_cast<float*>(dst)) = *(reinterpret_cast<const float*> (src));
			*(reinterpret_cast<float*>(dst + 4)) = *(reinterpret_cast<const float*> (src + 4));
			*(reinterpret_cast<float*>(dst + 8)) = *(reinterpret_cast<const float*> (src + 8));
			*(reinterpret_cast<float*>(dst + 12)) = *(reinterpret_cast<const float*> (src + 12));
			break;
			
		default:
			assert(FALSE);
	}
}


// ==========================================================
//   Big Endian / Little Endian utility functions
// ==========================================================

inline uint16_t 
__SwapUInt16(uint16_t arg) { 
#if defined(_MSC_VER) && _MSC_VER >= 1310 
	return _byteswap_ushort(arg); 
#elif defined(__i386__) && defined(__GNUC__) 
	__asm__("xchgb %b0, %h0" : "+q" (arg)); 
	return arg; 
#elif defined(__ppc__) && defined(__GNUC__) 
	uint16_t result; 
	__asm__("lhbrx %0,0,%1" : "=r" (result) : "r" (&arg), "m" (arg)); 
	return result; 
#else 
	// swap bytes 
	uint16_t result;
	result = ((arg << 8) & 0xFF00) | ((arg >> 8) & 0x00FF); 
	return result; 
#endif 
} 
 
inline uint32_t 
__SwapUInt32(uint32_t arg) { 
#if defined(_MSC_VER) && _MSC_VER >= 1310 
	return _byteswap_ulong(arg); 
#elif defined(__i386__) && defined(__GNUC__) 
	__asm__("bswap %0" : "+r" (arg)); 
	return arg; 
#elif defined(__ppc__) && defined(__GNUC__) 
	uint32_t result; 
	__asm__("lwbrx %0,0,%1" : "=r" (result) : "r" (&arg), "m" (arg)); 
	return result; 
#else 
	// swap words then bytes
	uint32_t result; 
	result = ((arg & 0x000000FF) << 24) | ((arg & 0x0000FF00) << 8) | ((arg >> 8) & 0x0000FF00) | ((arg >> 24) & 0x000000FF); 
	return result; 
#endif 
} 

inline void
SwapShort(uint16_t *sp) {
	*sp = __SwapUInt16(*sp);
}

inline void
SwapLong(uint32_t *lp) {
	*lp = __SwapUInt32(*lp);
}
 
inline void
SwapInt64(uint64_t *arg) {
#if defined(_MSC_VER) && _MSC_VER >= 1310
	*arg = _byteswap_uint64(*arg);
#else
	union Swap {
		uint64_t sv;
		uint32_t ul[2];
	} tmp, result;
	tmp.sv = *arg;
	SwapLong(&tmp.ul[0]);
	SwapLong(&tmp.ul[1]);
	result.ul[0] = tmp.ul[1];
	result.ul[1] = tmp.ul[0];
	*arg = result.sv;
#endif
}

// ==========================================================
//   Greyscale and color conversion
// ==========================================================

/**
Extract the luminance channel L from a RGBF image. 
Luminance is calculated from the sRGB model using a D65 white point, using the Rec.709 formula : 
L = ( 0.2126 * r ) + ( 0.7152 * g ) + ( 0.0722 * b )
Reference : 
A Standard Default Color Space for the Internet - sRGB. 
[online] http://www.w3.org/Graphics/Color/sRGB
*/
#define LUMA_REC709(r, g, b)	(0.2126F * r + 0.7152F * g + 0.0722F * b)

#define GREY(r, g, b) (uint8_t)(LUMA_REC709(r, g, b) + 0.5F)
/*
#define GREY(r, g, b) (uint8_t)(((uint16_t)r * 77 + (uint16_t)g * 150 + (uint16_t)b * 29) >> 8)	// .299R + .587G + .114B
*/
/*
#define GREY(r, g, b) (uint8_t)(((uint16_t)r * 169 + (uint16_t)g * 256 + (uint16_t)b * 87) >> 9)	// .33R + 0.5G + .17B
*/

/**
Convert a RGB 24-bit value to a 16-bit 565 value
*/
#define RGB565(b, g, r) ((((b) >> 3) << FI16_565_BLUE_SHIFT) | (((g) >> 2) << FI16_565_GREEN_SHIFT) | (((r) >> 3) << FI16_565_RED_SHIFT))

/**
Convert a RGB 24-bit value to a 16-bit 555 value
*/
#define RGB555(b, g, r) ((((b) >> 3) << FI16_555_BLUE_SHIFT) | (((g) >> 3) << FI16_555_GREEN_SHIFT) | (((r) >> 3) << FI16_555_RED_SHIFT))

/**
Returns TRUE if the format of a dib is RGB565
*/
#define IS_FORMAT_RGB565(dib) ((FreeImage_GetRedMask(dib) == FI16_565_RED_MASK) && (FreeImage_GetGreenMask(dib) == FI16_565_GREEN_MASK) && (FreeImage_GetBlueMask(dib) == FI16_565_BLUE_MASK))

/**
Convert a RGB565 or RGB555 FIRGBA8 pixel to a uint16_t
*/
#define RGBQUAD_TO_WORD(dib, color) (IS_FORMAT_RGB565(dib) ? RGB565((color)->blue, (color)->green, (color)->red) : RGB555((color)->blue, (color)->green, (color)->red))

/**
Create a greyscale palette
*/
#define CREATE_GREYSCALE_PALETTE(palette, entries) \
	for (unsigned i = 0, v = 0; i < entries; i++, v += 0x00FFFFFF / (entries - 1)) { \
		((unsigned *)palette)[i] = v; \
	}

/**
Create a reverse greyscale palette
*/
#define CREATE_GREYSCALE_PALETTE_REVERSE(palette, entries) \
	for (unsigned i = 0, v = 0x00FFFFFF; i < entries; i++, v -= (0x00FFFFFF / (entries - 1))) { \
		((unsigned *)palette)[i] = v; \
	}

// ==========================================================
//   Generic error messages
// ==========================================================

static const char *FI_MSG_ERROR_MEMORY = "Memory allocation failed";
static const char *FI_MSG_ERROR_DIB_MEMORY = "DIB allocation failed, maybe caused by an invalid image size or by a lack of memory";
static const char *FI_MSG_ERROR_PARSING = "Parsing error";
static const char *FI_MSG_ERROR_MAGIC_NUMBER = "Invalid magic number";
static const char *FI_MSG_ERROR_UNSUPPORTED_FORMAT = "Unsupported format";
static const char *FI_MSG_ERROR_UNSUPPORTED_COMPRESSION = "Unsupported compression type";
static const char *FI_MSG_WARNING_INVALID_THUMBNAIL = "Warning: attached thumbnail cannot be written to output file (invalid format) - Thumbnail saving aborted";

#endif // FREEIMAGE_UTILITIES_H
