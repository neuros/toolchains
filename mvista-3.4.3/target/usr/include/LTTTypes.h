/*
 * LTTTypes.h
 *
 * Copyright (C) 2000 Karim Yaghmour (karym@opersys.com).
 *
 * This is distributed under GPL.
 *
 * Header for LTT-secific types.
 *
 * History : 
 *    K.Y.  07/09/2001, Added David Schleef's architecture independent ltt_set_bit/ltt_clear_bit/ltt_test_bit
 *    JAL,  05/01/2001, Modified PPC bit manipulation functions for x86 compatibility.
 *                      (andy_lowe@mvista.com)
 *    K.Y., 31/05/2000, Initial typing.
 */

#ifndef __TRACE_TOOLKIT_TYPES_HEADER__
#define __TRACE_TOOLKIT_TYPES_HEADER__

#include <sys/types.h>
#include <sys/time.h>

#if defined(sun)

typedef unsigned char		u_int8_t;
typedef unsigned short		u_int16_t;
typedef unsigned int		u_int32_t;
#ifdef	_LP64
typedef unsigned long		u_int64_t;
#else	/* _ILP32 */
#if __STDC__ - 0 == 0 && !defined(_NO_LONGLONG)
typedef unsigned long long	u_int64_t;
#endif /* __STDC__ - 0 == 0 && !defined(_NO_LONGLONG) */
#endif /* _LP64 */

#define NEED_ALIGNMENT

#else

/* Some type corrections, just in case */
#ifndef uint8_t
#define uint8_t u_int8_t
#endif
#ifndef uint16_t
#define uint16_t u_int16_t
#endif
#ifndef uint32_t
#define uint32_t u_int32_t
#endif
#ifndef uint64_t
#define uint64_t u_int64_t
#endif

#endif /* defined(sun) */


extern __inline__ int ltt_set_bit(int nr, void * addr)
{
  unsigned char *p = (unsigned char*)addr;
  unsigned char mask = 1 << (nr&7);
  unsigned char old;

  p += nr>>3;
  old = *p;
  *p |= mask;
 
  return ((old & mask) != 0);
}

extern __inline__ int ltt_clear_bit(int nr, void * addr)
{
  unsigned char *p = (unsigned char*)addr;
  unsigned char mask = 1 << (nr&7);
  unsigned char old;

  p += nr>>3;
  old = *p;
  *p &= ~mask;
 
  return ((old & mask) != 0);
}

extern __inline__ int ltt_test_bit(int nr,void *addr)
{
  unsigned char *p = (unsigned char*)addr;
  unsigned char mask = 1 << (nr&7);
 
  p += nr>>3;
 
  return ((*p & mask) != 0);
}

/* 
 * Some platforms get bus error from unaligned accesses.  
 * Fix up the alignment while doing endian conversion.
 */

extern __inline__ uint16_t ltt_align16(char *src)
{
  char *   dst;
  uint16_t result;

  dst = (char *)&result;
  *dst++ = *src++;
  *dst++ = *src++;

  return result;
}

extern __inline__ uint32_t ltt_align32(char *src)
{
  char *   dst;
  uint32_t result;

  dst = (char *)&result;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;

  return result;
}

extern __inline__ uint64_t ltt_align64(char *src)
{
  char *   dst;
  uint64_t result;

  dst = (char *)&result;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;

  return result;
}

extern __inline__ uint16_t ltt_brev16(char *src)
{
  char *   dst;
  uint16_t result;

  dst = ((char *)&result) + 1;
  *dst-- = *src++;
  *dst-- = *src++;

  return result;
}

extern __inline__ uint32_t ltt_brev32(char *src)
{
  char *   dst;
  uint32_t result;

  dst = ((char *)&result) + 3;
  *dst-- = *src++;
  *dst-- = *src++;
  *dst-- = *src++;
  *dst-- = *src++;

  return result;
}

