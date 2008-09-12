/*
 * LinuxEvents.h
 *
 * Copyright (C) 2000 Karim Yaghmour (karym@opersys.com).
 *
 * This is distributed under GPL.
 *
 * Linux events being traced.
 *
 * History : 
 *    K.Y., 31/05/1999, Initial typing.
 *
 */

#ifndef __TRACE_TOOLKIT_RTAI_HEADER__
#define __TRACE_TOOLKIT_RTAI_HEADER__

#include <LTTTypes.h>

/* Begining ID for RTAI events */
#define TRACE_RTAI_START  TRACE_MAX

/* Traced events */
#define TRACE_RTAI_MOUNT              TRACE_RTAI_START +  1   /* The RTAI subsystem was mounted */
#define TRACE_RTAI_UMOUNT             TRACE_RTAI_START +  2   /* The RTAI subsystem was unmounted */
#define TRACE_RTAI_GLOBAL_IRQ_ENTRY   TRACE_RTAI_START +  3   /* Entry in a global IRQ */
#define TRACE_RTAI_GLOBAL_IRQ_EXIT    TRACE_RTAI_START +  4   /* Exit from a global IRQ */
#define TRACE_RTAI_OWN_IRQ_ENTRY      TRACE_RTAI_START +  5   /* Entry in a CPU own IRQ */
#define TRACE_RTAI_OWN_IRQ_EXIT       TRACE_RTAI_START +  6   /* Exit from a CPU own IRQ */
#define TRACE_RTAI_TRAP_ENTRY         TRACE_RTAI_START +  7   /* Entry in a trap */
#define TRACE_RTAI_TRAP_EXIT          TRACE_RTAI_START +  8   /* Exit from a trap */
#define TRACE_RTAI_SRQ_ENTRY          TRACE_RTAI_START +  9   /* Entry in a SRQ */
#define TRACE_RTAI_SRQ_EXIT           TRACE_RTAI_START + 10   /* Exit from a SRQ */
#define TRACE_RTAI_SWITCHTO_LINUX     TRACE_RTAI_START + 11   /* Switch a CPU to Linux */
#define TRACE_RTAI_SWITCHTO_RT        TRACE_RTAI_START + 12   /* Switch a CPU to real-time */
#define TRACE_RTAI_SCHED_CHANGE       TRACE_RTAI_START + 13   /* A scheduling change has occured */
#define TRACE_RTAI_TASK               TRACE_RTAI_START + 14   /* Hit key part of task services */
#define TRACE_RTAI_TIMER              TRACE_RTAI_START + 15   /* Hit key part of timer services */
#define TRACE_RTAI_SEM                TRACE_RTAI_START + 16   /* Hit key part of semaphore services */
#define TRACE_RTAI_MSG                TRACE_RTAI_START + 17   /* Hit key part of message services */
#define TRACE_RTAI_RPC                TRACE_RTAI_START + 18   /* Hit key part of RPC services */
#define TRACE_RTAI_MBX                TRACE_RTAI_START + 19   /* Hit key part of mail box services */
#define TRACE_RTAI_FIFO               TRACE_RTAI_START + 20   /* Hit key part of FIFO services */
#define TRACE_RTAI_SHM                TRACE_RTAI_START + 21   /* Hit key part of shared memory services */
#define TRACE_RTAI_POSIX              TRACE_RTAI_START + 22   /* Hit key part of Posix services */
#define TRACE_RTAI_LXRT               TRACE_RTAI_START + 23   /* Hit key part of LXRT services */
#define TRACE_RTAI_LXRTI              TRACE_RTAI_START + 24   /* Hit key part of LXRT-Informed services */

/* Max number of traced events */
#define TRACE_RTAI_MAX               TRACE_RTAI_LXRTI

/* Structures and macros for traced events */
/*  TRACE_RTAI_GLOBAL_IRQ_ENTRY */
typedef struct _trace_rtai_global_irq_entry
{
  uint8_t  irq_id;      /* IRQ number */
  uint8_t  kernel;      /* Are we executing kernel code */
} LTT_PACKED_STRUCT trace_rtai_global_irq_entry;
#define RTAI_GLOBAL_IRQ_ENTRY_EVENT(X) ((trace_rtai_global_irq_entry*)X)

