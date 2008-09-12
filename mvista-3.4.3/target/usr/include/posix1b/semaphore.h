/*-< SEMAPHORE.H >--------------------------------------------------*--------*/
/* POSIX.1b                   Version 1.0        (c) 1998  GARRET   *     ?  */
/* (POSIX.1b implementation for Linux)                              *   /\|  */
/*                                                                  *  /  \  */
/*                          Created:     25-Aug-98    K.A. Knizhnik * / [] \ */
/*                          Last update: 27-Aug-98    K.A. Knizhnik * GARRET */
/*------------------------------------------------------------------*--------*/
/* Semaphore interface                                              *        */
/*------------------------------------------------------------------*--------*/

#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#ifdef __cplusplus
extern "C" { 
#endif


typedef struct { 
    long  semkey;
    int   semid;
    int   initialized;
} sem_t;

#define SEM_VALUE_MAX  255

int sem_init(sem_t *sem, int pshared, unsigned int value);
 
sem_t *sem_open(const char *name, int oflag, ...);

int sem_post(sem_t *sem);

int sem_getvalue(sem_t *sem, int *sval);

int sem_wait(sem_t *sem);

int sem_trywait(sem_t *sem);

int sem_unlink(const char *name);

int sem_close(sem_t *sem);

int sem_destroy(sem_t *sem);


#ifdef __cplusplus
} 
#endif

#endif
