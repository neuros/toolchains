/*
 * linux/include/linux/ltt-events.h
 *
 * Copyright (C) 1999-2004 Karim Yaghmour (karim@opersys.com)
 * Copyright (C) 2004, 2005 - MontaVista Software, Inc. (source@mvista.com)
 *
 * This contains the event definitions for the Linux Trace Toolkit.
 *
 * This file is released  under the terms of the GNU GPL version 2.
 * This program  is licensed "as is" without any warranty of any kind,
 * whether express or implied.
 */

#ifndef _LINUX_TRACE_H
#define _LINUX_TRACE_H

#include <linux/ltt-core.h>
#include <linux/sched.h>

/* Is kernel tracing enabled */
#if defined(CONFIG_LTT)

extern unsigned int ltt_syscall_entry_trace_active;
extern unsigned int ltt_syscall_exit_trace_active;

static inline void ltt_ev(u8 event_id, void* data)
{
	ltt_log_event(event_id, data);
}

/* Traced events */
enum {
	LTT_EV_START = 0,	/* This is to mark the trace's start */
	LTT_EV_SYSCALL_ENTRY,	/* Entry in a given system call */
	LTT_EV_SYSCALL_EXIT,	/* Exit from a given system call */
	LTT_EV_TRAP_ENTRY,	/* Entry in a trap */
	LTT_EV_TRAP_EXIT,	/* Exit from a trap */
	LTT_EV_IRQ_ENTRY,	/* Entry in an irq */
	LTT_EV_IRQ_EXIT,	/* Exit from an irq */
	LTT_EV_SCHEDCHANGE,	/* Scheduling change */
	LTT_EV_KERNEL_TIMER,	/* The kernel timer routine has been called */
	LTT_EV_SOFT_IRQ,	/* Hit key part of soft-irq management */
	LTT_EV_PROCESS,		/* Hit key part of process management */
	LTT_EV_FILE_SYSTEM,	/* Hit key part of file system */
	LTT_EV_TIMER,		/* Hit key part of timer management */
	LTT_EV_MEMORY,		/* Hit key part of memory management */
	LTT_EV_SOCKET,		/* Hit key part of socket communication */
	LTT_EV_IPC,		/* Hit key part of System V IPC */
	LTT_EV_NETWORK,		/* Hit key part of network communication */
	LTT_EV_BUFFER_START,	/* Mark the begining of a trace buffer */
	LTT_EV_BUFFER_END,	/* Mark the ending of a trace buffer */
	LTT_EV_NEW_EVENT,	/* New event type */
	LTT_EV_CUSTOM,		/* Custom event */
	LTT_EV_CHANGE_MASK,	/* Change in event mask */
	LTT_EV_HEARTBEAT	/* Heartbeat event */
};

/* Number of traced events */
#define LTT_EV_MAX		LTT_EV_HEARTBEAT

/* Begin at the high bits of the ltt_event_mask and go downward */
#define LTT_MVISTA_EV_BASE	(sizeof(ltt_event_mask)*8 - 1)

enum mvista_events {
	LTT_EV_DEFINE_NAME = LTT_MVISTA_EV_BASE - 1,	/* Provide the name of an object */
	LTT_EV_DPM	/* Dynamic Power Management */
};

#define LTT_MVISTA_EV_MIN	LTT_EV_DEFINE_NAME
#define LTT_MVISTA_EV_MAX	LTT_EV_DPM
#define LTT_MVISTA_EV_NUM	(LTT_MVISTA_EV_MAX - LTT_MVISTA_EV_MIN + 1)

/* Information logged when a trace is started */
typedef struct _ltt_trace_start {
	u32 magic_number;
	u32 arch_type;
	u32 arch_variant;
	u32 system_type;
	u8 major_version;
	u8 minor_version;

	u32 buffer_size;
	ltt_event_mask event_mask;
	ltt_event_mask details_mask;
	u8 log_cpuid;
	u8 use_tsc;
	u8 flight_recorder;
} LTT_PACKED_STRUCT ltt_trace_start;

