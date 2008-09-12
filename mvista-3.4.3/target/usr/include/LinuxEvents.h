/*
 * LinuxEvents.h
 *
 * Copyright (C) 2000, 2001, 2002 Karim Yaghmour (karym@opersys.com).
 *
 * This header is distributed under GPL.
 *
 * Linux events being traced.
 *
 * History : 
 *    K.Y., 31/05/1999, Initial typing.
 *
 */

#ifndef __TRACE_TOOLKIT_LINUX_HEADER__
#define __TRACE_TOOLKIT_LINUX_HEADER__

#include <LTTTypes.h>

/* Traced events */
#define TRACE_START           0    /* This is to mark the trace's start */
#define TRACE_SYSCALL_ENTRY   1    /* Entry in a given system call */
#define TRACE_SYSCALL_EXIT    2    /* Exit from a given system call */
#define TRACE_TRAP_ENTRY      3    /* Entry in a trap */
#define TRACE_TRAP_EXIT       4    /* Exit from a trap */
#define TRACE_IRQ_ENTRY       5    /* Entry in an irq */
#define TRACE_IRQ_EXIT        6    /* Exit from an irq */
#define TRACE_SCHEDCHANGE     7    /* Scheduling change */
#define TRACE_KERNEL_TIMER    8    /* The kernel timer routine has been called */
#define TRACE_SOFT_IRQ        9    /* Hit key part of soft-irq management */
#define TRACE_PROCESS        10    /* Hit key part of process management */
#define TRACE_FILE_SYSTEM    11    /* Hit key part of file system */
#define TRACE_TIMER          12    /* Hit key part of timer management */
#define TRACE_MEMORY         13    /* Hit key part of memory management */
#define TRACE_SOCKET         14    /* Hit key part of socket communication */
#define TRACE_IPC            15    /* Hit key part of inter-process communication */
#define TRACE_NETWORK        16    /* Hit key part of network communication */

#define TRACE_BUFFER_START   17    /* Mark the begining of a trace buffer */
#define TRACE_BUFFER_END     18    /* Mark the ending of a trace buffer */
#define TRACE_NEW_EVENT      19    /* New event type */
#define TRACE_CUSTOM         20    /* Custom event */

#define TRACE_CHANGE_MASK    21    /* Change in event mask */
#define TRACE_HEARTBEAT      22    /* Heartbeat event */

/* Number of traced events */
#define TRACE_MAX		TRACE_HEARTBEAT

/* Begin at the high bits of the trace_event_mask and go downward */
#define TRACE_MVISTA_BASE	(sizeof(uint64_t)*8 - 1)

#define TRACE_DEFINE_NAME	(TRACE_MVISTA_BASE - 1) /* Provide the name of an object */
#define TRACE_DPM		(TRACE_MVISTA_BASE)     /* Dynamic Power Management */

#define TRACE_MVISTA_MIN	TRACE_DEFINE_NAME
#define TRACE_MVISTA_MAX	TRACE_DPM
#define TRACE_MVISTA_NUM	(TRACE_MVISTA_MAX - TRACE_MVISTA_MIN + 1)

/* Architecture types */
#define TRACE_ARCH_TYPE_I386                1   /* i386 system */
#define TRACE_ARCH_TYPE_PPC                 2   /* PPC system */
#define TRACE_ARCH_TYPE_SH                  3   /* SH system */
#define TRACE_ARCH_TYPE_S390                4   /* S/390 system */
#define TRACE_ARCH_TYPE_MIPS                5   /* MIPS system */
#define TRACE_ARCH_TYPE_ARM                 6   /* ARM system */
/* #define TRACE_ARCH_TYPE_X8664            7   Not implemented yet */
#define TRACE_ARCH_TYPE_PPC64               8

/* Standard definitions for variants */
#define TRACE_ARCH_VARIANT_NONE             0   /* Main architecture implementation */

/* PowerPC variants */
#define TRACE_ARCH_VARIANT_PPC_4xx          1   /* 4xx systems (IBM embedded series) */
#define TRACE_ARCH_VARIANT_PPC_6xx          2   /* 6xx/7xx/74xx/8260/POWER3 systems (desktop flavor) */
#define TRACE_ARCH_VARIANT_PPC_8xx          3   /* 8xx system (Motoral embedded series) */
#define TRACE_ARCH_VARIANT_PPC_ISERIES      4   /* 8xx system (iSeries) */
#define TRACE_ARCH_VARIANT_PPC_PSERIES      5   /* PPC64 only */

