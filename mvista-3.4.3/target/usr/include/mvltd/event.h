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
#ifndef __MVLT_EVENT_H__
#define __MVLT_EVENT_H__

int send_event (int port, char *event_name, char *event_data);
#endif
