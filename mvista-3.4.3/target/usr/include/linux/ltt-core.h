/*
 * linux/include/linux/ltt-core.h
 *
 * Copyright (C) 1999-2004 Karim Yaghmour (karim@opersys.com)
 * Copyright (C) 2004, 2005 - MontaVista Software, Inc. (source@mvista.com)
 *
 * This contains the core definitions for the Linux Trace Toolkit.
 *
 * This file is released  under the terms of the GNU GPL version 2.
 * This program  is licensed "as is" without any warranty of any kind,
 * whether express or implied.
 */

#ifndef _LTT_CORE_H
#define _LTT_CORE_H

#include <linux/config.h>
#include <linux/types.h>

#include <linux/relayfs_fs.h>

#include <asm/relay.h>

/* Is kernel tracing enabled */
#if defined(CONFIG_LTT)

/* Don't set this to "1" unless you really know what you're doing */
#define LTT_UNPACKED_STRUCTS    0

/* Structure packing within the trace */
#if LTT_UNPACKED_STRUCTS
#define LTT_PACKED_STRUCT
#else
#define LTT_PACKED_STRUCT __attribute__ ((packed))
#endif

#define LTT_CUSTOM_EV_MAX_SIZE		8192
#define LTT_CUSTOM_EV_TYPE_STR_LEN	20
#define LTT_CUSTOM_EV_DESC_STR_LEN	100
#define LTT_CUSTOM_EV_FORM_STR_LEN	256
#define LTT_CUSTOM_EV_FINAL_STR_LEN	200

#define LTT_CUSTOM_EV_FORMAT_TYPE_NONE	0
#define LTT_CUSTOM_EV_FORMAT_TYPE_STR	1
#define LTT_CUSTOM_EV_FORMAT_TYPE_HEX	2
#define LTT_CUSTOM_EV_FORMAT_TYPE_XML	3
#define LTT_CUSTOM_EV_FORMAT_TYPE_IBM	4

#define LTT_MAX_HANDLES			256

/* In the ltt root directory lives the trace control file, used for
   kernel-user communication. */
#define LTT_RELAYFS_ROOT		"ltt"
#define LTT_CONTROL_FILE		"control"

#define LTT_PROC_FILE			"proc"
#define LTT_PROC_SUBBUF_SIZE		8192
#define LTT_PROC_SUBBUF_NUM		4

/* We currently support 2 traces, normal trace and flight recorder */
#define NR_TRACES			2
#define TRACE_HANDLE			0
#define FLIGHT_HANDLE			1

/* System types */
#define LTT_SYS_TYPE_VANILLA_LINUX	1
#define LTT_SYS_TYPE_MONTAVISTA_LINUX	0x004C564D	/* MVL */

/* Architecture types */
#define LTT_ARCH_TYPE_I386		1
#define LTT_ARCH_TYPE_PPC		2
#define LTT_ARCH_TYPE_SH		3
#define LTT_ARCH_TYPE_S390		4
#define LTT_ARCH_TYPE_MIPS		5
#define LTT_ARCH_TYPE_ARM		6
/* #define LTT_ARCH_TYPE_X8664		7 - not implemented yet */
#define LTT_ARCH_TYPE_PPC64		8

/* Standard definitions for variants */
#define LTT_ARCH_VARIANT_NONE		0 /* Main architecture implementation */

/* The maximum number of CPUs the kernel might run on */
#define LTT_MAX_NR_CPUS			32

typedef u64 ltt_event_mask;

/* Per-CPU channel information */
struct ltt_channel_data
{
	int channel_handle;
	struct rchan_reader *reader;
	atomic_t waiting_for_cpu_async;
	u32 events_lost;
};

/* Per-trace status info */
struct ltt_trace_info
{
	int			active;
	unsigned int		trace_handle;
	int			paused;
	int			flight_recorder;
	int			use_locking;
	int			using_tsc;
	u32			n_buffers;
	u32			buf_size;
	ltt_event_mask		traced_events;
	ltt_event_mask		log_event_details_mask;
	u32			buffers_produced[LTT_MAX_NR_CPUS];
} LTT_PACKED_STRUCT;

/* Status info for all traces */
struct ltt_tracer_status
{
	int num_cpus;
	struct ltt_trace_info traces[NR_TRACES];
} LTT_PACKED_STRUCT;