/*  LTT_SYSCALL_ENTRY */
typedef struct _ltt_syscall_entry {
	u16   syscall_id;		/* Syscall entry number in entry.S */
	ulong address;		/* Address from which call was made */
} LTT_PACKED_STRUCT ltt_syscall_entry;

/*  LTT_TRAP_ENTRY */
#ifndef __s390__
typedef struct _ltt_trap_entry {
	u16   trap_id;		/* Trap number */
	ulong address;		/* Address where trap occured */
} LTT_PACKED_STRUCT ltt_trap_entry;
static inline void ltt_ev_trap_entry(u16 trap_id, ulong address)
#else
typedef u64 trapid_t;
typedef struct _ltt_trap_entry {
	trapid_t trap_id;	/* Trap number */
	u32 address;		/* Address where trap occured */
} LTT_PACKED_STRUCT ltt_trap_entry;
static inline void ltt_ev_trap_entry(trapid_t trap_id, u32 address)
#endif
{
	ltt_trap_entry trap_event;

	trap_event.trap_id = trap_id;
	trap_event.address = address;

	ltt_log_event(LTT_EV_TRAP_ENTRY, &trap_event);
}

/*  LTT_TRAP_EXIT */
static inline void ltt_ev_trap_exit(void)
{
	ltt_log_event(LTT_EV_TRAP_EXIT, NULL);
}

/*  LTT_IRQ_ENTRY */
typedef struct _ltt_irq_entry {
	u16 irq_id;		/* IRQ number */
	u8  kernel;		/* Are we executing kernel code */
} LTT_PACKED_STRUCT ltt_irq_entry;
static inline void ltt_ev_irq_entry(u16 irq_id, u8 in_kernel)
{
	ltt_irq_entry irq_entry;

	irq_entry.irq_id = irq_id;
	irq_entry.kernel = in_kernel;

	ltt_log_event(LTT_EV_IRQ_ENTRY, &irq_entry);
}

/*  LTT_IRQ_EXIT */
static inline void ltt_ev_irq_exit(void)
{
	ltt_log_event(LTT_EV_IRQ_EXIT, NULL);
}

/*  LTT_SCHEDCHANGE */
typedef struct _ltt_schedchange {
	u32 out;		/* Outgoing process */
	ulong in;		/* Incoming process */
	u32 out_state;		/* Outgoing process' state */
} LTT_PACKED_STRUCT ltt_schedchange;
static inline void __ltt_ev_schedchange(task_t * task_out, task_t * task_in)
{
	ltt_schedchange sched_event;

	sched_event.out = (u32) task_out->pid;
	sched_event.in  = (ulong) task_in;
	sched_event.out_state = (u32) task_out->state;

	ltt_log_event(LTT_EV_SCHEDCHANGE, &sched_event);
}
#if defined(CONFIG_PREEMPT_RT) && defined(CONFIG_SMP)
#define ltt_ev_schedchange(task_out, task_in) do { } while (0)
#define ltt_ev_schedchange_rt(task_out, task_in) \
		__ltt_ev_schedchange(task_out, task_in)
#else
#define ltt_ev_schedchange(task_out, task_in) \
		__ltt_ev_schedchange(task_out, task_in)
#define ltt_ev_schedchange_rt(task_out, task_in) do { } while (0)
#endif

/*  LTT_SOFT_IRQ */
enum {
	LTT_EV_SOFT_IRQ_BOTTOM_HALF = 1,	/* Conventional bottom-half */
	LTT_EV_SOFT_IRQ_SOFT_IRQ,		/* Real soft-irq */
	LTT_EV_SOFT_IRQ_TASKLET_ACTION,		/* Tasklet action */
	LTT_EV_SOFT_IRQ_TASKLET_HI_ACTION	/* Tasklet hi-action */
};
typedef struct _ltt_soft_irq {
	u8    event_sub_id;	/* Soft-irq event Id */
	ulong event_data;
} LTT_PACKED_STRUCT ltt_soft_irq;
static inline void ltt_ev_soft_irq(u8 ev_id, ulong data)
{
	ltt_soft_irq soft_irq_event;

	soft_irq_event.event_sub_id = ev_id;
	soft_irq_event.event_data = data;

	ltt_log_event(LTT_EV_SOFT_IRQ, &soft_irq_event);
}

