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
#ifndef __MVLT_UTILS_H__
#define __MVLT_UTILS_H__

#define DEFAULT_LOGS_DIR   "/var/log/mvltd"
#define CLIENT_POLL_TIMEOUT 60000 // 1 min

int   safe_send (int sock, const char * buffer, size_t length, int flags);

int   safe_recv (int sock, char * buffer, size_t length, int flags);

int   safe_write (int sock, int log_fd, const char * buffer, size_t length);

int   safe_write_log (int sock, int log_fd, const char * buffer, size_t length, int store_log);

int   safe_read (int sock, char * buffer, size_t length);

int   safe_read_tv (int sock, char * buffer, size_t length, int timeout);

int   save_log (const char *toolname, int err_fd, char *data);

void  close_log(int log_fd); 
#endif