/* Per-trace information - each trace/flight recorder represented by one */
struct ltt_trace_struct
{
	unsigned int		trace_handle;	/* For convenience */
	struct ltt_trace_struct	*active;	/* 'this' if active, or NULL */
	int			paused;		/* Not currently logging */
	struct ltt_channel_data relay_data[NR_CPUS]; /* Relayfs handles, by CPU */
	int			flight_recorder;/* i.e. this is not a trace */
	int			proc_channel;	/* '/proc' info */
	struct task_struct	*daemon_task_struct;/* Daemon associated with trace */
	struct _ltt_trace_start	*trace_start_data; /* Trace start event data, for flight recorder */
	int			tracer_started;
	int			tracer_stopping;
	struct proc_dir_entry	*proc_dir_entry;	/* proc/ltt/0..1 */
	ltt_event_mask		traced_events;
	ltt_event_mask		log_event_details_mask;
	u32			n_buffers;	/* Number of sub-buffers */
	u32			buf_size;	/* Size of sub-buffer */
	int			use_locking;
	int			using_tsc;
	int			log_cpuid;
	int			tracing_pid;
	int			tracing_pgrp;
	int			tracing_gid;
	int			tracing_uid;
	pid_t			traced_pid;
	pid_t			traced_pgrp;
	gid_t			traced_gid;
	uid_t			traced_uid;
	unsigned long		buffer_switches_pending;/* For trace */
	struct work_struct	work;	/* stop work struct */
};

extern int ltt_set_trace_config(
	int		do_syscall_depth,
	int		do_syscall_bounds,
	int		eip_depth,
	void		*eip_lower_bound,
	void		*eip_upper_bound);
extern void ltt_set_flight_recorder_config(
	struct ltt_trace_struct	*trace);
extern int ltt_get_trace_config(
	int		*do_syscall_depth,
	int		*do_syscall_bounds,
	int		*eip_depth,
	void		**eip_lower_bound,
	void		**eip_upper_bound);
extern int ltt_get_status(
	struct ltt_tracer_status	*tracer_status);
extern int ltt_create_event(
	char		*event_type,
	char		*event_desc,
	int		format_type,
	char		*format_data);
extern int ltt_create_owned_event(
	char		*event_type,
	char		*event_desc,
	int		format_type,
	char		*format_data,
	pid_t		owner_pid);
extern void ltt_destroy_event(
	int		event_id);
extern void ltt_destroy_owners_events(
	pid_t		owner_pid);
extern void ltt_reregister_custom_events(void);
extern int ltt_log_std_formatted_event(
	int		event_id,
	...);
extern int ltt_log_raw_event(
	int		event_id,
	int		event_size,
	void		*event_data);
extern int _ltt_log_event(
	struct ltt_trace_struct	*trace,
	u8			event_id,
	void			*event_struct,
	u8			cpu_id);
extern int ltt_log_event(
	u8		event_id,
	void		*event_struct);
extern int ltt_valid_trace_handle(
	unsigned int	tracer_handle);
extern int ltt_alloc_trace_handle(
	unsigned int	tracer_handle);
extern int ltt_free_trace_handle(
	unsigned int	tracer_handle);
extern int ltt_free_daemon_handle(
	struct ltt_trace_struct *trace);
extern void ltt_free_all_handles(
	struct task_struct*	task_ptr);
extern int ltt_set_buffer_size(
	struct ltt_trace_struct	*trace,
	int			buffers_size, 
	char			*dirname);
extern int ltt_set_n_buffers(
	struct ltt_trace_struct	*trace,
	int			no_buffers);
extern int ltt_set_default_config(
	struct ltt_trace_struct	*trace);
extern int ltt_syscall_active(
	int syscall_type);
extern void ltt_flight_pause(
	void);
extern void ltt_flight_unpause(
	void);

/* Tracer properties */
#define LTT_TRACER_DEFAULT_BUF_SIZE   50000
#define LTT_TRACER_MIN_BUF_SIZE        1000
#define LTT_TRACER_MAX_BUF_SIZE      500000
#define LTT_TRACER_MIN_BUFFERS            2
#define LTT_TRACER_MAX_BUFFERS          256
#define LTT_TRACER_MAGIC_NUMBER     0x00D6B7ED
#define LTT_TRACER_VERSION_MAJOR    2
#define LTT_TRACER_VERSION_MINOR    2

