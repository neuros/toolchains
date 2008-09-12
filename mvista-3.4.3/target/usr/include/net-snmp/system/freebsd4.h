#include "freebsd.h"

/*
 * freebsd4 is a superset of freebsd2 and freebsd3 
 */
#define freebsd2 1
#define freebsd3 1

#undef IFADDR_SYMBOL
#define IFADDR_SYMBOL "in_ifaddrhead"

#undef PROC_SYMBOL
#define PROC_SYMBOL "allproc"

#undef NPROC_SYMBOL
#define NPROC_SYMBOL "nprocs"

#undef TOTAL_MEMORY_SYMBOL