/* System types */
#define TRACE_SYS_TYPE_VANILLA_LINUX        1   /* Vanilla linux kernel  */
#define TRACE_SYS_TYPE_RTAI_LINUX           2   /* RTAI patched linux kernel */
#define TRACE_SYS_TYPE_MONTAVISTA_LINUX     0x004C564D /* "MVL" */

/* The information logged when the tracing is started */
#define TRACER_MAGIC_NUMBER        0x00D6B7ED     /* That day marks an important historical event ... */
#define TRACER_SUP_VERSION_MAJOR            2     /* Major version number */
#define TRACER_SUP_VERSION_MINOR            2     /* Minor version number */

typedef struct _ltt_timeval
{
	uint32_t tv_sec;
	uint32_t tv_usec;
} ltt_timeval;

typedef struct _trace_start
{
  uint32_t           MagicNumber;      /* Magic number to identify a trace */
  uint32_t           ArchType;         /* Type of architecture */
  uint32_t           ArchVariant;      /* Variant of the given type of architecture */
  uint32_t           SystemType;       /* Operating system type */
  uint8_t            MajorVersion;     /* Major version of trace */
  uint8_t            MinorVersion;     /* Minor version of trace */

  uint32_t           BufferSize;       /* Size of buffers */
  trace_event_mask   EventMask;        /* The event mask */
  trace_event_mask   DetailsMask;      /* Are the event details logged */
  uint8_t            LogCPUID;         /* Is the CPUID logged */
  uint8_t            UseTSC;	       /* Are we using TSCs or time deltas */

  uint8_t            FlightRecorder;   /* Is this a flight recorder trace? */
} LTT_PACKED_STRUCT trace_start;
#define START_EVENT(X) ((trace_start*)X)

/*  TRACE_SYSCALL_ENTRY */
typedef struct _trace_syscall_entry
{
  uint8_t   syscall_id;   /* Syscall entry number in entry.S */
  uint32_t  address;      /* Address from which call was made */
} LTT_PACKED_STRUCT trace_syscall_entry;
#define SYSCALL_EVENT(X) ((trace_syscall_entry*)X)

/*  TRACE_SYSCALL_ENTRY */
typedef struct _trace_syscall_entry16
{
  uint16_t   syscall_id;   /* Syscall entry number in entry.S */
  uint32_t  address;      /* Address from which call was made */
} LTT_PACKED_STRUCT trace_syscall_entry16;
#define SYSCALL_EVENT16(X) ((trace_syscall_entry16*)X)

/*  TRACE_SYSCALL_ENTRY */
typedef struct _trace_syscall_entry64
{
  uint16_t   syscall_id;   /* Syscall entry number in entry.S */
  uint64_t   address;      /* Address from which call was made */
} LTT_PACKED_STRUCT trace_syscall_entry64;
#define SYSCALL_EVENT64(X) ((trace_syscall_entry64*)X)

/*  TRACE_TRAP_ENTRY */
typedef struct _trace_trap_entry
{
  uint16_t  trap_id;     /* Trap number */
  uint32_t  address;     /* Address where trap occured */
} LTT_PACKED_STRUCT trace_trap_entry;
#define TRAP_EVENT(X) ((trace_trap_entry*)X)

/*  TRACE_TRAP_ENTRY */
typedef struct _trace_trap_entry64
{
  uint16_t  trap_id;     /* Trap number */
  uint64_t  address;     /* Address where trap occured */
} LTT_PACKED_STRUCT trace_trap_entry64;
#define TRAP_EVENT64(X) ((trace_trap_entry64*)X)

typedef struct _trace_trap_entry_s390
{
  uint64_t  trap_id;     /* Trap number */
  uint32_t  address;     /* Address where trap occured */
} LTT_PACKED_STRUCT trace_trap_entry_s390;
#define TRAP_EVENT_S390(X) ((trace_trap_entry_s390*)X)

/*  TRACE_IRQ_ENTRY */
typedef struct _trace_irq_entry
{
  uint8_t  irq_id;      /* IRQ number */
  uint8_t  kernel;      /* Are we executing kernel code */
} LTT_PACKED_STRUCT trace_irq_entry;
#define IRQ_EVENT(X) ((trace_irq_entry*)X)

