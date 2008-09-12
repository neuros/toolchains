#include "sysv.h"

#undef bsdlike
#undef IP_FORWARDING_SYMBOL
#undef ARPTAB_SYMBOL
#define ARPTAB_SYMBOL "arptab_nb"
#undef ARPTAB_SIZE_SYMBOL
#define ARPTAB_SIZE_SYMBOL "arphd"
#undef ICMPSTAT_SYMBOL
#undef TCPSTAT_SYMBOL
#undef TCP_SYMBOL
#undef UDPSTAT_SYMBOL
#undef UDB_SYMBOL
#undef RTTABLES_SYMBOL
#undef RTHASHSIZE_SYMBOL
#undef RTHOST_SYMBOL
#undef RTNET_SYMBOL
#undef IPSTAT_SYMBOL
#undef TCP_TTL_SYMBOL
#undef PROC_SYMBOL
#undef TOTAL_MEMORY_SYMBOL
#undef MBSTAT_SYMBOL

/*
 * solaris cc doesn't like our use of inline. Until we figure out why,
 * turn of inline.
 */
#ifndef __GNUC__
#  define NETSNMP_NO_INLINE 1
#endif
