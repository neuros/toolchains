/*
 * EventDB.h
 *
 * Copyright (C) 1999 Karim Yaghmour.
 *
 * This is distributed under GPL.
 *
 * Header for event database engine for trace toolkit.
 *
 * History : 
 *    K.Y., 26/06/99, Initial typing.
 */

#ifndef __TRACE_TOOLKIT_EVENT_DB__
#define __TRACE_TOOLKIT_EVENT_DB__

#include <stdio.h>

#include <LinuxEvents.h>
#include <RTAIEvents.h>
#include <EventOptions.h>

/* Definitions */
#define EVENT_STRING_MAX_SIZE       256
#define HEADER_BANER  "####################################################################\n"
#define HEADER_CPUID  "CPU-ID"
#define HEADER_EVENT  "Event\t\t"
#define HEADER_TIME   "\tTime\t\t"
#define HEADER_PID    "\tPID"
#define HEADER_LENGTH "\tLength"
#define HEADER_STRING "\tDescription"

/* Length of time representing strings */
#define TIME_STR_LEN 80

/* Maximum number of interrupts possible */
#define MAX_NB_INTERRUPTS 0x400

/* Maximum traced processor number */
#define MAX_CPUID 128

/* Maximum number of DPM states possible */
#define MAX_NB_DPM_STATES 32

/* Trace processing errors */
#define ERR_UNKNOWN_TRACE_TYPE    -1   /* Trace type not recognized */
#define ERR_EMPTY_TRACE           -2   /* Trace doesn't contain any events */

/******************************************************************
 * Macro :
 *    PrintData()
 * Description :
 *    Prints out the given data in the given format to the given file.
 * Parameters :
 *    pmFile, File to which we write.
 *    pmFormat, Format of the data.
 *    pmArg, The data to be printed
 * History :
 *    K.Y., 17/10/99, Initial typing
 ******************************************************************/