typedef struct _trace_irq_entry16
{
  uint16_t  irq_id;      /* IRQ number */
  uint8_t   kernel;      /* Are we executing kernel code */
} LTT_PACKED_STRUCT trace_irq_entry16;
#define IRQ_EVENT16(X) ((trace_irq_entry16*)X)

/*  TRACE_SCHEDCHANGE */ 
typedef struct _trace_schedchange
{
  uint32_t  out;         /* Outgoing process */
  uint32_t  in;          /* Incoming process */
  uint32_t  out_state;   /* Outgoing process' state */
} LTT_PACKED_STRUCT trace_schedchange;
#define SCHED_EVENT(X) ((trace_schedchange*)X)

/*  TRACE_SCHEDCHANGE */ 
typedef struct _trace_schedchange64
{
  uint32_t  out;         /* Outgoing process */
  uint64_t  in;          /* Incoming process */
  uint32_t  out_state;   /* Outgoing process' state */
} LTT_PACKED_STRUCT trace_schedchange64;
#define SCHED_EVENT64(X) ((trace_schedchange64*)X)

/*  TRACE_SOFT_IRQ */
#define TRACE_SOFT_IRQ_BOTTOM_HALF        1  /* Conventional bottom-half */
#define TRACE_SOFT_IRQ_SOFT_IRQ           2  /* Real soft-irq */
#define TRACE_SOFT_IRQ_TASKLET_ACTION     3  /* Tasklet action */
#define TRACE_SOFT_IRQ_TASKLET_HI_ACTION  4  /* Tasklet hi-action */

typedef struct _trace_soft_irq
{
  uint8_t   event_sub_id;     /* Soft-irq event Id */
  uint32_t  event_data;       /* Data associated with event */
} LTT_PACKED_STRUCT trace_soft_irq;
#define SOFT_IRQ_EVENT(X) ((trace_soft_irq*)X)

typedef struct _trace_soft_irq64
{
  uint8_t   event_sub_id;     /* Soft-irq event Id */
  uint64_t  event_data;       /* Data associated with event */
} LTT_PACKED_STRUCT trace_soft_irq64;
#define SOFT_IRQ_EVENT64(X) ((trace_soft_irq64*)X)

/*  TRACE_PROCESS */
#define TRACE_PROCESS_KTHREAD     1  /* Creation of a kernel thread */
#define TRACE_PROCESS_FORK        2  /* A fork or clone occured */
#define TRACE_PROCESS_EXIT        3  /* An exit occured */
#define TRACE_PROCESS_WAIT        4  /* A wait occured */
#define TRACE_PROCESS_SIGNAL      5  /* A signal has been sent */
#define TRACE_PROCESS_WAKEUP      6  /* Wake up a process */

typedef struct _trace_process
{
  uint8_t   event_sub_id;    /* Process event ID */
  uint32_t  event_data1;     /* Data associated with event */
  uint32_t  event_data2;    
} LTT_PACKED_STRUCT trace_process;
#define PROC_EVENT(X) ((trace_process*)X)

typedef struct _trace_process64
{
  uint8_t   event_sub_id;    /* Process event ID */
  uint32_t  event_data1;     /* Data associated with event */
  uint64_t  event_data2;    
} LTT_PACKED_STRUCT trace_process64;
#define PROC_EVENT64(X) ((trace_process64*)X)

/*  TRACE_FILE_SYSTEM */
#define TRACE_FILE_SYSTEM_BUF_WAIT_START  1  /* Starting to wait for a data buffer */
#define TRACE_FILE_SYSTEM_BUF_WAIT_END    2  /* End to wait for a data buffer */
#define TRACE_FILE_SYSTEM_EXEC            3  /* An exec occured */
#define TRACE_FILE_SYSTEM_OPEN            4  /* An open occured */
#define TRACE_FILE_SYSTEM_CLOSE           5  /* A close occured */
#define TRACE_FILE_SYSTEM_READ            6  /* A read occured */
#define TRACE_FILE_SYSTEM_WRITE           7  /* A write occured */
#define TRACE_FILE_SYSTEM_SEEK            8  /* A seek occured */
#define TRACE_FILE_SYSTEM_IOCTL           9  /* An ioctl occured */
#define TRACE_FILE_SYSTEM_SELECT         10  /* A select occured */
#define TRACE_FILE_SYSTEM_POLL           11  /* A poll occured */
typedef struct _trace_file_system
{
  uint8_t   event_sub_id;   /* File system event ID */
  uint32_t  event_data1;    /* Event data */
  uint32_t  event_data2;    /* Event data 2 */
  char*     file_name;      /* Name of file operated on */
} LTT_PACKED_STRUCT trace_file_system;
#define FS_EVENT(X) ((trace_file_system*)X)
#define FS_EVENT_FILENAME(X, isArch64)                          \
	( (isArch64) ?                                          \
		((char*) ((X) + 4 + sizeof(trace_file_system))) \
			:                                       \
		((char*) ((X) + sizeof(trace_file_system)))     \
	)