/*  TRACE_RTAI_OWN_IRQ_ENTRY */
typedef struct _trace_rtai_own_irq_entry
{
  uint8_t  irq_id;      /* IRQ number */
  uint8_t  kernel;      /* Are we executing kernel code */
} LTT_PACKED_STRUCT trace_rtai_own_irq_entry;
#define RTAI_OWN_IRQ_ENTRY_EVENT(X) ((trace_rtai_own_irq_entry*)X)

/*  TRACE_RTAI_TRAP_ENTRY */
typedef struct _trace_rtai_trap_entry
{
  uint8_t   trap_id;        /* Trap number */
  uint32_t  address;        /* Address where trap occured */
} LTT_PACKED_STRUCT trace_rtai_trap_entry;
#define RTAI_TRAP_ENTRY_EVENT(X) ((trace_rtai_trap_entry*)X)

/*  TRACE_RTAI_SRQ_ENTRY */
typedef struct _trace_rtai_srq_entry
{
  uint8_t  srq_id;      /* SRQ number */
  uint8_t  kernel;      /* Are we executing kernel code */
} LTT_PACKED_STRUCT trace_rtai_srq_entry;
#define RTAI_SRQ_ENTRY_EVENT(X) ((trace_rtai_srq_entry*)X)

/*  TRACE_RTAI_SWITCHTO_LINUX */
typedef struct _trace_rtai_switchto_linux
{
  uint8_t   cpu_id;         /* The CPUID being switched to Linux */
} LTT_PACKED_STRUCT trace_rtai_switchto_linux;
#define RTAI_SWITCHTO_LINUX_EVENT(X) ((trace_rtai_switchto_linux*)X)

/*  TRACE_RTAI_SWITCHTO_RT */
typedef struct _trace_rtai_switchto_rt
{
  uint8_t   cpu_id;         /* The CPUID being switched to RT */
} LTT_PACKED_STRUCT trace_rtai_switchto_rt;
#define RTAI_SWITCHTO_RT_EVENT(X) ((trace_rtai_switchto_rt*)X)

/*  TRACE_RTAI_SCHED_CHANGE */ 
typedef struct _trace_rtai_sched_change
{
  uint32_t  out;         /* Outgoing process */
  uint32_t  in;          /* Incoming process */
  uint32_t  out_state;   /* Outgoing process' state */
} LTT_PACKED_STRUCT trace_rtai_sched_change;
#define RTAI_SCHED_CHANGE_EVENT(X) ((trace_rtai_sched_change*)X)

/*  TRACE_RTAI_TASK */
#define TRACE_RTAI_TASK_INIT                    1     /* Initialize task */
#define TRACE_RTAI_TASK_DELETE                  2     /* Delete task */
#define TRACE_RTAI_TASK_SIG_HANDLER             3     /* Set signal handler */
#define TRACE_RTAI_TASK_YIELD                   4     /* Yield CPU control */
#define TRACE_RTAI_TASK_SUSPEND                 5     /* Suspend task */
#define TRACE_RTAI_TASK_RESUME                  6     /* Resume task */
#define TRACE_RTAI_TASK_MAKE_PERIOD_RELATIVE    7     /* Make task periodic relative in nanoseconds */
#define TRACE_RTAI_TASK_MAKE_PERIOD             8     /* Make task periodic */
#define TRACE_RTAI_TASK_WAIT_PERIOD             9     /* Wait until the next period */
#define TRACE_RTAI_TASK_BUSY_SLEEP             10     /* Busy sleep */
#define TRACE_RTAI_TASK_SLEEP                  11     /* Sleep */
#define TRACE_RTAI_TASK_SLEEP_UNTIL            12     /* Sleep until */
typedef struct _trace_rtai_task
{
  uint8_t   event_sub_id;  /* Task event ID */
  uint32_t  event_data1;   /* Event data */
  uint64_t  event_data2;   /* Event data 2 */
  uint64_t  event_data3;   /* Event data 3 */
} LTT_PACKED_STRUCT trace_rtai_task;
#define RTAI_TASK_EVENT(X) ((trace_rtai_task*)X)

/*  TRACE_RTAI_TIMER */
#define TRACE_RTAI_TIMER_REQUEST                1     /* Request timer */
#define TRACE_RTAI_TIMER_FREE                   2     /* Free timer */
#define TRACE_RTAI_TIMER_REQUEST_APIC           3     /* Request APIC timers */
#define TRACE_RTAI_TIMER_APIC_FREE              4     /* Free APIC timers */
#define TRACE_RTAI_TIMER_HANDLE_EXPIRY          5     /* Handle timer expiry */
typedef struct _trace_rtai_timer
{
  uint8_t   event_sub_id;  /* Timer event ID */
  uint32_t  event_data1;   /* Event data 1 */
  uint32_t  event_data2;   /* Event data 2 */
} LTT_PACKED_STRUCT trace_rtai_timer;
#define RTAI_TIMER_EVENT(X) ((trace_rtai_timer*)X)