/*  LTT_PROCESS */
enum {
	LTT_EV_PROCESS_KTHREAD = 1,	/* Creation of a kernel thread */
	LTT_EV_PROCESS_FORK,		/* A fork or clone occured */
	LTT_EV_PROCESS_EXIT,		/* An exit occured */
	LTT_EV_PROCESS_WAIT,		/* A wait occured */
	LTT_EV_PROCESS_SIGNAL,		/* A signal has been sent */
	LTT_EV_PROCESS_WAKEUP		/* Wake up a process */
};
typedef struct _ltt_process {
	u8    event_sub_id;	/* Process event ID */
	u32   event_data1;
	ulong event_data2;
} LTT_PACKED_STRUCT ltt_process;
static inline void ltt_ev_process(u8 ev_id, u32 data1, ulong data2)
{
	ltt_process proc_event;

	proc_event.event_sub_id = ev_id;
	proc_event.event_data1 = data1;
	proc_event.event_data2 = data2;

	ltt_log_event(LTT_EV_PROCESS, &proc_event);
}
static inline void ltt_ev_process_exit(u32 data1, ulong data2)
{
	ltt_process proc_event;

	proc_event.event_sub_id = LTT_EV_PROCESS_EXIT;

	/**** WARNING ****/
	/* Regardless of whether this trace statement is active or not, these
	two function must be called, otherwise there will be inconsistencies
	in the kernel's structures. */
	ltt_destroy_owners_events(current->pid);
	ltt_free_all_handles(current);

	ltt_log_event(LTT_EV_PROCESS, &proc_event);
}

/*  LTT_FILE_SYSTEM */
enum {
	LTT_EV_FILE_SYSTEM_BUF_WAIT_START = 1,	/* Starting to wait for a data buffer */
	LTT_EV_FILE_SYSTEM_BUF_WAIT_END,	/* End to wait for a data buffer */
	LTT_EV_FILE_SYSTEM_EXEC,		/* An exec occured */
	LTT_EV_FILE_SYSTEM_OPEN,		/* An open occured */
	LTT_EV_FILE_SYSTEM_CLOSE,		/* A close occured */
	LTT_EV_FILE_SYSTEM_READ,		/* A read occured */
	LTT_EV_FILE_SYSTEM_WRITE,		/* A write occured */
	LTT_EV_FILE_SYSTEM_SEEK,		/* A seek occured */
	LTT_EV_FILE_SYSTEM_IOCTL,		/* An ioctl occured */
	LTT_EV_FILE_SYSTEM_SELECT,		/* A select occured */
	LTT_EV_FILE_SYSTEM_POLL			/* A poll occured */
};
typedef struct _ltt_file_system {
	u8 event_sub_id;	/* File system event ID */
	u32 event_data1;
	u32 event_data2;
	char *file_name;	/* Name of file operated on */
} LTT_PACKED_STRUCT ltt_file_system;
static inline void ltt_ev_file_system(u8 ev_id, u32 data1, u32 data2, const unsigned char *file_name)
{
	ltt_file_system fs_event;

	fs_event.event_sub_id = ev_id;
	fs_event.event_data1 = data1;
	fs_event.event_data2 = data2;
	fs_event.file_name = (char*) file_name;

	ltt_log_event(LTT_EV_FILE_SYSTEM, &fs_event);
}

/*  LTT_TIMER */
enum {
	LTT_EV_TIMER_EXPIRED = 1,	/* Timer expired */
	LTT_EV_TIMER_SETITIMER,		/* Setting itimer occurred */
	LTT_EV_TIMER_SETTIMEOUT		/* Setting sched timeout occurred */
};
typedef struct _ltt_timer {
	u8 event_sub_id;	/* Timer event ID */
	u8 event_sdata;		/* Short data */
	ulong event_data1;
	ulong event_data2;
} LTT_PACKED_STRUCT ltt_timer;
static inline void ltt_ev_timer(u8 ev_id, u8 sdata, ulong data1, ulong data2)
{
	ltt_timer timer_event;

	timer_event.event_sub_id = ev_id;
	timer_event.event_sdata = sdata;
	timer_event.event_data1 = data1;
	timer_event.event_data2 = data2;

	ltt_log_event(LTT_EV_TIMER, &timer_event);
}

