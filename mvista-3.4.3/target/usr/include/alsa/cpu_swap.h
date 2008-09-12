#ifndef CPU_SWAP_H
#define CPU_SWAP_H

#include <endian.h>
#include <byteswap.h>

#define cpu_to_le32(x) (BYTE_ORDER==LITTLE_ENDIAN) ? (x) : (bswap_32(x))

#define cpu_to_be32(x) (BYTE_ORDER==BIG_ENDIAN) ? (x) : (bswap_32(x))


#define cpu_to_le16(x) (BYTE_ORDER==LITTLE_ENDIAN) ? (x) : (bswap_16(x))

#define cpu_to_be16(x) (BYTE_ORDER==BIG_ENDIAN) ? (x) : (bswap_16(x))
		


#define le32_to_cpu(x) (BYTE_ORDER==LITTLE_ENDIAN) ? (x) : (bswap_32(x))

#define be32_to_cpu(x) (BYTE_ORDER==BIG_ENDIAN) ? (x) : (bswap_32(x))


#define le16_to_cpu(x) (BYTE_ORDER==LITTLE_ENDIAN) ? (x) : (bswap_16(x))

#define be16_to_cpu(x) (BYTE_ORDER==BIG_ENDIAN) ? (x) : (bswap_16(x))

#endif
