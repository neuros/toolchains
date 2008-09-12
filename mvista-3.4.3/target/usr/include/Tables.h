/*
 * Tables.h
 *
 * Copyright (C) 1999 Karim Yaghmour.
 *
 * This is distributed under GPL.
 *
 * Tables for data for the trace toolkit.
 *
 * History : 
 *    K.Y., 29/10/1999, Initial typing.
 */

#ifndef __TRACE_TOOLKIT_TABLES__
#define __TRACE_TOOLKIT_TABLES__

#include <EventDB.h>

/* Number of events defined */
extern int MaxEventID;

/* Event structure sizes */
extern int*   EventStructSize[];
extern int*   MVEventStructSize[];

#define EVENT_STRUCT_SIZE(X, EVENT_SIZE_TABLE, MV_EVENT_SIZE_TABLE)	\
	(((X) >= TRACE_MVISTA_MIN)                    ?			\
	 ((MV_EVENT_SIZE_TABLE)[X-TRACE_MVISTA_MIN]) :			\
	 ((EVENT_SIZE_TABLE)[X]))

/* Event description strings */
extern char* (*EventString)
               (db*    /* Event database */,
                int    /* Event ID */,
                event* /* Event */);
#define EVENT_STR(DB, ID, EVENT)	(*EventString)(DB, ID, EVENT)

/* Event strings the user can specify at the command line to omit or trace */
extern char** EventOT;
extern char** MVEventOT;
#define EVENT_OT(X)  					\
	(((X) >= TRACE_MVISTA_MIN)                  ? 	\
	((char*) (MVEventOT[X-TRACE_MVISTA_MIN]))   : 	\
	((char*) (EventOT[X])))

/* System call name according to it's ID */
extern char* (*SyscallString)
               (db*   /* Event database */,
		int   /* Syscall ID */);
#define SYSCALL_STR(DB, ID) (*SyscallString)(DB, ID)

/* Trap according to it's ID */
extern char* (*TrapString)
               (db*      /* Event database */,
		uint64_t /* Syscall ID */);
#define TRAP_STR(DB, ID) (*TrapString)(DB, ID)

#endif /* __TRACE_TOOLKIT_TABLES__ */