/*  TRACE_TIMER */
#define TRACE_TIMER_EXPIRED      1  /* Timer expired */
#define TRACE_TIMER_SETITIMER    2  /* Setting itimer occurred */
#define TRACE_TIMER_SETTIMEOUT   3  /* Setting sched timeout occurred */

typedef struct _trace_timer
{
  uint8_t   event_sub_id;    /* Timer event ID */
  uint8_t   event_sdata;     /* Short data */
  uint32_t  event_data1;     /* Data associated with event */
  uint32_t  event_data2;     
} LTT_PACKED_STRUCT trace_timer;
#define TIMER_EVENT(X) ((trace_timer*)X)

typedef struct _trace_timer64
{
  uint8_t   event_sub_id;    /* Timer event ID */
  uint8_t   event_sdata;     /* Short data */
  uint64_t  event_data1;     /* Data associated with event */
  uint64_t  event_data2;     
} LTT_PACKED_STRUCT trace_timer64;
#define TIMER_EVENT64(X) ((trace_timer64*)X)

/*  TRACE_MEMORY */
#define TRACE_MEMORY_PAGE_ALLOC        1  /* Allocating pages */
#define TRACE_MEMORY_PAGE_FREE         2  /* Freing pages */
#define TRACE_MEMORY_SWAP_IN           3  /* Swaping pages in */
#define TRACE_MEMORY_SWAP_OUT          4  /* Swaping pages out */
#define TRACE_MEMORY_PAGE_WAIT_START   5  /* Start to wait for page */
#define TRACE_MEMORY_PAGE_WAIT_END     6  /* End to wait for page */

typedef struct _trace_memory
{
  uint8_t        event_sub_id;    /* Memory event ID */
  unsigned long  event_data;      /* Data associated with event */
} LTT_PACKED_STRUCT trace_memory;
#define MEM_EVENT(X) ((trace_memory*)X)

typedef struct _trace_memory64
{
  uint8_t        event_sub_id;    /* Memory event ID */
  uint64_t       event_data;      /* Data associated with event */
} LTT_PACKED_STRUCT trace_memory64;
#define MEM_EVENT64(X) ((trace_memory64*)X)

/*  TRACE_SOCKET */
#define TRACE_SOCKET_CALL     1  /* A socket call occured */
#define TRACE_SOCKET_CREATE   2  /* A socket has been created */
#define TRACE_SOCKET_SEND     3  /* Data was sent to a socket */
#define TRACE_SOCKET_RECEIVE  4  /* Data was read from a socket */

typedef struct _trace_socket
{
  uint8_t   event_sub_id;    /* Socket event ID */
  uint32_t  event_data1;     /* Data associated with event */
  uint32_t  event_data2;     /* Data associated with event */
} LTT_PACKED_STRUCT trace_socket;
#define SOCKET_EVENT(X) ((trace_socket*)X)

typedef struct _trace_socket64
{
  uint8_t   event_sub_id;    /* Socket event ID */
  uint32_t  event_data1;     /* Data associated with event */
  uint64_t  event_data2;     /* Data associated with event */
} LTT_PACKED_STRUCT trace_socket64;
#define SOCKET_EVENT64(X) ((trace_socket64*)X)

/*  TRACE_IPC */
#define TRACE_IPC_CALL            1  /* A System V IPC call occured */
#define TRACE_IPC_MSG_CREATE      2  /* A message queue has been created */
#define TRACE_IPC_SEM_CREATE      3  /* A semaphore was created */
#define TRACE_IPC_SHM_CREATE      4  /* A shared memory segment has been created */
typedef struct _trace_ipc
{
  uint8_t   event_sub_id;    /* IPC event ID */
  uint32_t  event_data1;     /* Data associated with event */
  uint32_t  event_data2;     /* Data associated with event */
} LTT_PACKED_STRUCT trace_ipc;
#define IPC_EVENT(X) ((trace_ipc*)X)