#define LTT_TRACER_FIRST_EVENT_SIZE   (sizeof(u8) + sizeof(u32) + sizeof(ltt_buffer_start) + sizeof(uint16_t))
#define LTT_TRACER_START_TRACE_EVENT_SIZE   (sizeof(u8) + sizeof(u32) + sizeof(ltt_trace_start) + sizeof(uint16_t))
#define LTT_TRACER_LAST_EVENT_SIZE   (sizeof(u8) \
				  + sizeof(u8) \
				  + sizeof(u32) \
				  + sizeof(ltt_buffer_end) \
				  + sizeof(uint16_t) \
				  + sizeof(u32))

/* The configurations possible */
enum {
	LTT_TRACER_START = LTT_TRACER_MAGIC_NUMBER,	/* Start tracing events using the current configuration */
	LTT_TRACER_STOP,				/* Stop tracing */
	LTT_TRACER_CONFIG_DEFAULT,			/* Set the tracer to the default configuration */
	LTT_TRACER_CONFIG_MEMORY_BUFFERS,		/* Set the memory buffers the daemon wants us to use */
	LTT_TRACER_CONFIG_EVENTS,			/* Trace the given events */
	LTT_TRACER_CONFIG_DETAILS,			/* Record the details of the event, or not */
	LTT_TRACER_CONFIG_CPUID,			/* Record the CPUID associated with the event */
	LTT_TRACER_CONFIG_PID,				/* Trace only one process */
	LTT_TRACER_CONFIG_PGRP,				/* Trace only the given process group */
	LTT_TRACER_CONFIG_GID,				/* Trace the processes of a given group of users */
	LTT_TRACER_CONFIG_UID,				/* Trace the processes of a given user */
	LTT_TRACER_CONFIG_SYSCALL_EIP_DEPTH,		/* Set the call depth at which the EIP should be fetched on syscall */
	LTT_TRACER_CONFIG_SYSCALL_EIP_LOWER,		/* Set the lowerbound address from which EIP is recorded on syscall */
	LTT_TRACER_CONFIG_SYSCALL_EIP_UPPER,		/* Set the upperbound address from which EIP is recorded on syscall */
	LTT_TRACER_DATA_COMITTED,			/* The daemon has comitted the last trace */
	LTT_TRACER_GET_EVENTS_LOST,			/* Get the number of events lost */
	LTT_TRACER_CREATE_USER_EVENT,			/* Create a user tracable event */
	LTT_TRACER_DESTROY_USER_EVENT,			/* Destroy a user tracable event */
	LTT_TRACER_TRACE_USER_EVENT,			/* Trace a user event */
	LTT_TRACER_SET_EVENT_MASK,			/* Set the trace event mask */
	LTT_TRACER_GET_EVENT_MASK,			/* Get the trace event mask */
	LTT_TRACER_GET_BUFFER_CONTROL,			/* Get the buffer control data for the lockless schem*/
	LTT_TRACER_CONFIG_N_MEMORY_BUFFERS,		/* Set the number of memory buffers the daemon wants us to use */
	LTT_TRACER_CONFIG_USE_LOCKING,			/* Set the locking scheme to use */
	LTT_TRACER_CONFIG_TIMESTAMP,			/* Set the timestamping method to use */
	LTT_TRACER_GET_ARCH_INFO,			/* Get information about the CPU configuration */
	LTT_TRACER_ALLOC_HANDLE,			/* Allocate a tracer handle */
	LTT_TRACER_FREE_HANDLE,				/* Free a single handle */
	LTT_TRACER_FREE_DAEMON_HANDLE,			/* Free the daemon's handle */
	LTT_TRACER_FREE_ALL_HANDLES,			/* Free all handles */
	LTT_TRACER_MAP_BUFFER,				/* Map buffer to process-space */
	LTT_TRACER_PAUSE,				/* Pause tracing */
	LTT_TRACER_UNPAUSE,				/* Unpause tracing */
	LTT_TRACER_GET_START_INFO,			/* trace start data */
	LTT_TRACER_GET_STATUS			/* status of traces */
};

/* Lockless scheme definitions */
#define LTT_TRACER_LOCKLESS_MIN_BUF_SIZE LTT_CUSTOM_EV_MAX_SIZE + 8192
#define LTT_TRACER_LOCKLESS_MAX_BUF_SIZE 0x1000000

/* Flags used for per-CPU tasks */
#define LTT_NOTHING_TO_DO      0x00
#define LTT_FINALIZE_TRACE     0x02
#define LTT_TRACE_HEARTBEAT    0x08

/* How often the LTT per-CPU timers fire */
#define LTT_PERCPU_TIMER_FREQ  (HZ/10);

