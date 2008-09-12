/*
 * DevCommand.h
 *
 * Copyright (C) 2001 Karim Yaghmour.
 *
 * This is distributed under GPL.
 *
 * Header for trace device commands.
 *
 * History : 
 *    K.Y., 03/12/2001, Initial typing.
 *
 */

#ifndef __TRACE_DEV_COMMAND_HEADER__
#define __TRACE_DEV_COMMAND_HEADER__

#include <LinuxEvents.h>

/* The configurations possible */
enum {
	TRACER_START = TRACER_MAGIC_NUMBER,	/* Start tracing events using the current configuration */
	TRACER_STOP,				/* Stop tracing */
	TRACER_CONFIG_DEFAULT,			/* Set the tracer to the default configuration */
	TRACER_CONFIG_MEMORY_BUFFERS,		/* Set the memory buffers the daemon wants us to use */
	TRACER_CONFIG_EVENTS,			/* Trace the given events */
	TRACER_CONFIG_DETAILS,			/* Record the details of the event, or not */
	TRACER_CONFIG_CPUID,			/* Record the CPUID associated with the event */
	TRACER_CONFIG_PID,			/* Trace only one process */
	TRACER_CONFIG_PGRP,			/* Trace only the given process group */
	TRACER_CONFIG_GID,			/* Trace the processes of a given group of users */
	TRACER_CONFIG_UID,			/* Trace the processes of a given user */
	TRACER_CONFIG_SYSCALL_EIP_DEPTH,	/* Set the call depth at which the EIP should be fetched on syscall */
	TRACER_CONFIG_SYSCALL_EIP_LOWER,	/* Set the lowerbound address from which EIP is recorded on syscall */
	TRACER_CONFIG_SYSCALL_EIP_UPPER,	/* Set the upperbound address from which EIP is recorded on syscall */
	TRACER_DATA_COMITTED,			/* The daemon has comitted the last trace */
	TRACER_GET_EVENTS_LOST,			/* Get the number of events lost */
	TRACER_CREATE_USER_EVENT,		/* Create a user tracable event */
	TRACER_DESTROY_USER_EVENT,		/* Destroy a user tracable event */
	TRACER_TRACE_USER_EVENT,		/* Trace a user event */
	TRACER_SET_EVENT_MASK,			/* Set the trace event mask */
	TRACER_GET_EVENT_MASK,			/* Get the trace event mask */
	TRACER_GET_BUFFER_CONTROL,		/* Get the buffer control data for the lockless schem*/
	TRACER_CONFIG_N_MEMORY_BUFFERS,		/* Set the number of memory buffers the daemon wants us to use */
	TRACER_CONFIG_USE_LOCKING,		/* Set the locking scheme to use */
	TRACER_CONFIG_TIMESTAMP,		/* Set the timestamping method to use */
	TRACER_GET_ARCH_INFO,			/* Get information about the CPU configuration */
	TRACER_ALLOC_HANDLE,			/* Allocate a tracer handle */
	TRACER_FREE_HANDLE,			/* Free a single handle */
	TRACER_FREE_DAEMON_HANDLE,		/* Free the daemon's handle */
	TRACER_FREE_ALL_HANDLES,		/* Free all handles */
	TRACER_MAP_BUFFER,			/* Map buffer to process-space */
	TRACER_PAUSE,				/* Pause tracing */
	TRACER_UNPAUSE,				/* Unpause tracing */
	TRACER_GET_START_INFO,			/* trace start data */
	TRACER_GET_STATUS			/* status of traces */
};

/* Structures for user commands */
typedef trace_new_event tracer_create_user_event;
typedef trace_custom    tracer_trace_user_event;

/* In the ltt root directory lives the trace control file, used for
   kernel-user communication.  */
#define TRACE_RELAYFS_ROOT "ltt"  /* /relayfs/mount/ltt/handle/cpu0-cpuX */
#define TRACE_CONTROL_FILE "control" /* /relayfs/mount/ltt/control */

extern int get_relayfs_mntpt(char *pmMountPoint);

#endif  /* __TRACE_DEV_COMMAND_HEADER__ */