/*  TRACE_RTAI_SEM */
#define TRACE_RTAI_SEM_INIT                     1     /* Initialize semaphore */
#define TRACE_RTAI_SEM_DELETE                   2     /* Delete semaphore */
#define TRACE_RTAI_SEM_SIGNAL                   3     /* Signal semaphore */
#define TRACE_RTAI_SEM_WAIT                     4     /* Wait on semaphore */
#define TRACE_RTAI_SEM_WAIT_IF                  5     /* Take semaphore if possible */
#define TRACE_RTAI_SEM_WAIT_UNTIL               6     /* Wait on semaphore until a certain time */
typedef struct _trace_rtai_sem
{
  uint8_t   event_sub_id;  /* Semaphore event ID */
  uint32_t  event_data1;   /* Event data 1 */
  uint64_t  event_data2;   /* Event data 2 */
} LTT_PACKED_STRUCT trace_rtai_sem;
#define RTAI_SEM_EVENT(X) ((trace_rtai_sem*)X)

/*  TRACE_RTAI_MSG */
#define TRACE_RTAI_MSG_SEND                      1    /* Send a message */
#define TRACE_RTAI_MSG_SEND_IF                   2    /* Send if possible */
#define TRACE_RTAI_MSG_SEND_UNTIL                3    /* Try sending until a certain time */
#define TRACE_RTAI_MSG_RECV                      4    /* Receive a message */
#define TRACE_RTAI_MSG_RECV_IF                   5    /* Receive if possible */
#define TRACE_RTAI_MSG_RECV_UNTIL                6    /* Try receiving until a certain time */
typedef struct _trace_rtai_msg
{
  uint8_t   event_sub_id;  /* Message event ID */
  uint32_t  event_data1;   /* Event data 1 */
  uint32_t  event_data2;   /* Event data 2 */
  uint64_t  event_data3;   /* Event data 3 */
} LTT_PACKED_STRUCT trace_rtai_msg;
#define RTAI_MSG_EVENT(X) ((trace_rtai_msg*)X)

/*  TRACE_RTAI_RPC */
#define TRACE_RTAI_RPC_MAKE                       1    /* Make a remote procedure call */
#define TRACE_RTAI_RPC_MAKE_IF                    2    /* Make RPC if receiver is ready */
#define TRACE_RTAI_RPC_MAKE_UNTIL                 3    /* Try making an RPC until a certain time */
#define TRACE_RTAI_RPC_RETURN                     4    /* Send result of RPC back to caller */
typedef struct _trace_rtai_rpc
{
  uint8_t   event_sub_id;  /* RPC event ID */
  uint32_t  event_data1;   /* Event data 1 */
  uint32_t  event_data2;   /* Event data 2 */
  uint64_t  event_data3;   /* Event data 3 */
} LTT_PACKED_STRUCT trace_rtai_rpc;
#define RTAI_RPC_EVENT(X) ((trace_rtai_rpc*)X)

/*  TRACE_RTAI_MBX */
#define TRACE_RTAI_MBX_INIT                       1    /* Initialize Message BoX */
#define TRACE_RTAI_MBX_DELETE                     2    /* Delete message box */
#define TRACE_RTAI_MBX_SEND                       3    /* Send a message to a message box */
#define TRACE_RTAI_MBX_SEND_WP                    4    /* Send as many bytes as possible */
#define TRACE_RTAI_MBX_SEND_IF                    5    /* Send a message if possible */
#define TRACE_RTAI_MBX_SEND_UNTIL                 6    /* Try sending until a certain time */
#define TRACE_RTAI_MBX_RECV                       7    /* Receive a message */
#define TRACE_RTAI_MBX_RECV_WP                    8    /* Receive as many bytes as possible */
#define TRACE_RTAI_MBX_RECV_IF                    9    /* Receive a message if available */
#define TRACE_RTAI_MBX_RECV_UNTIL                10    /* Try receiving until a certain time */
typedef struct _trace_rtai_mbx
{
  uint8_t   event_sub_id;  /* Message Box event ID */
  uint32_t  event_data1;   /* Event data 1 */
  uint32_t  event_data2;   /* Event data 2 */
  uint64_t  event_data3;   /* Event data 3 */  
} LTT_PACKED_STRUCT trace_rtai_mbx;
#define RTAI_MBX_EVENT(X) ((trace_rtai_mbx*)X)