/*  LTT_MEMORY */
enum {
	LTT_EV_MEMORY_PAGE_ALLOC = 1,	/* Allocating pages */
	LTT_EV_MEMORY_PAGE_FREE,	/* Freing pages */
	LTT_EV_MEMORY_SWAP_IN,		/* Swaping pages in */
	LTT_EV_MEMORY_SWAP_OUT,		/* Swaping pages out */
	LTT_EV_MEMORY_PAGE_WAIT_START,	/* Start to wait for page */
	LTT_EV_MEMORY_PAGE_WAIT_END	/* End to wait for page */
};
typedef struct _ltt_memory {
	u8 event_sub_id;	/* Memory event ID */
	ulong event_data;
} LTT_PACKED_STRUCT ltt_memory;
static inline void ltt_ev_memory(u8 ev_id, ulong data)
{
	ltt_memory memory_event;

	memory_event.event_sub_id = ev_id;
	memory_event.event_data = data;

	ltt_log_event(LTT_EV_MEMORY, &memory_event);
}

/*  LTT_SOCKET */
enum {
	LTT_EV_SOCKET_CALL = 1,	/* A socket call occured */
	LTT_EV_SOCKET_CREATE,	/* A socket has been created */
	LTT_EV_SOCKET_SEND,	/* Data was sent to a socket */
	LTT_EV_SOCKET_RECEIVE	/* Data was read from a socket */
};
typedef struct _ltt_socket {
	u8 event_sub_id;	/* Socket event ID */
	u32 event_data1;
	ulong  event_data2;
} LTT_PACKED_STRUCT ltt_socket;
static inline void ltt_ev_socket(u8 ev_id, u32 data1, ulong data2)
{
	ltt_socket socket_event;

	socket_event.event_sub_id = ev_id;
	socket_event.event_data1 = data1;
	socket_event.event_data2 = data2;

	ltt_log_event(LTT_EV_SOCKET, &socket_event);
}

/*  LTT_IPC */
enum {
	LTT_EV_IPC_CALL = 1,	/* A System V IPC call occured */
	LTT_EV_IPC_MSG_CREATE,	/* A message queue has been created */
	LTT_EV_IPC_SEM_CREATE,	/* A semaphore was created */
	LTT_EV_IPC_SHM_CREATE	/* A shared memory segment has been created */
};
typedef struct _ltt_ipc {
	u8 event_sub_id;	/* IPC event ID */
	u32 event_data1;
	u32 event_data2;
} LTT_PACKED_STRUCT ltt_ipc;
static inline void ltt_ev_ipc(u8 ev_id, u32 data1, u32 data2)
{
	ltt_ipc ipc_event;

	ipc_event.event_sub_id = ev_id;
	ipc_event.event_data1 = data1;
	ipc_event.event_data2 = data2;

	ltt_log_event(LTT_EV_IPC, &ipc_event);
}

/*  LTT_NETWORK */
enum {
	LTT_EV_NETWORK_PACKET_IN = 1,	/* A packet came in */
	LTT_EV_NETWORK_PACKET_OUT	/* A packet was sent */
};
typedef struct _ltt_network {
	u8 event_sub_id;	/* Network event ID */
	u32 event_data;
} LTT_PACKED_STRUCT ltt_network;
static inline void ltt_ev_network(u8 ev_id, u32 data)
{
	ltt_network net_event;

	net_event.event_sub_id = ev_id;
	net_event.event_data = data;

	ltt_log_event(LTT_EV_NETWORK, &net_event);
}

/*
 * We deliberately preserve 32bit versions of 
 * ltt_buffer_start and ltt_buffer_end structures even at 64bit architectures.
 * It doesn't yeild us troubles, but makes tracedaemon's life much easier...
 */