/* Convenience accessors */
#define waiting_for_cpu_async(trace_handle, cpu) (current_traces[trace_handle].relay_data[cpu].waiting_for_cpu_async)
#define trace_channel_handle(trace_handle, cpu) (current_traces[trace_handle].relay_data[cpu].channel_handle)
#define trace_channel_reader(trace_handle, cpu) (current_traces[trace_handle].relay_data[cpu].reader)
#define trace_buffers_full(cpu) (daemon_relay_data[cpu].buffers_full)
#define events_lost(trace_handle, cpu) (current_traces[trace_handle].relay_data[cpu].events_lost)

/* struct used on each ioctl to pass the tracer handle and command argument */
struct ltt_control_data
{
	u32 tracer_handle;
	ulong command_arg;
} LTT_PACKED_STRUCT;

/* this struct is used ONLY in the syscall parameters used when a user wants
 * to trace a custom event, not to log the event, it adds the data pointer */
struct ltt_custom_cmd
{
	u32 id;          /* Event ID */
	u32 data_size;   /* Size of data recorded by event */
	void *data;
};

/* Used for sharing per-buffer information between driver and daemon */
struct ltt_buf_control_info
{
	s16 cpu_id;
	u32 buffer_switches_pending;
	u32 buffer_control_valid;

	u32 buf_size;
	u32 n_buffers;
	u32 cur_idx;
	u32 buffers_produced;
	u32 buffers_consumed;
	int buffer_complete[LTT_TRACER_MAX_BUFFERS];
} LTT_PACKED_STRUCT;

/* Used for sharing buffer-commit information between driver and daemon */
struct ltt_buffers_committed
{
	u8 cpu_id;
	u32 buffers_consumed;
} LTT_PACKED_STRUCT;

/* Used for specifying size/cpu id pair between driver and daemon */
struct ltt_cpu_mmap_data
{
	u8 cpu_id;
	unsigned long map_size;
} LTT_PACKED_STRUCT;

/* Used for sharing architecture-specific info between driver and daemon */
struct ltt_arch_info
{
	int n_cpus;
	int page_shift;
} LTT_PACKED_STRUCT;

extern __inline__ int ltt_set_bit(int nr, void *addr)
{
	unsigned char *p = addr;
	unsigned char mask = 1 << (nr & 7);
	unsigned char old;

	p += nr >> 3;
	old = *p;
	*p |= mask;

	return ((old & mask) != 0);
}

extern __inline__ int ltt_clear_bit(int nr, void *addr)
{
	unsigned char *p = addr;
	unsigned char mask = 1 << (nr & 7);
	unsigned char old;

	p += nr >> 3;
	old = *p;
	*p &= ~mask;

	return ((old & mask) != 0);
}

extern __inline__ int ltt_test_bit(int nr, void *addr)
{
	unsigned char *p = addr;
	unsigned char mask = 1 << (nr & 7);

	p += nr >> 3;

	return ((*p & mask) != 0);
}

/**
 *	switch_time_delta: - Utility function getting buffer switch time delta.
 *	@time_delta: previously calculated or retrieved time delta 
 *
 *	Returns the time_delta passed in if we're using TSC or 0 otherwise.
 */
static inline u32 switch_time_delta(u32 time_delta,
				    int using_tsc)
{
	if((using_tsc == 1) && have_tsc())
		return time_delta;
	else
		return 0;
}

#else /* defined(CONFIG_LTT) */
static inline int ltt_create_event(char	*event_type,
		    char	*event_desc,
		    int		format_type,
		    char	*format_data)
{
	return 0;
}

static inline int ltt_create_owned_event(char		*event_type,
			   char		*event_desc,
			   int		format_type,
			   char		*format_data,
			   pid_t	owner_pid)
{
	return 0;
}

static inline void ltt_destroy_event(int event_id)
{
}

static inline void ltt_destroy_owners_events(pid_t owner_pid)
{
}

static inline void ltt_reregister_custom_events(void)
{
}

static inline int ltt_log_std_formatted_event(int event_id, ...)
{
	return 0;
}


static inline  int ltt_log_raw_event(int	event_id,
				     int	event_size,
				     void	*event_data)
{
	return 0;
}

static inline  int _ltt_log_event(u8	event_id,
				  void	*event_struct,
				  u8	cpu_id)
{
	return 0;
}

static inline int ltt_log_event(u8	event_id,
				void	*event_struct)
{
	return 0;
}

static inline void ltt_flight_pause(void)
{
}

static inline void ltt_flight_unpause(void)
{
}

#endif /* defined(CONFIG_LTT) */
#endif /* _LTT_CORE_H */