#define DB_PRINT_STR_LEN  256
#define DBPrintData(pmFile, pmFormat, pmArg...) \
do \
{\
  char         lWriteString[DB_PRINT_STR_LEN];\
\
  snprintf(lWriteString, DB_PRINT_STR_LEN, pmFormat, ##pmArg);\
  write(pmFile, lWriteString, strlen(lWriteString));\
} while(0)

/* Time operation macros */
/*  (T3 = T2 - T1) */
#define DBTimeSub(T3, T2, T1) \
do \
{\
  T3.tv_sec  = T2.tv_sec  - T1.tv_sec;  \
  T3.tv_usec = T2.tv_usec;		\
  if (T2.tv_usec < T1.tv_usec)		\
    {					\
    T3.tv_sec--;			\
    T3.tv_usec += 1000000;		\
    }					\
  T3.tv_usec -= T1.tv_usec;		\
} while(0)

/*  (T3 = T2 + T1) */
#define DBTimeAdd(T3, T2, T1) \
do \
{\
  T3.tv_sec  = T2.tv_sec  + T1.tv_sec;  \
  T3.tv_usec = T2.tv_usec + T1.tv_usec; \
  if(T3.tv_usec >= 1000000)\
    {\
    T3.tv_sec += T3.tv_usec / 1000000;\
    T3.tv_usec = T3.tv_usec % 1000000;\
    }\
} while(0)

/*  Get the time in the double format */
#define DBGetTimeInDouble(X) (double) (((double)X.tv_sec)*1000000 + (double)X.tv_usec)

/* Events are equal */
#define DBEventsEqual(X, Y)   ((X.BufID == Y.BufID) && (X.EventPos == Y.EventPos))

#define DBIEventGetBufStart(pmDB, pmEvent) \
        (pmEvent->BufID * pmDB->BufferSize + pmDB->TraceStart)
#define DBIEventGetPosition(pmDB, pmEvent) \
        (DBIEventGetBufStart(pmDB, pmEvent) + pmEvent->EventPos)

/* Event structure */
typedef struct _event
{
  uint32_t         BufID;    /* Buffer ID of the buffer to which this event belongs */
  uint32_t         EventPos; /* Event position in the buffer */
} event;

typedef struct _eventDescription
{
  /* Characteristics as read from raw trace */
  int              ID;                             /* The ID number of the event as given in linux/include/linux/profile.h */
  ltt_timeval      Time;                           /* Time at which event occured */
  int              CPUID;                          /* CPU on which this event occured */
  void*            Struct;                         /* Structured describing the event */
  char             String[EVENT_STRING_MAX_SIZE];  /* String describing the event */
  int              Size;                           /* Size of entry describing event (as was recorded in kernel trace module) */
} eventDescription;

/* Custom event description */
typedef struct _customEventDesc
{
  /* The event itself */
  uint32_t                    ID;
  trace_new_event             Event;               /* The "event creation" event containing the custom event description */

  /* List links */
  struct _customEventDesc*    Next;
  struct _customEventDesc*    Prev;

  /* Hash links */
  struct _customEventDesc*    NextHash;
} customEventDesc;

/* Size of custom event hash table */
#define CUSTOM_EVENT_HASH_TABLE_SIZE    256

/* Size to add to reallocated TSC rollover count/address structs */
#define ROLLOVER_REALLOC_SIZE          256
#define ROLLOVER_ADDRS_REALLOC_SIZE    256

/* Data structure holding TSC rollover information for a given buffer.  A
   preprocessing pass goes through the database and calculates how many
   times and at what addresses the TSC counter rolled over in each buffer.
   This information is necessary for converting TSC timestamps into time
   deltas subsequently by the time retrieval DB functions. */
typedef struct _TSCRollover
{
  uint32_t                  NRollovers;        /* # rollovers seen in buffer */
  uint32_t                  RolloverAddrsSize; /* # rollovers allocated */
  void**                    RolloverAddrs;     /* Rollover event addresses */
} TSCRollover;

/* Event database */
typedef struct _db
{
  int                      ByteRev;             /* Byte-reverse trace data */
  int                      IndEventStruct;      /* The index in the LinuxEventStructSizeDefaults */
  void*                    TraceStart;          /* Address at which the raw trace starts */
  int                      isArch64;            /* 0 if not */
  off_t                    TraceSize;           /* Size of the trace */
  uint32_t                 BufferSize;          /* Size of buffers */

  trace_event_mask         EventMask;           /* The event mask */
  trace_event_mask         DetailsMask;         /* Are the event details logged */
  uint8_t                  LogCPUID;            /* Is the CPUID logged */
  uint32_t                 ArchType;            /* Architecture on which trace was generated */
  uint32_t                 ArchVariant;         /* Variant of the given architecture */
  uint32_t                 SystemType;          /* System on which trace was generated */
  uint8_t                  MajorVersion;        /* Major version number of trace format */
  uint8_t                  MinorVersion;        /* Minor version number of trace format */

  uint8_t                  UseTSC;              /* Using TSCs or time deltas */
  uint8_t                  FlightRecorder;      /* Is this a flight recorder trace? */
  uint8_t                  Preprocessing;       /* Currently preprocessing?  */
  uint32_t                 CurBufID;            /* Preprocessing this bufid  */
  uint32_t                 PrevTSC;             /* Previous TSC */
  uint32_t                 LastBufID;           /*  */
  ltt_timeval              LastBufStartTime;    /* Time at which trace ended */
  uint32_t                 LastBufStartTSC;     /*  */
  ltt_timeval              LastBufEndTime;      /* Time at which trace ended */
  uint32_t                 LastEventTSC;        /* Previous TSC */
  double                   LastGoodTSCPerUSec;
  uint32_t                 TSCRolloversSize;    /* Number of rollover structs allocated (1 per buffer) */
  TSCRollover*             TSCRollovers;        /* TSC rollover data (1 per buffer) */

  unsigned long int        Nb;                  /* Number of events */
  event                    FirstEvent;          /* First event in database */
  event                    LastEvent;           /* Last event in event list */
  ltt_timeval              StartTime;           /* Time at which trace started */
  ltt_timeval              EndTime;             /* Time at which trace ended */

  int                      SchedEventsExist;    /* There is at least one scheduling change in the trace */
  event                    FirstEventWithProc;  /* First event that belongs to a process */
  ltt_timeval              DrawStartTime;       /* Time at which we should start drawing */

  /* Custom event information */
  unsigned long int        NbCustom;            /* Number of custom events */
  customEventDesc          CustomEvents;        /* Circular list of custom events */
  customEventDesc          CustomHashTable[CUSTOM_EVENT_HASH_TABLE_SIZE];     /* Hash table of custom event descriptions */

  /* Information needed to understand trace */
  int                      MaxEventID;          /* Maximum event ID */
  char**                   EventOT;             /* Event Omit/Trace table */

  /* Functions */
  int                      (*EventStructSize) ();  /* Event structure size function */
  char*                    (*EventString) ();      /* Event string function */
  char*                    (*SyscallString) ();    /* Syscall string function */
  char*                    (*TrapString) ();       /* Trap string function */
} db;

/* Syscall tracking structure */
typedef struct _syscallInfo
{
  int                  ID;    /* The ID number of the system call */
  int                  Nb;    /* Number of times system call was called */
  int                  Depth; /* Depth of system call */
  ltt_timeval          Time;  /* Total amount of time passed in system call */
  struct _syscallInfo* Next;  /* Next system call */
} syscallInfo;

/* Structure type */
#define LINUX_PROCESS_OBJECT      0     /* type = "struct _process" */

/* Process description */
typedef struct _process
{
  /* DO NOT MOVE THIS FROM THE BEGINING OF THE STRUCT */
  /* Structure type  */
  int                  Type;          /* Type of structure */

  /* Graphic properties of process */
  int                  ListPos;       /* Position of process in process list */

  /* Properties */
  int                  PID;           /* PID of process */
  int                  PPID;          /* PID of parent */
  char*                Command;       /* Command given to start process */
  char*                CommandCurrent;/* Most recent Command exec()'ed in process */
  int                  CommandInherit;/* true if Command inheritted from parent */
  int                  NbSyscalls;    /* Number of system calls that occured */
  int                  NbTraps;       /* Number of traps that occured */
  int                  QFileRead;     /* Quantity of data read by process to files */
  int                  QFileWrite;    /* Quantity of data written by process to files */
  ltt_timeval          TimeOfBirth;   /* Time at which process was created (or forked */
  ltt_timeval          TimeOfFork;    /* Time at which process was forked */
  ltt_timeval          TimeOfExit;    /* Time at which process exited */
  ltt_timeval          TimeProcCode;  /* Time during which we were executing process code */
  ltt_timeval          TimeRuning;    /* Time during which this process had the control over the CPU */
  ltt_timeval          TimeIO;        /* Time spent waiting for I/O */

  /* Temporary data used during graph construction */
  ltt_timeval          ReportedSwitchTime; /* Time of occurence of last task switch with this task in */

  /* Variables used during analysis */
  int                  Depth;         /* Current syscall depth */
  syscallInfo*         Pending;       /* Syscalls not completely analyzed yet */
  int                  CtrlDepth;     /* Depth in control events */
  event                LastIOEvent;   /* Last I/O event */
  event                LastSchedIn;   /* Last time the task was scheduled in */

  /* Related structures */
  syscallInfo*         Syscalls;      /* Process specific syscall analysis  */
  struct _systemInfo*  System;        /* System to which this process belongs */

  /* List hooks */
  struct _process*     Next;          /* Next process in list */
  struct _process*     Children;      /* Pointer to first child */
  struct _process*     NextChild;     /* Next child in child list */ 
  struct _process*     PrevChild;     /* Previous child in child list */
  struct _process*     NextHash;      /* Next process in hash table entry */

  /* Generic hook */
  void*                Hook;          /* Data hook into process structure */

  char*                WallTimeGraph; /* ascii wall time graph */
} process;

/* number of columns in the x-axis of the ascii wall time graph */
#define WALL_TIME_GRAPH_MAX   100
#define WALL_TIME_GRAPH_MAX80  50

typedef uint8_t proc_hash;
#define PROC_HASH_SIZE   256

/* System database */
typedef struct _systemInfo
{
  /* Statistics */
  /*  Core events */
  unsigned long int  SyscallEntry; /* Number of system call entries */
  unsigned long int  SyscallExit;  /* Number of system call exits */
  unsigned long int  TrapEntry;    /* Number of trap entries */
  unsigned long int  TrapExit;     /* Number of trap exits */
  unsigned long int  IRQEntry;     /* Number of interrupt entries */
  unsigned long int  IRQExit;      /* Number of interrupt exits */
  unsigned long int  SchedChange;  /* Number of scheduler changes */
  unsigned long int  KernelTimer;  /* Number of times the kernel timer has been called */
  unsigned long int  BH;           /* Number of bottom-halves that occured */

  /*  Extend events set */
  unsigned long int  TimerExpire;  /* Number of timer expiries */
  unsigned long int  PageAlloc;    /* Number of page allocations */
  unsigned long int  PageFree;     /* Number of page frees */
  unsigned long int  PacketOut;    /* Number of outgoing packets */
  unsigned long int  PacketIn;     /* Number of incoming packets */

  /* Lists of system components */
  char*              Interrupts[MAX_NB_INTERRUPTS]; /* Interrupt descriptions */
  char*              DPM_States[MAX_NB_DPM_STATES]; /* DPM Operating States */
  process*           Processes;                     /* All the processes seen during trace */
  process*           ProcTree;                      /* The process tree */
  process            ProcHash[PROC_HASH_SIZE];      /* Hash table containing pointer to the processes */
  syscallInfo*       Syscalls;                      /* System-wide syscall analysis */  

  /* Trace type system specific data */
  void*              SystemSpec;
} systemInfo;

/* The trace manipulating functions */
uint8_t  DBEventID
              (db*               /* The database to which the event belongs */,
	       event*            /* The event for which the ID is sought */);
void     DBEventTime
              (db*               /* The database to which the event belongs */,
	       event*            /* The event for which the time is sought */,
	       ltt_timeval*      /* Pointer to the timeval structure to be filled */);
uint8_t  DBEventCPUID
              (db*               /* The database to which the event belongs */,
	       event*            /* The event for which the CPUID is sought */);
void*    DBEventStruct
              (db*               /* The database to which the event belongs */,
	       event*            /* The event for which the describing structure is sought */);
int      DBEventString
              (db*               /* The database to which the event belongs */,
	       systemInfo*       /* The system to which this event belongs */,
	       event*            /* The event for which the size is sought */,
	       char*             /* Pointer to the buffer to be filled with the description of the event */,
	       int               /* The size of the buffer */);
uint16_t DBEventSize
              (db*               /* The database to which the event belongs */,
	       event*            /* The event for which the size is sought */);
int      DBEventControlEntry
              (db*               /* The database to which the event belongs */,
	       event*            /* The event that is being tested for being a control event */,
               int               /* The PID of the process to which this event belongs */);
int      DBEventControlExit
              (db*               /* The database to which the event belongs */,
	       event*            /* The event that is being tested for being a control event */,
               int               /* The PID of the process to which this event belongs */);
int      DBEventControl
              (db*               /* The database to which the event belongs */,
	       event*            /* The event that is being tested for being a control event */,
	       systemInfo*       /* The system to which this event belongs */);
customEventDesc*
         DBEventGetCustomDescription
              (db*               /* The database to which the event belongs */,
	       event*            /* The event who's custom formatting data is looked for */);
char*    DBEventGetFormat
              (db*               /* The database to which the event belongs */,
	       event*            /* The event who's custom formatting data is looked for */,
	       int*              /* Pointer to int where to store the format type */);
char*    DBEventGetFormatByCustomID
              (db*               /* The database to which the events belongs */,
	       int               /* The custom event ID who's custom formatting data is looked for */,
	       int*              /* Pointer to int where to store the format type */);
char*    DBEventGetFormatByCustomType
              (db*               /* The database to which the events belongs */,
	       char*             /* The custom event type string who's custom formatting data is looked for */,
	       int*              /* Pointer to int where to store the format type */);
int      DBEventSetFormat
              (db*               /* The database to which the events belongs */,
	       event*            /* The event who's custom formatting data is to be set */,
	       int               /* The format type being set */,
	       char*             /* Custom event formatting string to be set */);
int      DBEventSetFormatByCustomID
              (db*               /* The database to which the events belongs */,
	       int               /* The custom event ID who's custom formatting data is to be set */,
	       int               /* The format type being set */,
	       char*             /* Custom event formatting string to be set */);
int      DBEventSetFormatByCustomType
              (db*               /* The database to which the events belongs */,
	       char*             /* The custom event type string who's custom formatting data is to be set */,
	       int               /* The format type being set */,
	       char*             /* Custom event formatting string to be set */);
process* DBEventProcess
              (db*               /* The database to which the event belongs */,
	       event*            /* The event for which the parent process is sought */,
	       systemInfo*       /* The system to which this event belongs */,
	       int               /* Should the time of occurence of the corresponding sched-change be remembered */);
void     DBEventDescription
              (db*               /* The database to which the event belongs */,
	       systemInfo*       /* The system to which this event belongs */,
	       event*            /* The event for which the description is sought */,
	       int               /* Boolean to indicate whether the description string is necessary */,
	       eventDescription* /* Pointer to an eventDescription structure to be filled */);
int      DBEventNext
              (db*               /* The database to which the event belongs */,
	       event*            /* The event for which the next event is sought */);
int      DBEventPrev
              (db*               /* The database to which the event belongs */,
	       event*            /* The event for which the previous event is sought */);

int      DBReadTrace
              (int         /* File from which data is to be read */,
	       db*         /* Database to be filled */);
void     DBProcessProcInfo
              (FILE*       /* File containing proc info */,
	       systemInfo* /* System description to be filled */);
void     DBBuildProcTree
              (systemInfo* /* System for which proc tree is to be built */);
void     DBBuildProcHash
              (systemInfo* /* System for which proc hash is to be built */);
process* DBGetProcByPID
              (int         /* PID of process to find */,
               systemInfo* /* System in which process is to be found */);
process* DBFindProcInTree
              (int         /* PID of process to find */,
               process*    /* Process tree in which to search */);
int      DBPreprocessTrace
              (systemInfo* /* System to be pre-processed */,
	       db*         /* Database of events */,
	       options*    /* The processing options */);
int      DBProcessTrace
              (systemInfo* /* System to be processed */,
	       db*         /* Database of events */,
	       options*    /* The processing options */);
void     DBFormatTimeInReadableString
              (char*       /* Pointer to buffer where formatted string is to be outputed */,
	       uint32_t    /* Number of seconds */,
	       uint32_t    /* Number of micro-seconds */);
void     DBPrintHeader
              (int         /* File to which data is to be printed */,
	       db*         /* Database of events */,
	       options*    /* Options */);
void     DBPrintTrace
              (int         /* File to which data is to be printed */,
	       systemInfo* /* System who's information is to be printed */,
	       db*         /* Database of events */,
	       options*    /* Options */);
process* DBCreateProcess
              (systemInfo* /* The system to which the process belongs */,
               int         /* The PID of the created process */,
               int         /* The PID of the parent process */);
db*      DBCreateDB
              (void);
systemInfo* DBCreateSystem
              (void);
void        DBDestroyTrace
              (systemInfo* /* The system to be destroyed */,
               db*         /* The event database to be destroyed */);


#endif /* __TRACE_TOOLKIT_EVENT_DB__ */