/*  TRACE_RTAI_FIFO */
#define TRACE_RTAI_FIFO_CREATE                     1   /* Create FIFO */
#define TRACE_RTAI_FIFO_DESTROY                    2   /* Destroy FIFO */
#define TRACE_RTAI_FIFO_RESET                      3   /* Reset FIFO */
#define TRACE_RTAI_FIFO_RESIZE                     4   /* Resize FIFO */
#define TRACE_RTAI_FIFO_PUT                        5   /* Write data to FIFO */
#define TRACE_RTAI_FIFO_GET                        6   /* Get data from FIFO */
#define TRACE_RTAI_FIFO_CREATE_HANDLER             7   /* Install FIFO handler */
#define TRACE_RTAI_FIFO_OPEN                       8   /* Open FIFO */
#define TRACE_RTAI_FIFO_RELEASE                    9   /* Release FIFO */
#define TRACE_RTAI_FIFO_READ                      10   /* Read from FIFO */
#define TRACE_RTAI_FIFO_WRITE                     11   /* Write to FIFO */
#define TRACE_RTAI_FIFO_READ_TIMED                12   /* Read with time limit */
#define TRACE_RTAI_FIFO_WRITE_TIMED               13   /* Write with time limit */
#define TRACE_RTAI_FIFO_READ_ALLATONCE            14   /* Read all the data from FIFO */
#define TRACE_RTAI_FIFO_LLSEEK                    15   /* Seek position into FIFO */
#define TRACE_RTAI_FIFO_FASYNC                    16   /* Asynchronous notification */
#define TRACE_RTAI_FIFO_IOCTL                     17   /* IO control on FIFO */
#define TRACE_RTAI_FIFO_POLL                      18   /* Poll FIFO */
#define TRACE_RTAI_FIFO_SUSPEND_TIMED             19   /* Suspend task for given period */
#define TRACE_RTAI_FIFO_SET_ASYNC_SIG             20   /* Set asynchrounous signal */
#define TRACE_RTAI_FIFO_SEM_INIT                  21   /* Initialize semaphore */
#define TRACE_RTAI_FIFO_SEM_POST                  22   /* Post semaphore */
#define TRACE_RTAI_FIFO_SEM_WAIT                  23   /* Wait on semaphore */
#define TRACE_RTAI_FIFO_SEM_TRY_WAIT              24   /* Try waiting on semaphore */
#define TRACE_RTAI_FIFO_SEM_TIMED_WAIT            25   /* Wait on semaphore until a certain time */
#define TRACE_RTAI_FIFO_SEM_DESTROY               26   /* Destroy semaphore  */
typedef struct _trace_rtai_fifo
{
  uint8_t   event_sub_id;  /* FIFO event ID */
  uint32_t  event_data1;   /* Event data 1 */
  uint32_t  event_data2;   /* Event data 2 */
} LTT_PACKED_STRUCT trace_rtai_fifo;
#define RTAI_FIFO_EVENT(X) ((trace_rtai_fifo*)X)

/*  TRACE_RTAI_SHM */
#define TRACE_RTAI_SHM_MALLOC                       1  /* Allocate shared memory */
#define TRACE_RTAI_SHM_KMALLOC                      2  /* Allocate shared memory in kernel space */
#define TRACE_RTAI_SHM_GET_SIZE                     3  /* Get the size of the shared memory area */ 
#define TRACE_RTAI_SHM_FREE                         4  /* Free shared memory */
#define TRACE_RTAI_SHM_KFREE                        5  /* Free kernel space shared memory */
typedef struct _trace_rtai_shm
{
  uint8_t   event_sub_id;  /* SHared Memory event ID */
  uint32_t  event_data1;   /* Event data 1 */
  uint32_t  event_data2;   /* Event data 2 */
  uint32_t  event_data3;   /* Event data 3 */
} LTT_PACKED_STRUCT trace_rtai_shm;
#define RTAI_SHM_EVENT(X) ((trace_rtai_shm*)X)

