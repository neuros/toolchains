/* 
 * © 2003 MontaVista Software, Inc.  All rights reserved.  
 * This program is licensed under the MontaVista Software, Inc. License 
 * Agreement ("License Agreement"), and is for the purposes of the License 
 * Agreement a MontaVista Licensed Deployment Program.  The License requires 
 * that you have a valid Product Subscription with MontaVista Software, Inc., 
 * or are a Named Contact with active access to the MontaVista Zone, or have 
 * a Software License Agreement with MontaVista Software, Inc. This program 
 * comes with no warranties other than those provided for in the Product 
 * Subscription agreement. The License Agreement grants you the right to 
 * install, modify and use the program.  You may distribute the object code 
 * and scripts for this program, but you have no right to distribute the 
 * source code for this program.
 */
#ifndef __MVLT_REMOTEFS_H__
#define __MVLT_REMOTEFS_H__

#include "mvltproto.h"

#define RS_SUCCESS            MVLTPROTO_RESP_SUCCESS
#define RS_INCOMPLETERESULT   MVLTPROTO_RESP_INCOMPLETERESULT

#define RS_BADREQUEST         MVLTPROTO_RESP_BADREQUEST
#define RS_TOOLERROR          MVLTPROTO_RESP_TOOLERROR
#define RS_WRONGREQUESTDATA   MVLTPROTO_RESP_WRONGREQUESTDATA

#define RQ_COMMIT             MVLTPROTO_REQ_COMMIT
#define RQ_FETCHRESULT        MVLTPROTO_REQ_FETCHRESULT
#define RQ_GETFILE            MVLTPROTO_REQ_FETCHRESULT+1
#define RQ_CHUNK              MVLTPROTO_REQ_FETCHRESULT+2
#define RQ_DONE               MVLTPROTO_REQ_FETCHRESULT+3
#define RQ_MKDIR              MVLTPROTO_REQ_FETCHRESULT+4
#define RQ_DELETE             MVLTPROTO_REQ_FETCHRESULT+5
#define RQ_STAT               MVLTPROTO_REQ_FETCHRESULT+6
#define RQ_SETMODE            MVLTPROTO_REQ_FETCHRESULT+7
#define RQ_TERMINATE          MVLTPROTO_REQ_TERMINATE

int send_files_list(int out_fd, char *dirname);

int get_file(int out_fd, char *filename);

int put_file(char *filename, int out_fd, int in_fd);

int make_directory(int out_fd, char *dirname);

int delete_resource(int out_fd, char *filename);

int stat_resource(int out_fd, char *path);

int set_mode(int out_fd, char *path, mode_t mode);
#endif