extern __inline__ uint64_t ltt_brev64(char *src)
{
  char *   dst;
  uint64_t result;

  dst = ((char *)&result) + 7;
  *dst-- = *src++;
  *dst-- = *src++;
  *dst-- = *src++;
  *dst-- = *src++;
  *dst-- = *src++;
  *dst-- = *src++;
  *dst-- = *src++;
  *dst-- = *src++;

  return result;
}

#if defined(NEED_ALIGNMENT)

#if TARGET_NATIVE

#define BREV8(x)    (x)
#define BREV16(x)   ltt_align16((char *)&(x))
#define BREV32(x)   ltt_align32((char *)&(x))
#define BREV64(x)   ltt_align64((char *)&(x))
#define RFT8(db,x)  (x)
#define RFT16(db,x) ltt_align16((char *)&(x))
#define RFT32(db,x) ltt_align32((char *)&(x))
#define RFT64(db,x) ltt_align64((char *)&(x))

/* Non-native development */
#else

#define BREV8(x)    (x)
#define BREV16(x)   ltt_brev16((char *)&(x))
#define BREV32(x)   ltt_brev32((char *)&(x))
#define BREV64(x)   ltt_brev64((char *)&(x))
#define RFT8(db,x)  (x)
#define RFT16(db,x) ((db)->ByteRev ? BREV16(x) : ltt_align16((char *)(&(x))))
#define RFT32(db,x) ((db)->ByteRev ? BREV32(x) : ltt_align32((char *)(&(x))))
#define RFT64(db,x) ((db)->ByteRev ? BREV64(x) : ltt_align64((char *)(&(x))))

#endif /* TRACE_TARGET_NATIVE */

#else

/* Big-endian/little-endian conversion macros for cross-development. */
#if TARGET_NATIVE
/* For native development, these conversion macros aren't needed. */
#define BREV16(x)   (x)
#define BREV32(x)   (x)
#define BREV64(x)   (x)
#define RFT8(db,x)  (x)
#define RFT16(db,x) (x)
#define RFT32(db,x) (x)
#define RFT64(db,x) (x)

/* Non-native development */
#else
        /* BREV16: byte-reverse a 16-bit integer */
#define BREV16(x) ((((x) & 0xff00) >> 8) | (((x) & 0x00ff) << 8))
	/* BREV32: byte-reverse a 32-bit integer */
#define BREV32(x) ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >> 8) \
		 | (((x) & 0x0000ff00) << 8) | (((x) & 0x000000ff) << 24))
	/* BREV64: byte-reverse a 64-bit integer */
#define BREV64(x) ((((x) & 0xff00000000000000ULL) >> 56) \
                 | (((x) & 0x00ff000000000000ULL) >> 40) \
		 | (((x) & 0x0000ff0000000000ULL) >> 24) \
		 | (((x) & 0x000000ff00000000ULL) >>  8) \
		 | (((x) & 0x00000000ff000000ULL) <<  8) \
		 | (((x) & 0x0000000000ff0000ULL) << 24) \
		 | (((x) & 0x000000000000ff00ULL) << 40) \
		 | (((x) & 0x00000000000000ffULL) << 56))
	/* RFTn: Read From Trace
	 *	Conditionally byte-reverse an 8-, 16-, 32-, or 64-bit integer
	 *      based on the value of the ByteRev member of the trace database
	 *      structure pointer passed as the first argument..
	 */
#define RFT8(db,x)  (x)
#define RFT16(db,x) ((db)->ByteRev ? BREV16(x) : (x))
#define RFT32(db,x) ((db)->ByteRev ? BREV32(x) : (x))
#define RFT64(db,x) ((db)->ByteRev ? BREV64(x) : (x))
#endif /* TRACE_TARGET_NATIVE */
#endif /* NEED_ALIGNMENT */

/* Structure packing */
#if LTT_UNPACKED_STRUCTS
#define LTT_PACKED_STRUCT
#else
#define LTT_PACKED_STRUCT __attribute__ ((packed))
#endif /* UNPACKED_STRUCTS */

/* Trace mask */
typedef uint64_t trace_event_mask;

/* Boolean stuff */
#define TRUE  1
#define FALSE 0

#endif /* __TRACE_TOOLKIT_TYPES_HEADER__ */
