/*-< MQUEUE.H >-----------------------------------------------------*--------*/
/* POSIX.1b                   Version 1.0        (c) 1998  GARRET   *     ?  */
/* (POSIX.1b implementation for Linux)                              *   /\|  */
/*                                                                  *  /  \  */
/*                          Created:     25-Aug-98    K.A. Knizhnik * / [] \ */
/*                          Last update: 27-Aug-98    K.A. Knizhnik * GARRET */
/*------------------------------------------------------------------*--------*/
/* Message queue interface                                          *        */
/*------------------------------------------------------------------*--------*/

#ifndef __MQUEUE_H__
#define __MQUEUE_H__

#ifdef __cplusplus
extern "C" { 
#endif

#include <stdlib.h>

struct mq_notification_request { 
    int pid;
    int signo;
};

struct mq_descriptor { 
    int  msgid;
    int  fd;
    long flags;
    struct mq_notification_request* req;
};

typedef struct mq_descriptor* mqd_t; /* Process-level message queue descriptor */

#define  MQ_PRIO_MAX    (256)

#define  P4IPC_MSGSIZE	128  /* Implementation specified default size in bytes of
		       	      * a message.  Used when attr is not specified by 
     			      * the user in mq_open().
			      */
#define  P4IPC_MAXMSG	64   /* Implementation specified number of messages
			      * in a queue.  Used  when attr is not specified 
			      * by the user in mq_open().
			      */
typedef struct mq_attr {
    long mq_maxmsg;	/* max # of messages allowed in MQ */
    long mq_msgsize;  	/* max size of any one message */
    long mq_flags;	/* actions and state for mq operations */
    long mq_curmsgs;	/* # of messages currently on mq */
} mq_attr_t;

#ifndef SIGEV_SIGNAL

#define SIGEV_SIGNAL (0)
#define SIGEV_NONE   (1)

union sigval { 
    int   sival_int;
    void* sival_ptr;
};

struct sigevent { 
    int          sigev_notify;
    int          sigev_signo;
    union sigval sigev_value;
};

#endif

mqd_t mq_open(const char *name, int oflag, ...);

int mq_close(mqd_t mqdes);

int mq_unlink(const char *name);

int mq_notify(mqd_t mqdes, const struct sigevent *notification);

int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, 
	    unsigned int msg_prio);

ssize_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, 
		   unsigned int *msg_prio);

int mq_getattr(mqd_t mqdes, struct mq_attr *mqstat);

int mq_setattr(mqd_t mqdes, const struct mq_attr *mqstat,
	       struct mq_attr *omqstat);


#ifdef __cplusplus
} 
#endif

#endif
