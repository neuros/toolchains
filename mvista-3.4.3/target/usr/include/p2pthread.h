/*****************************************************************************
 * p2pthread.h - declares the wrapper functions and data structures needed
 *               to implement a Wind River pSOS+ (R) kernel API 
 *               in a POSIX Threads environment.
 *  
 * Copyright (C) 2000, 2001  MontaVista Software Inc.
 *
 * Author : Gary S. Robertson
 *
 * pSOS and pSOS+ are registered trademarks of Wind River Systems, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 ****************************************************************************/

#include <pthread.h>

#if __cplusplus
extern "C" {
#endif

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE  !FALSE
#endif
#ifndef UCHAR
#define UCHAR  unsigned char
#endif
#ifndef USHORT
#define USHORT unsigned short
#endif
#ifndef UINT
#define UINT   unsigned int
#endif
#ifndef ULONG
#define ULONG  unsigned long
#endif

#define P2PT_TICK 10 /* milliseconds per p2pthread scheduler tick */

/*
**  Task Scheduling Priorities in p2pthread are higher as numbers increase...
**  define the largest and smallest possible priority numbers. (Priority
**  levels 0 and 240-255 are reserved for system use.)
*/
#define MIN_P2PT_PRIORITY 1 
#define MAX_P2PT_PRIORITY 239 

#define NUM_TASK_REGS 8

#define ERR_NO_ERROR  0x00     /* Normal return value */
/*****************************************************************************
**  Task suspend reasons
*****************************************************************************/
#define WAIT_READY 0
#define WAIT_TSTRT 1
#define WAIT_TSUSP 2
#define WAIT_DELAY 3
#define WAIT_ATIME 4
#define WAIT_MUTEX 5
#define WAIT_QUEUE 6
#define WAIT_VQUE  7
#define WAIT_SEMAP 8
#define WAIT_EVENT 9

/*****************************************************************************
**  Control block for pthread wrapper for p2pthread task
*****************************************************************************/
typedef struct p2pt_pthread_ctl_blk
{
        /*
        ** Task ID for task
        */
   ULONG 
        taskid;

        /*
        ** Task Name
        */
    char
        taskname[4];

        /*
        ** Thread ID for task
        */
    pthread_t
        pthrid;

        /*
        ** Thread attributes for task
        */
    pthread_attr_t
        attr;

        /*
        ** Previous scheduler priority for task
        */
    struct sched_param
        prv_priority;

        /*
        ** Execution entry point address for task
        */
    void (*entry_point)( ULONG, ULONG, ULONG, ULONG );

        /*
        ** 'Registers' for task
        */
    ULONG
        registers[NUM_TASK_REGS];

        /*
        ** Mutex and Condition variable for task events
        */
    pthread_mutex_t
        event_lock;
    pthread_cond_t
        event_change;

        /*
        ** Event state to awaken task (if suspended)
        */
    ULONG
        event_mask;

        /*
        ** Current state of captured event flags for task
        */
    ULONG
        events_captured;

        /*
        ** Current state of pending event flags for task
        */
    ULONG
        events_pending;

        /*
        ** Mode flags for task
        */
    ULONG
        flags;

        /*
        ** Reason why task is blocked
        */
    int
        suspend_reason;

        /*
        ** Pointer to suspended task list for object task is waiting on
        */
    struct p2pt_pthread_ctl_blk **
        suspend_list;

        /*
        ** Next task control block in list of tasks waiting on object
        */
    struct p2pt_pthread_ctl_blk *
        nxt_susp;

        /*
        ** Next task control block in list
        */
    struct p2pt_pthread_ctl_blk *
        nxt_task;
} p2pthread_cb_t;

/*****************************************************************************
**  Parameter block for pthread wrapper for p2pthread task
*****************************************************************************/
typedef struct p2pt_pthread_parms
{
        /*
        ** Pointer to task control block for thread
        */
    p2pthread_cb_t *
        tcb;

        /*
        ** Task code entry point address
        */
    void (*task_ptr)( ULONG, ULONG, ULONG, ULONG );

        /*
        ** Task parameter block address
        */
    ULONG 
         parms[4];

} p2pthread_pb_t;

#if __cplusplus
}
#endif