/*  TRACE_NETWORK */
#define TRACE_NETWORK_PACKET_IN   1  /* A packet came in */
#define TRACE_NETWORK_PACKET_OUT  2  /* A packet was sent */
typedef struct _trace_network
{
  uint8_t  event_sub_id;   /* Network event ID */
  uint32_t event_data;     /* Event data */
} LTT_PACKED_STRUCT trace_network;
#define NET_EVENT(X) ((trace_network*)X)

/* Start of trace buffer information */
typedef struct _trace_buffer_start
{
  ltt_timeval        Time;    /* Time stamp of this buffer */
  uint32_t           TSC;     /* TSC of this buffer, if applicable */
  uint32_t           ID;      /* Unique buffer ID */
} LTT_PACKED_STRUCT trace_buffer_start;

/* End of trace buffer information */
typedef struct _trace_buffer_end
{
  ltt_timeval        Time;    /* Time stamp of this buffer */
  uint32_t           TSC;     /* TSC of this buffer, if applicable */
} LTT_PACKED_STRUCT trace_buffer_end;

/* Maximal size a custom event can have */
#define CUSTOM_EVENT_MAX_SIZE        8192

/* String length limits for custom events creation */
#define CUSTOM_EVENT_TYPE_STR_LEN      20
#define CUSTOM_EVENT_DESC_STR_LEN     100
#define CUSTOM_EVENT_FORM_STR_LEN     256

/* Type of custom event formats */
#define CUSTOM_EVENT_FORMAT_TYPE_NONE   0
#define CUSTOM_EVENT_FORMAT_TYPE_STR    1
#define CUSTOM_EVENT_FORMAT_TYPE_HEX    2
#define CUSTOM_EVENT_FORMAT_TYPE_XML    3
#define CUSTOM_EVENT_FORMAT_TYPE_IBM    4

typedef struct _trace_new_event
{
  /* Basics */
  uint32_t         id;                                /* Custom event ID */
  char             type[CUSTOM_EVENT_TYPE_STR_LEN];   /* Event type description */
  char             desc[CUSTOM_EVENT_DESC_STR_LEN];   /* Detailed event description */

  /* Custom formatting */
  uint32_t         format_type;                       /* Type of formatting */
  char             form[CUSTOM_EVENT_FORM_STR_LEN];   /* Data specific to format */
} LTT_PACKED_STRUCT trace_new_event;
#define NEW_EVENT(X) ((trace_new_event*) X)

typedef struct _trace_custom
{
  uint32_t           id;          /* Event ID */
  uint32_t           data_size;   /* Size of data recorded by event */
  void*              data;        /* Data recorded by event */
} LTT_PACKED_STRUCT trace_custom;
#define CUSTOM_EVENT(X) ((trace_custom*) X)

/* TRACE_CHANGE_MASK */
typedef struct _trace_change_mask
{
  trace_event_mask          mask;       /* Event mask */
} LTT_PACKED_STRUCT trace_change_mask;
#define CHMASK_EVENT(X) ((trace_change_mask*) X)

/* TRACE_DEFINE_NAME */
#define TRACE_DEFINE_NAME_DPM_STATE       3
typedef struct _trace_define_name
{
  uint8_t   event_sub_id;
  uint32_t  event_data1;
  uint32_t  event_data2;
  uint32_t  event_name_len;
  char*     event_name;
} LTT_PACKED_STRUCT trace_define_name;
#define DEFINE_EVENT(X) ((trace_define_name*) X)
#define DEFINE_EVENT_NAME(X, isArch64)                          \
	( (isArch64) ?                                          \
		((char*) ((X) + 4 + sizeof(trace_define_name))) \
			:					\
		((char*) ((X) + sizeof(trace_define_name)))     \
	)

/* TRACE_DPM */
#define TRACE_DPM_OP   1     /* operating point */
#define TRACE_DPM_OS   2     /* operating state */
typedef struct _trace_dpm
{
  uint8_t   event_sub_id;
  uint32_t  event_data1;
  char*     event_name;
} LTT_PACKED_STRUCT trace_dpm;
#define DPM_EVENT(X) ((trace_dpm*) X)
#define DPM_EVENT_NAME(X, isArch64)                             \
	( (isArch64) ?                                          \
		((char*) ((X) + 4 + sizeof(trace_dpm)))         \
			:					\
		((char*) ((X) + sizeof(trace_dpm)))             \
	)

#endif /* __TRACE_TOOLKIT_LINUX_HEADER__ */
