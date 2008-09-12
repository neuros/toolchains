/* 
 * If linux/types.h is already been included, assume it has defined
 * everything we need.  (cross fingers)  Other header files may have 
 * also defined the types that we need.
 */
#if (!defined(_LINUX_TYPES_H) && !defined(_BLKID_TYPES_H) && \
	!defined(_EXT2_TYPES_H))
#define _EXT2_TYPES_H

typedef unsigned char __u8;
typedef signed char __s8;

#if (4 == 8)
typedef int		__s64;
typedef unsigned int	__u64;
#else
#if (4 == 8)
typedef long		__s64;
typedef unsigned long	__u64;
#else
#if (8 == 8)
#if defined(__GNUC__)
typedef __signed__ long long 	__s64;
#else
typedef signed long long 	__s64;
#endif /* __GNUC__ */
typedef unsigned long long	__u64;
#endif /* SIZEOF_LONG_LONG == 8 */
#endif /* SIZEOF_LONG == 8 */
#endif /* SIZEOF_INT == 8 */

#if (4 == 2)
typedef	int		__s16;
typedef	unsigned int	__u16;
#else
#if (2 == 2)
typedef	short		__s16;
typedef	unsigned short	__u16;
#else
  ?==error: undefined 16 bit type
#endif /* SIZEOF_SHORT == 2 */
#endif /* SIZEOF_INT == 2 */

#if (4 == 4)
typedef	int		__s32;
typedef	unsigned int	__u32;
#else
#if (4 == 4)
typedef	long		__s32;
typedef	unsigned long	__u32;
#else
#if (2 == 4)
typedef	short		__s32;
typedef	unsigned short	__u32;
#else
 ?== error: undefined 32 bit type
#endif /* SIZEOF_SHORT == 4 */
#endif /* SIZEOF_LONG == 4 */
#endif /* SIZEOF_INT == 4 */

#endif /* _*_TYPES_H */
