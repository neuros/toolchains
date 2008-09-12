/*
 * UserTrace.h
 *
 * Copyright (C) 2001 Karim Yaghmour (karym@opersys.com).
 *
 * This header is distributed under v2.1 of LGPL.
 *
 * User event tracing libary header.
 *
 * History : 
 *    K.Y., 01/12/2001, Initial typing.
 *
 */

#include <LTTTypes.h>

#ifndef PATH_MAX
#	include <sys/param.h>
#	ifndef MAXPATHLEN
#		define PATH_MAX 4096
#	else
#		define PATH_MAX MAXPATHLEN
#	endif
#endif

struct control_data
{
	unsigned int tracer_handle;
	unsigned long command_arg;
} LTT_PACKED_STRUCT;

int trace_attach
       (void);
int trace_detach
       (void);
int trace_create_event
       (char*         /* String describing event type */,
	char*         /* String to format standard event description */,
	int           /* Type of formatting used to log event data */,
	char*         /* Data specific to format */);
int trace_destroy_event
       (int           /* The event ID given by trace_create_event() */);
int trace_user_event
       (int           /* The event ID given by trace_create_event() */,
	int           /* The size of the raw data */,
	void*         /* Pointer to the raw event data */);
int trace_set_event_mask
       (trace_event_mask   /* The event mask to be set */);
int trace_get_event_mask
       (trace_event_mask*  /* Pointer to variable where to set event mask retrieved */);
int trace_enable_event_trace
       (int          /* Event ID who's tracing is to be enabled */);
int trace_disable_event_trace
       (int          /* Event ID who's tracing is to be disabled */);
int trace_is_event_traced
       (int          /* Event ID to be checked for tracing */);