typedef struct _ltt_timeval_32
{
	uint32_t tv_sec;
	uint32_t tv_usec;
} LTT_PACKED_STRUCT ltt_timeval_32;

/* Start of trace buffer information */
typedef struct _ltt_buffer_start {
	ltt_timeval_32 time;	/* Time stamp of this buffer */
	u32 tsc;   		/* TSC of this buffer, if applicable */
	u32 id;			/* Unique buffer ID */
} LTT_PACKED_STRUCT ltt_buffer_start;

/* End of trace buffer information */
typedef struct _ltt_buffer_end {
	ltt_timeval_32 time;	/* Time stamp of this buffer */
	u32 tsc;   		/* TSC of this buffer, if applicable */
} LTT_PACKED_STRUCT ltt_buffer_end;

/* Custom declared events */
/* ***WARNING*** These structures should never be used as is, use the 
   provided custom event creation and logging functions. */
typedef struct _ltt_new_event {
	/* Basics */
	u32 id;						/* Custom event ID */
	char type[LTT_CUSTOM_EV_TYPE_STR_LEN];	/* Event type description */
	char desc[LTT_CUSTOM_EV_DESC_STR_LEN];	/* Detailed event description */

	/* Custom formatting */
	u32 format_type;				/* Type of formatting */
	char form[LTT_CUSTOM_EV_FORM_STR_LEN];	/* Data specific to format */
} LTT_PACKED_STRUCT ltt_new_event;
typedef struct _ltt_custom {
	u32 id;			/* Event ID */
	u32 data_size;		/* Size of data recorded by event */
	void *data;		/* Data recorded by event */
} LTT_PACKED_STRUCT ltt_custom;

/* LTT_CHANGE_MASK */
typedef struct _ltt_change_mask {
	ltt_event_mask mask;	/* Event mask */
} LTT_PACKED_STRUCT ltt_change_mask;

/*  LTT_HEARTBEAT */
static inline void ltt_ev_heartbeat(void)
{
	ltt_log_event(LTT_EV_HEARTBEAT, NULL);
}

/* LTT_DEFINE_NAME */
typedef struct _ltt_define_name
{
	uint8_t  event_sub_id;
	uint32_t event_data1;
	uint32_t event_data2;
	uint32_t event_name_len;
	char*    event_name;
} LTT_PACKED_STRUCT ltt_define_name;

static inline void 
ltt_ev_define_name(u8 event_id, u32 data1, u32 data2, unsigned char *event_name)
{
	ltt_define_name define_name_event;

	define_name_event.event_sub_id   = event_id;
	define_name_event.event_data1    = data1;
	define_name_event.event_data2    = data2;
	define_name_event.event_name_len = strlen(event_name) + 1;
	define_name_event.event_name     = (char *)event_name;

	ltt_log_event(LTT_EV_DEFINE_NAME, &define_name_event);
}

#else /* defined(CONFIG_LTT) */
#define ltt_ev(ID, DATA)
#define ltt_ev_trap_entry(ID, EIP)
#define ltt_ev_trap_exit()
#define ltt_ev_irq_entry(ID, KERNEL)
#define ltt_ev_irq_exit()
#define ltt_ev_schedchange(OUT, IN)
#define ltt_ev_schedchange_rt(OUT, IN)
#define ltt_ev_soft_irq(ID, DATA)
#define ltt_ev_process(ID, DATA1, DATA2)
#define ltt_ev_process_exit(DATA1, DATA2)
#define ltt_ev_file_system(ID, DATA1, DATA2, FILE_NAME)
#define ltt_ev_timer(ID, SDATA, DATA1, DATA2)
#define ltt_ev_memory(ID, DATA)
#define ltt_ev_socket(ID, DATA1, DATA2)
#define ltt_ev_ipc(ID, DATA1, DATA2)
#define ltt_ev_network(ID, DATA)
#define ltt_ev_heartbeat()
#define ltt_ev_define_name(ID, DATA1, DATA2, NAME)
#endif /* defined(CONFIG_LTT) */
#endif /* _LINUX_TRACE_H */