/*  TRACE_RTAI_POSIX */
#define TRACE_RTAI_POSIX_MQ_OPEN                       1  /* Open/create message queue */
#define TRACE_RTAI_POSIX_MQ_CLOSE                      2  /* Close message queue */
#define TRACE_RTAI_POSIX_MQ_SEND                       3  /* Send message to queue */
#define TRACE_RTAI_POSIX_MQ_RECV                       4  /* Receive message from queue */
#define TRACE_RTAI_POSIX_MQ_GET_ATTR                   5  /* Get message queue attributes */
#define TRACE_RTAI_POSIX_MQ_SET_ATTR                   6  /* Set message queue attributes */
#define TRACE_RTAI_POSIX_MQ_NOTIFY                     7  /* Register to be notified of message arrival */
#define TRACE_RTAI_POSIX_MQ_UNLINK                     8  /* Destroy message queue */
#define TRACE_RTAI_POSIX_PTHREAD_CREATE                9  /* Create RT task */
#define TRACE_RTAI_POSIX_PTHREAD_EXIT                 10  /* Terminate calling thread */
#define TRACE_RTAI_POSIX_PTHREAD_SELF                 11  /* Get thread ID */
#define TRACE_RTAI_POSIX_PTHREAD_ATTR_INIT            12  /* Initialize thread attribute */
#define TRACE_RTAI_POSIX_PTHREAD_ATTR_DESTROY         13  /* Destroy thread attribute */
#define TRACE_RTAI_POSIX_PTHREAD_ATTR_SETDETACHSTATE  14  /* Set detach state of thread */
#define TRACE_RTAI_POSIX_PTHREAD_ATTR_GETDETACHSTATE  15  /* Get detach state of thread */
#define TRACE_RTAI_POSIX_PTHREAD_ATTR_SETSCHEDPARAM   16  /* Set thread scheduling parameters */
#define TRACE_RTAI_POSIX_PTHREAD_ATTR_GETSCHEDPARAM   17  /* Get thread scheduling parameters */
#define TRACE_RTAI_POSIX_PTHREAD_ATTR_SETSCHEDPOLICY  18  /* Set thread scheduling policy */
#define TRACE_RTAI_POSIX_PTHREAD_ATTR_GETSCHEDPOLICY  19  /* Get thread scheduling policy */
#define TRACE_RTAI_POSIX_PTHREAD_ATTR_SETINHERITSCHED 20  /* Set thread scheduling inheritance */
#define TRACE_RTAI_POSIX_PTHREAD_ATTR_GETINHERITSCHED 21  /* Get thread scheduling inheritance */
#define TRACE_RTAI_POSIX_PTHREAD_ATTR_SETSCOPE        22  /* Set thread scheduling scope */
#define TRACE_RTAI_POSIX_PTHREAD_ATTR_GETSCOPE        23  /* Get thread scheduling scope */
#define TRACE_RTAI_POSIX_PTHREAD_SCHED_YIELD          24  /* Yield processor control */
#define TRACE_RTAI_POSIX_PTHREAD_CLOCK_GETTIME        25  /* Get current clock count */
#define TRACE_RTAI_POSIX_PTHREAD_MUTEX_INIT           26  /* Initialize mutex */
#define TRACE_RTAI_POSIX_PTHREAD_MUTEX_DESTROY        27  /* Destroy mutex */
#define TRACE_RTAI_POSIX_PTHREAD_MUTEXATTR_INIT       28  /* Initiatize mutex attribute */
#define TRACE_RTAI_POSIX_PTHREAD_MUTEXATTR_DESTROY    29  /* Destroy mutex attribute */
#define TRACE_RTAI_POSIX_PTHREAD_MUTEXATTR_SETKIND_NP 30  /* Set kind of attribute */
#define TRACE_RTAI_POSIX_PTHREAD_MUTEXATTR_GETKIND_NP 31  /* Get kind of attribute */
#define TRACE_RTAI_POSIX_PTHREAD_SETSCHEDPARAM        32  /* Set scheduling parameters */
#define TRACE_RTAI_POSIX_PTHREAD_GETSCHEDPARAM        33  /* Get scheduling parameters */
#define TRACE_RTAI_POSIX_PTHREAD_MUTEX_TRY_LOCK       34  /* Non-blocking mutex lock */
#define TRACE_RTAI_POSIX_PTHREAD_MUTEX_LOCK           35  /* Blocking mutex lock */
#define TRACE_RTAI_POSIX_PTHREAD_MUTEX_UNLOCK         36  /* Mutex unlock */
#define TRACE_RTAI_POSIX_PTHREAD_COND_INIT            37  /* Initialize conditionnal variable */
#define TRACE_RTAI_POSIX_PTHREAD_COND_DESTROY         38  /* Destroy cond. variable */
#define TRACE_RTAI_POSIX_PTHREAD_CONDATTR_INIT        39  /* Initialize cond. attribute variable */
#define TRACE_RTAI_POSIX_PTHREAD_CONDATTR_DESTROY     40  /* Destroy cond. attribute variable */
#define TRACE_RTAI_POSIX_PTHREAD_COND_WAIT            41  /* Wait for cond. variable to be signaled */
#define TRACE_RTAI_POSIX_PTHREAD_COND_TIMEDWAIT       42  /* Wait for a certain time */
#define TRACE_RTAI_POSIX_PTHREAD_COND_SIGNAL          43  /* Signal a waiting thread */
#define TRACE_RTAI_POSIX_PTHREAD_COND_BROADCAST       44  /* Signal all waiting threads */
typedef struct _trace_rtai_posix
{
  uint8_t   event_sub_id;  /* POSIX event ID */
  uint32_t  event_data1;   /* Event data 1 */
  uint32_t  event_data2;   /* Event data 2 */
  uint32_t  event_data3;   /* Event data 3 */
} LTT_PACKED_STRUCT trace_rtai_posix;
#define RTAI_POSIX_EVENT(X) ((trace_rtai_posix*)X)

