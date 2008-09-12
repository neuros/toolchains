/*
 * RTAIDB.h
 *
 * Copyright (C) 2000 Karim Yaghmour (karym@opersys.com).
 *
 * This is distributed under GPL.
 *
 * Header for event database engine for RTAI traces.
 *
 * History : 
 *    K.Y., 12/06/2000, Initial typing.
 */

#ifndef __TRACE_TOOLKIT_RTAI_DB__
#define __TRACE_TOOLKIT_RTAI_DB__

/* Structure type */
#define RTAI_TASK_OBJECT          1     /* type = "struct _task" */

/* RTAI task description */
typedef struct _RTAItask
{
  /* DO NOT MOVE THIS FROM THE BEGINING OF THE STRUCT */
  /* Structure type */
  int                Type;            /* Type of structure */  

  /* Graphic properties of task */
  int                ListPos;         /* Position of task in process list */

  /* Properties */
  int                TID;             /* TID of task */
  int                PTID;            /* TID of task's parent */
  int                IsProcessBuddy;  /* This RT task is a Linux process buddy */
  int                IsProcessShadow; /* This RT task is a Linux process shadow */
  process*           BuddyProcess;    /* Linux process of which this is the RT buddy */
  process*           ShadowProcess;   /* Linux process of which this is the RT shadow */
  systemInfo*        System;          /* The system to which this task belongs */

  /* Temporary data used during graph construction */
  struct timeval     ReportedSwitchTime; /* Time of occurence of last task switch with this task in */

  /* Communication data */
  int                QFIFOPut;        /* Quantity of bytes PUT into FIFO */
  int                QFIFOGet;        /* Quantity of bytes GET from FIFO */

  /* Time and scheduling data */
  int                NbTimesScheduled;  /* Number of times that the task was scheduled */
  event              LastSchedIn;       /* Last time the task was scheduled in */
  event              LastSchedOut;      /* Last time the task was scheduled out */
  struct timeval     TimeOfBirth ;      /* Time at which process was created (TRACE_RTAI_TASK_INIT occurence) */
  struct timeval     TimeRunning;       /* Time during which this process had the control over the CPU */
  struct timeval     TimeBetweenRuns;   /* Time spent waiting to get scheduled */
  
  /* List hooks */
  struct _RTAItask*  Next;            /* Next task in list */
  struct _RTAItask*  Children;        /* Pointer to first child */
  struct _RTAItask*  NextChild;       /* Next child in child list */ 
  struct _RTAItask*  NextHash;        /* Next task in hash table entry */
} RTAItask;

#define TASK_HASH_SIZE  32

/* The description of a RTAI system */
typedef struct _RTAIsystem
{
  /* Statistics */
  /*  Core events */
  unsigned long int  NbRTEvents;      /* Number of RT events */
  unsigned long int  Mount;           /* Number of RTAI mounts */
  unsigned long int  UMount;          /* Number of RTAI un-mounts */
  unsigned long int  GlobalIRQEntry;  /* Number of Global IRQ entries */
  unsigned long int  GlobalIRQExit;   /* Number of Global IRQ exits */
  unsigned long int  CPUOwnIRQEntry;  /* Number of CPU-own IRQ entries */
  unsigned long int  CPUOwnIRQExit;   /* Number of CPU-own IRQ exits */
  unsigned long int  TrapEntry;       /* Number of trap entries */
  unsigned long int  TrapExit;        /* Number of trap exits */
  unsigned long int  SRQEntry;        /* Number of SRQ entries */
  unsigned long int  SRQExit;         /* Number of SRQ exits */
  unsigned long int  SwitchToLinux;   /* Number of switches to Linux */
  unsigned long int  SwitchToRT;      /* Number of switches to RT */
  unsigned long int  SchedChange;     /* Number of RT scheduling changes */
  unsigned long int  LXRTSchedChange; /* Number of LXRT scheduling changes */

  /* Lists of system components */
  RTAItask*         Tasks;                    /* All the tasks seen during trace */
  RTAItask*         TaskTree;                 /* The task tree */
  RTAItask          TaskHash[TASK_HASH_SIZE]; /* Hash table containing pointer to the tasks */
} RTAIsystem;

/* The different states an RTAI system can be in */
typedef enum _RTAIsystemState
{
  NO_STATE = 0,
  RTAI_CORE,
  RTAI_TASK,
  LINUX_KERNEL,
  LINUX_PROCESS
}RTAIsystemState;

/* Type conversion from a generic pointer into a RTAIsystem pointer */
#define RTAI_SYSTEM(X) ((RTAIsystem*)X)

/* Function prototypes */
int              RTAIDBIEventString
                       (db*              /* Database to which event belongs */,
	                uint32_t         /* ID of buffer containing event */,
	                void*            /* Event's start address */,
	                char*            /* Buffer where event string is to be put */,
	                int              /* Destination buffer's size */);
RTAItask*        RTAIDBEventTask
                       (db*              /* Database to which event belongs */,
		        event*           /* Pointer to event */,
		        systemInfo*      /* System to which event belongs */,
			int              /* Should the time of occurence of the corresponding sched-change be remembered */);
RTAIsystemState  RTAIDBGetCurrentState
                       (db*              /* Database to which this event belongs */,		 
			event*           /* Pointer to event */,
			systemInfo*      /* System to which this event belongs */);
RTAIsystemState  RTAIDBEventState
                       (db*              /* Database to which this event belongs */,
			event*           /* Pointer to event */,
			process*         /* Pointer to currently running process */,
			RTAIsystemState  /* The current system state */,
			RTAItask*        /* Pointer to the task running prior to this event */);
RTAItask*        RTAIDBFindTaskInTree
                       (int              /* ID of Task we are looking for */,
		        RTAItask*        /* Tree in which we should look for task */);
int              RTAIDBProcessTrace
                       (systemInfo*      /* Trace system description */,
	                db*              /* Database of events */,
	                options*         /* User-given options */);
void             RTAIDBPrintTrace
                       (int              /* File to which data is to be printed */,
			systemInfo*      /* System who's information is to be printed */,
			db*              /* Database of events */,
			options*         /* Options */);
RTAItask*        RTAIDBCreateTask
                       (systemInfo*      /* The system to which the task belongs */,
		        int              /* The TID of the created task */,
		        int              /* The TID of the parent task */);
void             RTAIDBDestroySystem
                       (systemInfo*      /* System for which RTAI-specific info. is to be desctroyed */);

#endif /* __TRACE_TOOLKIT_RTAI_DB__ */
