/*****************************************************************************
 * not_quite_p_os.h - satisfies the references within the demo and validation
 *                    example code which would normally come from including
 *                    'psos.h' into a Wind River pSOS+ (R) source file.
 *  
 * Copyright ( C ) 2000, 2001  MontaVista Software Inc.
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

#define UCHAR           unsigned char
#define USHORT          unsigned short
#define UINT            unsigned int
#define ULONG           unsigned long

#define EV_ALL          ((ULONG)0)
#define EV_ANY          ((ULONG)2)
#define EV_NOWAIT       ((ULONG)1)
#define EV_WAIT         ((ULONG)0)

#define PT_DEL          ((ULONG)4)
#define PT_NODEL        ((ULONG)0)

#define Q_FIFO          ((ULONG)0)
#define Q_LIMIT         ((ULONG)4)
#define Q_NOLIMIT       ((ULONG)0)
#define Q_NOWAIT        ((ULONG)1)
#define Q_PRIOR         ((ULONG)2)
#define Q_WAIT          ((ULONG)0)

#define SM_FIFO         ((ULONG)0)
#define SM_PRIOR        ((ULONG)2)
#define SM_NOWAIT       ((ULONG)1)
#define SM_WAIT         ((ULONG)0)

#define T_LOCAL         ((ULONG)0)
#define T_NOPREEMPT     ((ULONG)1)
#define T_PREEMPT       ((ULONG)0)
#define T_NOTSLICE      ((ULONG)0)
#define T_TSLICE        ((ULONG)2)

ULONG ev_receive( ULONG mask, ULONG opt, ULONG max_wait, ULONG *captured );
ULONG ev_send( ULONG taskid, ULONG new_events );

ULONG pt_create( char name[4], void *paddr, void *laddr, ULONG length,
                 ULONG bsize, ULONG flags, ULONG *ptid, ULONG *nbuf );
ULONG pt_delete( ULONG ptid );
ULONG pt_getbuf( ULONG ptid, void **bufaddr );
ULONG pt_ident( char name[4], ULONG node, ULONG *ptid );
ULONG pt_retbuf( ULONG ptid, void *bufaddr );

ULONG q_broadcast( ULONG qid, ULONG msg[4], ULONG *count );
ULONG q_create( char name[4], ULONG qsize, ULONG opt, ULONG *qid );
ULONG q_delete( ULONG qid );
ULONG q_ident( char name[4], ULONG node, ULONG *qid );
ULONG q_receive( ULONG qid, ULONG opt, ULONG max_wait, ULONG msg[4] );
ULONG q_send( ULONG qid, ULONG msg[4] );
ULONG q_urgent( ULONG qid, ULONG msg[4] );

ULONG q_vcreate( char name[4], ULONG opt, ULONG qsize, ULONG msglen,
                 ULONG *qid );
ULONG q_vdelete( ULONG qid );
ULONG q_vident( char name[4], ULONG node, ULONG *qid );
ULONG q_vreceive( ULONG qid, ULONG opt, ULONG max_wait, void *msgbuf,
                  ULONG buflen, ULONG *msglen );
ULONG q_vsend( ULONG qid, void *msgbuf, ULONG msglen );
ULONG q_vurgent( ULONG qid, void *msgbuf, ULONG msglen );
ULONG q_vbroadcast( ULONG qid, void *msgbuf, ULONG msglen, ULONG *tasks );

ULONG sm_create( char name[4], ULONG count, ULONG opt, ULONG *smid );
ULONG sm_delete( ULONG smid );
ULONG sm_ident( char name[4], ULONG node, ULONG *smid );
ULONG sm_p( ULONG smid, ULONG opt, ULONG max_wait );
ULONG sm_v( ULONG smid );

ULONG t_create( char name[4], ULONG pri, ULONG sstack, ULONG ustack,
               ULONG mode, ULONG *tid );
ULONG t_delete( ULONG tid );
ULONG t_getreg( ULONG tid, ULONG regnum, ULONG *reg_value );
ULONG t_ident( char name[4], ULONG node, ULONG *tid );
ULONG t_mode( ULONG mask, ULONG new_flags, ULONG *old_flags );

ULONG t_resume( ULONG tid );
ULONG t_setpri( ULONG tid, ULONG pri, ULONG *oldpri );
ULONG t_setreg( ULONG tid, ULONG regnum, ULONG reg_value );
ULONG t_start( ULONG tid, ULONG mode, void (*task)(), ULONG parms[4] );
ULONG t_suspend( ULONG tid );

ULONG tm_wkafter( ULONG interval );