/*  TRACE_RTAI_LXRT */
#define TRACE_RTAI_LXRT_RTAI_SYSCALL_ENTRY             1  /* Entry in LXRT syscall */
#define TRACE_RTAI_LXRT_RTAI_SYSCALL_EXIT              2  /* Exit from LXRT syscall */
#define TRACE_RTAI_LXRT_SCHED_CHANGE                   3  /* Scheduling change */
#define TRACE_RTAI_LXRT_STEAL_TASK                     4  /* Take task control from Linux */
#define TRACE_RTAI_LXRT_GIVE_BACK_TASK                 5  /* Give task control back to Linux */
#define TRACE_RTAI_LXRT_SUSPEND                        6  /* Suspend a task */
#define TRACE_RTAI_LXRT_RESUME                         7  /* Resume task's execution */
#define TRACE_RTAI_LXRT_HANDLE                         8  /* Handle a request for an RTAI service */
typedef struct _trace_rtai_lxrt
{
  uint8_t   event_sub_id;  /* LXRT event ID */
  uint32_t  event_data1;   /* Event data 1 */
  uint32_t  event_data2;   /* Event data 2 */
  uint32_t  event_data3;   /* Event data 3 */  
} LTT_PACKED_STRUCT trace_rtai_lxrt;
#define RTAI_LXRT_EVENT(X) ((trace_rtai_lxrt*)X)

/*  TRACE_RTAI_LXRTI */
#define TRACE_RTAI_LXRTI_NAME_ATTACH                    1  /* Register current process as name */
#define TRACE_RTAI_LXRTI_NAME_LOCATE                    2  /* Locate a given process usint it's name */
#define TRACE_RTAI_LXRTI_NAME_DETACH                    3  /* Detach process from name */
#define TRACE_RTAI_LXRTI_SEND                           4  /* Send message to PID */
#define TRACE_RTAI_LXRTI_RECV                           5  /* Receive message */
#define TRACE_RTAI_LXRTI_CRECV                          6  /* Non-blocking receive */
#define TRACE_RTAI_LXRTI_REPLY                          7  /* Reply to message received */
#define TRACE_RTAI_LXRTI_PROXY_ATTACH                   8  /* Attach proxy to process */
#define TRACE_RTAI_LXRTI_PROXY_DETACH                   9  /* Detach proxy from process */
#define TRACE_RTAI_LXRTI_TRIGGER                       10  /* Trigger proxy */
typedef struct _trace_rtai_lxrti
{
  uint8_t   event_sub_id;  /* LXRT event ID */
  uint32_t  event_data1;   /* Event data 1 */
  uint32_t  event_data2;   /* Event data 2 */
  uint64_t  event_data3;   /* Event data 3 */  
} LTT_PACKED_STRUCT trace_rtai_lxrti;
#define RTAI_LXRTI_EVENT(X) ((trace_rtai_lxrti*)X)

#endif /* __TRACE_TOOLKIT_RTAI_HEADER */
