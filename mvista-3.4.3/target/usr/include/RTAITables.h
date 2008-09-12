/*
 * RTAITables.h
 *
 * Copyright (C) 2000 Karim Yaghmour (karym@opersys.com).
 *
 * This is distributed under GPL.
 *
 * Tables for data for RTAI.
 *
 * History : 
 *    K.Y., 31/05/2000, Initial typing.
 */

#ifndef __TRACE_TOOLKIT_RTAI_TABLES__
#define __TRACE_TOOLKIT_RTAI_TABLES__

#include <EventDB.h>

/* Event structure sizes */
extern int RTAIEventStructSize
             (db*     /* Event database */,
	      int     /* Event ID */);

/* Event description strings */
#if 0
extern char *RTAIEventID[];
#else
char* RTAIEventString
        (db*    /* Event database */,
	 int    /* Event ID */,
	 event* /* Event */);
#endif

/* Event strings the user can specify at the command line to omit or trace */
extern char *RTAIEventOT[];

#endif /*  __TRACE_TOOLKIT_RTAI_TABLES__ */
