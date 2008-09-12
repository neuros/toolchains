/*
 * EventOptions.h
 *
 * Copyright (C) 2001 Karim Yaghmour.
 *
 * This is distributed under GPL.
 *
 * Header for event options.
 *
 * History : 
 *    K.Y., 19/06/2001, Initial typing.
 *
 */

#ifndef __TRACE_TOOLKIT_EVENT_OPTIONS__
#define __TRACE_TOOLKIT_EVENT_OPTIONS__

#include <LTTTypes.h>

/* Options structure */
typedef struct _options
{
  int              Graphic;         /* Are we to go in graphic mode */
  int              Dump;            /* Do we only dump to output without interpreting anything */
  int              Omit;            /* Are we in "omit" mode */
  trace_event_mask OmitMask;        /* Mask with omitted events set to 1 */
  int              Trace;           /* Are we in trace mode */
  trace_event_mask TraceMask;       /* Mask with traced events set to 1 */
  int              TraceCPUID;      /* Trace only on CPUID */
  int              CPUID;           /* CPUID being traced */
  int              TracePID;        /* Trace only one PID */
  int              PID;             /* PID being traced */
  int              Summarize;       /* Is the summary requested */
  int              AcctSysCall;     /* Account time spent in system calls */
  int              WallTime;        /* Wall Time for each process */
  int              ForgetCPUID;     /* Do we print the CPUID */
  int              ForgetTime;      /* Do we print time */
  int              ForgetPID;       /* Do we print the PID */
  int              ForgetDataLen;   /* Do we print length of data entry */
  int              ForgetString;    /* Do we print description string */
  int              Benchmark;       /* Benchmark tool's internal calculations */
  int              FlightRecorder;  /* Is this a flight recorder trace? */
  char*            InputFileName;   /* Name of file used for input */
  char*            OutputFileName;  /* Name of file used for output */
  char*            ProcFileName;    /* Name of file contaning /proc information as of daemon's startup */
} options;

/* Functions */
options* CreateOptions
           (void);
void     DestroyOptions
           (options*  /* Options to be destroyed */);

#endif /* __TRACE_TOOLKIT_EVENT_OPTIONS__ */

