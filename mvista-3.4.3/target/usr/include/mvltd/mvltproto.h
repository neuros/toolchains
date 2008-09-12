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
#ifndef MVLTPROTO_H
#define MVLTPROTO_H

#define MVLTPROTO_EOL  '\n'

#define MVLTPROTO_REQUEST_MAX_LEN    1024

#define MVLTPROTO_RESPONSE_MAX_LEN   4096

#define MVLTPROTO_RESPONSE_MAX_DATA_LEN   4076

#define MVLTPROTO_HEAD               "BOR"

#define MVLTPROTO_TAIL               "EOR"



typedef enum {
  MVLTPROTO_RESP_SUCCESS           = 100,
  //  MVLTPROTO_RESP_RESULT            = 101,
  MVLTPROTO_RESP_INCOMPLETERESULT  = 102,

  MVLTPROTO_RESP_TRYAGAIN          = 301,
  MVLTPROTO_RESP_TRYLATER          = 302,

  MVLTPROTO_RESP_TRANSFERERROR     = 401,
  MVLTPROTO_RESP_BADTOOLNAME       = 402,
  MVLTPROTO_RESP_BADTOOL           = 403,
  MVLTPROTO_RESP_NOTSUPPORTED      = 404,
  MVLTPROTO_RESP_INTERNALERROR     = 405,
  MVLTPROTO_RESP_BADREQUEST        = 406,
  MVLTPROTO_RESP_TOOLERROR         = 407,
  MVLTPROTO_RESP_WRONGREQUESTDATA  = 408,
  MVLTPROTO_RESP_AUTHREQIRED       = 409
} response_code_t;



typedef enum {
  MVLTPROTO_REQ_ACTIVATE        = 1,
  MVLTPROTO_REQ_COMMIT          = 2,
  MVLTPROTO_REQ_FETCHRESULT     = 3,
  MVLTPROTO_REQ_VERSION         = 4,
  MVLTPROTO_REQ_SAVELOG         = 13,
  MVLTPROTO_REQ_RELOAD          = 14,
  MVLTPROTO_REQ_TERMINATE       = 99
} request_code_t;




typedef struct {
  int    code;
  int data_len;
  char data[MVLTPROTO_REQUEST_MAX_LEN];
} request_t;



typedef struct {
  int code;
  int data_len;
  char * data;
} response_t;



int fetch_request( int fd, request_t * rq );

int send_response_head_log ( int fd, int log_fd, int code );

int send_response_tail_log ( int fd, int log_fd );

int send_simple_response_log ( int fd, int log_fd, int code, const char * msg );

int send_response_head ( int fd, int code );

int send_response_tail ( int fd  );

int send_simple_response ( int fd, int code, const char * msg );

const char * mvltproto_strerror(int mvlterrno);


enum { MVLTPROTO_EIOERROR  =  0,
       MVLTPROTO_EBADHEAD,
       MVLTPROTO_EBADCODE,
       MVLTPROTO_ETOOLONG,
       MVLTPROTO_ELOSTCON,       
       MVLTPROTO_MAX_ERRCODE };


extern int mvltproto_errno;

#endif
