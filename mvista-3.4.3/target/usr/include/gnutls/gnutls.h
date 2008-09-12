/*
 *  Copyright (C) 2000,2001,2002,2003 Nikos Mavroyanopoulos
 *  Copyright (C) 2004 Free Software Foundation
 *
 *  This file is part of GNUTLS.
 *
 *  The GNUTLS library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public   
 *  License as published by the Free Software Foundation; either 
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 */

/* This file contains the types and prototypes for all the
 * high level functionality of gnutls main library. For the
 * extra functionality (which is under the GNU GPL license) check
 * the gnutls/extra.h header. The openssl compatibility layer is
 * in gnutls/openssl.h.
 *
 * The low level cipher functionality is in libgcrypt. Check
 * gcrypt.h
 */


#ifndef GNUTLS_H
# define GNUTLS_H

#ifdef __cplusplus
extern "C" {
#endif

#define LIBGNUTLS_VERSION "1.0.16"

#ifndef HAVE_SIZE_T
# define HAVE_SIZE_T
#include <sys/types.h>
#endif

#ifndef HAVE_SSIZE_T
# define HAVE_SSIZE_T

#endif

#ifndef HAVE_TIME_T
# define HAVE_TIME_T
#include <time.h>
#endif

#define GNUTLS_CIPHER_RIJNDAEL_128_CBC GNUTLS_CIPHER_AES_128_CBC
#define GNUTLS_CIPHER_RIJNDAEL_256_CBC GNUTLS_CIPHER_AES_256_CBC
#define GNUTLS_CIPHER_RIJNDAEL_CBC GNUTLS_CIPHER_AES_128_CBC
#define GNUTLS_CIPHER_ARCFOUR GNUTLS_CIPHER_ARCFOUR_128

typedef enum gnutls_cipher_algorithm { GNUTLS_CIPHER_NULL=1, 
	GNUTLS_CIPHER_ARCFOUR_128, GNUTLS_CIPHER_3DES_CBC, GNUTLS_CIPHER_AES_128_CBC, 
	GNUTLS_CIPHER_AES_256_CBC, GNUTLS_CIPHER_ARCFOUR_40
} gnutls_cipher_algorithm;

typedef enum gnutls_kx_algorithm { GNUTLS_KX_RSA=1, GNUTLS_KX_DHE_DSS, 
	GNUTLS_KX_DHE_RSA, GNUTLS_KX_ANON_DH, GNUTLS_KX_SRP,
	GNUTLS_KX_RSA_EXPORT, GNUTLS_KX_SRP_RSA, GNUTLS_KX_SRP_DSS
} gnutls_kx_algorithm;

typedef enum gnutls_params_type { GNUTLS_PARAMS_RSA_EXPORT=1,
	GNUTLS_PARAMS_DH 
} gnutls_params_type;

typedef enum gnutls_credentials_type { GNUTLS_CRD_CERTIFICATE=1, GNUTLS_CRD_ANON, GNUTLS_CRD_SRP } gnutls_credentials_type;

typedef enum gnutls_mac_algorithm { GNUTLS_MAC_NULL=1, 
	GNUTLS_MAC_MD5, GNUTLS_MAC_SHA, GNUTLS_MAC_RMD160
} gnutls_mac_algorithm;

/* The enumerations here should have the same value with gnutls_mac_algorithm.
 */
typedef enum gnutls_digest_algorithm { GNUTLS_DIG_NULL=1, GNUTLS_DIG_MD5, 
	GNUTLS_DIG_SHA, GNUTLS_DIG_RIPEMD160 
} gnutls_digest_algorithm;

/* exported for other gnutls headers. This is the maximum number
 * of algorithms (ciphers, kx or macs). 
 */
#define GNUTLS_MAX_ALGORITHM_NUM 16

#define GNUTLS_COMP_ZLIB GNUTLS_COMP_DEFLATE
typedef enum gnutls_compression_method { GNUTLS_COMP_NULL=1, 
	GNUTLS_COMP_DEFLATE, 
	GNUTLS_COMP_LZO /* only available if gnutls-extra has been initialized 
                         */
} gnutls_compression_method;

typedef enum gnutls_connection_end { GNUTLS_SERVER=1, GNUTLS_CLIENT } gnutls_connection_end;

typedef enum gnutls_alert_level { GNUTLS_AL_WARNING=1, GNUTLS_AL_FATAL } gnutls_alert_level;

typedef enum gnutls_alert_description { GNUTLS_A_CLOSE_NOTIFY, 
	GNUTLS_A_UNEXPECTED_MESSAGE=10, GNUTLS_A_BAD_RECORD_MAC=20,
	GNUTLS_A_DECRYPTION_FAILED, GNUTLS_A_RECORD_OVERFLOW, GNUTLS_A_DECOMPRESSION_FAILURE=30,
	GNUTLS_A_HANDSHAKE_FAILURE=40, GNUTLS_A_SSL3_NO_CERTIFICATE=41,
	GNUTLS_A_BAD_CERTIFICATE=42, GNUTLS_A_UNSUPPORTED_CERTIFICATE,
	GNUTLS_A_CERTIFICATE_REVOKED, GNUTLS_A_CERTIFICATE_EXPIRED, GNUTLS_A_CERTIFICATE_UNKNOWN,
	GNUTLS_A_ILLEGAL_PARAMETER, GNUTLS_A_UNKNOWN_CA, GNUTLS_A_ACCESS_DENIED, GNUTLS_A_DECODE_ERROR=50,
	GNUTLS_A_DECRYPT_ERROR, GNUTLS_A_EXPORT_RESTRICTION=60, GNUTLS_A_PROTOCOL_VERSION=70,
	GNUTLS_A_INSUFFICIENT_SECURITY, GNUTLS_A_INTERNAL_ERROR=80, GNUTLS_A_USER_CANCELED=90,
	GNUTLS_A_NO_RENEGOTIATION=100, GNUTLS_A_UNSUPPORTED_EXTENSION=110,
	GNUTLS_A_CERTIFICATE_UNOBTAINABLE=111, GNUTLS_A_UNRECOGNIZED_NAME=112,
	GNUTLS_A_UNKNOWN_SRP_USERNAME=120, GNUTLS_A_MISSING_SRP_USERNAME=121
} gnutls_alert_description;

typedef enum gnutls_handshake_description { GNUTLS_HANDSHAKE_HELLO_REQUEST, 
	GNUTLS_HANDSHAKE_CLIENT_HELLO, GNUTLS_HANDSHAKE_SERVER_HELLO,
	GNUTLS_HANDSHAKE_CERTIFICATE_PKT=11, GNUTLS_HANDSHAKE_SERVER_KEY_EXCHANGE,
	GNUTLS_HANDSHAKE_CERTIFICATE_REQUEST, GNUTLS_HANDSHAKE_SERVER_HELLO_DONE,
	GNUTLS_HANDSHAKE_CERTIFICATE_VERIFY, GNUTLS_HANDSHAKE_CLIENT_KEY_EXCHANGE,
	GNUTLS_HANDSHAKE_FINISHED=20 
} gnutls_handshake_description;

/* Note that the status bits have different meanings
 * in openpgp keys and x.509 certificate verification.
 */
typedef enum gnutls_certificate_status { 
	GNUTLS_CERT_INVALID=2, /* will be set if the certificate
				* was not verified.
				*/
	GNUTLS_CERT_REVOKED=32, /* in X.509 this will be set only if CRLs are checked
				 */

	/* Those are extra information about the verification
	 * process. Will be set only if the certificate was 
	 * not verified.
	 */
	GNUTLS_CERT_SIGNER_NOT_FOUND=64,
	GNUTLS_CERT_SIGNER_NOT_CA=128
} gnutls_certificate_status;

typedef enum gnutls_certificate_request { GNUTLS_CERT_IGNORE, GNUTLS_CERT_REQUEST=1, GNUTLS_CERT_REQUIRE } gnutls_certificate_request;

typedef enum gnutls_openpgp_key_status { GNUTLS_OPENPGP_KEY, 
	GNUTLS_OPENPGP_KEY_FINGERPRINT
} gnutls_openpgp_key_status;

typedef enum gnutls_close_request { GNUTLS_SHUT_RDWR=0, GNUTLS_SHUT_WR=1 } gnutls_close_request;

#define GNUTLS_TLS1 GNUTLS_TLS1_0
typedef enum gnutls_protocol_version { GNUTLS_SSL3=1, GNUTLS_TLS1_0,
	GNUTLS_TLS1_1 } gnutls_protocol_version;

typedef enum gnutls_certificate_type { GNUTLS_CRT_X509=1, GNUTLS_CRT_OPENPGP 
} gnutls_certificate_type;

typedef enum gnutls_x509_crt_fmt { GNUTLS_X509_FMT_DER, 
	GNUTLS_X509_FMT_PEM } gnutls_x509_crt_fmt;

typedef enum gnutls_pk_algorithm { GNUTLS_PK_RSA = 1, GNUTLS_PK_DSA,
	GNUTLS_PK_UNKNOWN = 0xff
} gnutls_pk_algorithm;

const char *gnutls_pk_algorithm_get_name( gnutls_pk_algorithm algorithm);

typedef enum gnutls_sign_algorithm { GNUTLS_SIGN_RSA_SHA = 1, GNUTLS_SIGN_DSA_SHA,
	GNUTLS_SIGN_RSA_MD5, GNUTLS_SIGN_RSA_MD2,
	GNUTLS_SIGN_UNKNOWN = 0xff
} gnutls_sign_algorithm;

const char *gnutls_sign_algorithm_get_name( gnutls_sign_algorithm algorithm);

/* If you want to change this, then also change the 
 * define in gnutls_int.h, and recompile.
 */
typedef void * gnutls_transport_ptr;

struct gnutls_session_int;
typedef struct gnutls_session_int* gnutls_session;

struct gnutls_dh_params_int;
typedef struct gnutls_dh_params_int* gnutls_dh_params;

struct gnutls_rsa_params_int;
typedef struct gnutls_rsa_params_int* gnutls_rsa_params;

typedef struct {
	unsigned char * data;
	unsigned int size;
} gnutls_datum;

#define gnutls_const_datum gnutls_datum

/* internal functions */

int gnutls_init(gnutls_session * session, gnutls_connection_end con_end);
void gnutls_deinit(gnutls_session session);
void _gnutls_deinit(gnutls_session session);
int gnutls_bye( gnutls_session session, gnutls_close_request how);

int gnutls_handshake( gnutls_session session);
int gnutls_rehandshake( gnutls_session session);

gnutls_alert_description gnutls_alert_get( gnutls_session session);
int 		 gnutls_alert_send( gnutls_session, gnutls_alert_level, gnutls_alert_description);
int 		 gnutls_alert_send_appropriate(  gnutls_session session, int err);
const char*	 gnutls_alert_get_name( gnutls_alert_description alert);

/* get information on the current session */
gnutls_cipher_algorithm		gnutls_cipher_get( gnutls_session session);
gnutls_kx_algorithm 		gnutls_kx_get( gnutls_session session);
gnutls_mac_algorithm		gnutls_mac_get( gnutls_session session);
gnutls_compression_method	gnutls_compression_get( gnutls_session session);
gnutls_certificate_type		gnutls_certificate_type_get( gnutls_session session);

size_t gnutls_cipher_get_key_size( gnutls_cipher_algorithm algorithm);

/* the name of the specified algorithms */
const char *gnutls_cipher_get_name( gnutls_cipher_algorithm);
const char *gnutls_mac_get_name( gnutls_mac_algorithm);
const char *gnutls_compression_get_name( gnutls_compression_method);
const char *gnutls_kx_get_name( gnutls_kx_algorithm algorithm);
const char *gnutls_certificate_type_get_name( gnutls_certificate_type type);


/* error functions */
int gnutls_error_is_fatal( int error);
int gnutls_error_to_alert( int err, int* level);

void gnutls_perror( int error);
const char* gnutls_strerror( int error);

/* Semi-internal functions.
 */
void gnutls_handshake_set_private_extensions(gnutls_session session, int allow);
gnutls_handshake_description gnutls_handshake_get_last_out( gnutls_session session);
gnutls_handshake_description gnutls_handshake_get_last_in( gnutls_session session);

/* Record layer functions.
 */
ssize_t gnutls_record_send( gnutls_session session, const void *data, size_t sizeofdata);
ssize_t gnutls_record_recv( gnutls_session session, void *data, size_t sizeofdata);
#define gnutls_read gnutls_record_recv
#define gnutls_write gnutls_record_send

int gnutls_record_get_direction(gnutls_session session);

size_t gnutls_record_get_max_size( gnutls_session session);
ssize_t gnutls_record_set_max_size( gnutls_session session, size_t size);

size_t gnutls_record_check_pending(gnutls_session session);

/* TLS Extensions */

typedef enum gnutls_server_name_type { GNUTLS_NAME_DNS=1 
} gnutls_server_name_type;

int gnutls_server_name_set(gnutls_session session,
	gnutls_server_name_type type,
	const void *name, size_t name_length);

int gnutls_server_name_get(gnutls_session session, 
	void *data, size_t *data_length,
	unsigned int * type, int unsigned indx);

/* functions to set priority of cipher suites 
 */
int gnutls_cipher_set_priority( gnutls_session session, const int*);
int gnutls_mac_set_priority( gnutls_session session, const int*);
int gnutls_compression_set_priority( gnutls_session session, const int*);
int gnutls_kx_set_priority( gnutls_session session, const int*);
int gnutls_protocol_set_priority( gnutls_session session, const int*);
int gnutls_certificate_type_set_priority( gnutls_session session, const int*);

/* if you just want some defaults, use the following.
 */
int gnutls_set_default_priority(gnutls_session session);
int gnutls_set_default_export_priority(gnutls_session session);

/* Returns the name of a cipher suite */
const char *gnutls_cipher_suite_get_name(gnutls_kx_algorithm kx_algorithm,
	gnutls_cipher_algorithm cipher_algorithm, gnutls_mac_algorithm mac_algorithm);

/* get the currently used protocol version */
gnutls_protocol_version gnutls_protocol_get_version(gnutls_session session);

const char *gnutls_protocol_get_name(gnutls_protocol_version version);


/* get/set session 
 */
int gnutls_session_set_data( gnutls_session session, const void* session_data, size_t session_data_size);
int gnutls_session_get_data( gnutls_session session, void* session_data, size_t *session_data_size);
/* returns the session ID */
#define GNUTLS_MAX_SESSION_ID 32
int gnutls_session_get_id( gnutls_session session, void* session_id, size_t *session_id_size);

/* checks if this session is a resumed one 
 */
int gnutls_session_is_resumed(gnutls_session session);

typedef int (*gnutls_db_store_func)(void*, gnutls_datum key, gnutls_datum data);
typedef int (*gnutls_db_remove_func)(void*, gnutls_datum key);
typedef gnutls_datum (*gnutls_db_retr_func)(void*, gnutls_datum key);

void gnutls_db_set_cache_expiration( gnutls_session session, int seconds);

void gnutls_db_remove_session( gnutls_session session);
void gnutls_db_set_retrieve_function( gnutls_session, gnutls_db_retr_func);
void gnutls_db_set_remove_function( gnutls_session, gnutls_db_remove_func);
void gnutls_db_set_store_function( gnutls_session, gnutls_db_store_func);
void gnutls_db_set_ptr( gnutls_session, void* db_ptr);
void* gnutls_db_get_ptr( gnutls_session);
int  gnutls_db_check_entry( gnutls_session session, gnutls_datum session_entry);

void gnutls_handshake_set_max_packet_length( gnutls_session session, int max);

/* returns libgnutls version (call it with a NULL argument) 
 */
const char* gnutls_check_version( const char*);

/* Functions for setting/clearing credentials 
 */
int gnutls_credentials_clear( gnutls_session session);

/* cred is a structure defined by the kx algorithm 
 */
int gnutls_credentials_set( gnutls_session, gnutls_credentials_type type, void* cred);
#define gnutls_cred_set	gnutls_credentials_set

/* Credential structures for SRP - used in gnutls_credentials_set(); */

struct DSTRUCT;
typedef struct DSTRUCT* gnutls_certificate_credentials;
typedef gnutls_certificate_credentials gnutls_certificate_server_credentials;
typedef gnutls_certificate_credentials gnutls_certificate_client_credentials;

typedef struct DSTRUCT* gnutls_anon_server_credentials;
typedef struct DSTRUCT* gnutls_anon_client_credentials;

void gnutls_anon_free_server_credentials( gnutls_anon_server_credentials sc);
int gnutls_anon_allocate_server_credentials( gnutls_anon_server_credentials *sc);

void gnutls_anon_set_server_dh_params( gnutls_anon_server_credentials res, gnutls_dh_params dh_params);

void gnutls_anon_free_client_credentials( gnutls_anon_client_credentials sc);
int gnutls_anon_allocate_client_credentials( gnutls_anon_client_credentials *sc);

/* CERTFILE is an x509 certificate in PEM form.
 * KEYFILE is a pkcs-1 private key in PEM form (for RSA keys).
 */
void gnutls_certificate_free_credentials( gnutls_certificate_credentials sc);
int gnutls_certificate_allocate_credentials( gnutls_certificate_credentials *sc);

void gnutls_certificate_free_keys(gnutls_certificate_credentials sc);
void gnutls_certificate_free_cas(gnutls_certificate_credentials sc);
void gnutls_certificate_free_ca_names(gnutls_certificate_credentials sc);
void gnutls_certificate_free_crls(gnutls_certificate_credentials sc);

void gnutls_certificate_set_dh_params(gnutls_certificate_credentials res, gnutls_dh_params);
void gnutls_certificate_set_rsa_export_params(gnutls_certificate_credentials res, gnutls_rsa_params rsa_params);
void gnutls_certificate_set_verify_flags(gnutls_certificate_credentials res, unsigned int flags);


int gnutls_certificate_set_x509_trust_file( gnutls_certificate_credentials res, const char* CAFILE, 
	gnutls_x509_crt_fmt);
int gnutls_certificate_set_x509_trust_mem(gnutls_certificate_credentials res, 
	const gnutls_datum *CA, gnutls_x509_crt_fmt);

int gnutls_certificate_set_x509_crl_file(gnutls_certificate_credentials res, 
		const char *crlfile, gnutls_x509_crt_fmt type);
int gnutls_certificate_set_x509_crl_mem(gnutls_certificate_credentials res, 
	const gnutls_datum *CRL, gnutls_x509_crt_fmt type);

int gnutls_certificate_set_x509_key_file( gnutls_certificate_credentials res, 
	const char *CERTFILE, const char* KEYFILE, gnutls_x509_crt_fmt);
int gnutls_certificate_set_x509_key_mem(gnutls_certificate_credentials res, 
	const gnutls_datum* CERT, const gnutls_datum* KEY,
	gnutls_x509_crt_fmt);


/* New functions to allow setting already parsed X.509 stuff.
 */
struct gnutls_x509_privkey_int;
typedef struct gnutls_x509_privkey_int* gnutls_x509_privkey;

struct gnutls_x509_crl_int;
typedef struct gnutls_x509_crl_int* gnutls_x509_crl;

struct gnutls_x509_crt_int;
typedef struct gnutls_x509_crt_int* gnutls_x509_crt;

int gnutls_certificate_set_x509_key(gnutls_certificate_credentials res, 
	gnutls_x509_crt *cert_list, int cert_list_size,
	gnutls_x509_privkey key);
int gnutls_certificate_set_x509_trust(gnutls_certificate_credentials res, 
	gnutls_x509_crt * ca_list, int ca_list_size);
int gnutls_certificate_set_x509_crl(gnutls_certificate_credentials res, 
	gnutls_x509_crl* crl_list, int crl_list_size);


/* global state functions 
 */
int gnutls_global_init(void);
void gnutls_global_deinit(void);

typedef void* (*gnutls_alloc_function)(size_t);
typedef void* (*gnutls_calloc_function)(size_t, size_t);
typedef void (*gnutls_free_function)(void*);
typedef void* (*gnutls_realloc_function)(void*, size_t);

void gnutls_global_set_mem_functions( 
	gnutls_alloc_function, gnutls_alloc_function,
	int (*gnutls_is_secure_func)(const void*), gnutls_realloc_function,
	gnutls_free_function);

/* For use in callbacks */
extern gnutls_alloc_function gnutls_malloc;
extern gnutls_calloc_function gnutls_calloc;
extern gnutls_free_function gnutls_free;
extern char* (*gnutls_strdup)( const char*);

typedef void (*gnutls_log_func)( int, const char*);
void gnutls_global_set_log_function( gnutls_log_func log_func);
void gnutls_global_set_log_level( int level);

/* Diffie Hellman parameter handling.
 */
int gnutls_dh_params_init( gnutls_dh_params*);
void gnutls_dh_params_deinit( gnutls_dh_params);
int gnutls_dh_params_import_raw(gnutls_dh_params dh_params, const gnutls_datum *prime,
	const gnutls_datum* generator);
int gnutls_dh_params_import_pkcs3(gnutls_dh_params params, 
	const gnutls_datum * pkcs3_params, gnutls_x509_crt_fmt format);
int gnutls_dh_params_generate2(gnutls_dh_params params, int bits);
int gnutls_dh_params_export_pkcs3( gnutls_dh_params params,
	gnutls_x509_crt_fmt format, unsigned char* params_data, size_t* params_data_size);
int gnutls_dh_params_export_raw(gnutls_dh_params params,
	gnutls_datum * prime, gnutls_datum * generator, unsigned int *bits);
int gnutls_dh_params_cpy(gnutls_dh_params dst, gnutls_dh_params src);


/* RSA params 
 */
int gnutls_rsa_params_init(gnutls_rsa_params * rsa_params);
void gnutls_rsa_params_deinit(gnutls_rsa_params rsa_params);
int gnutls_rsa_params_cpy(gnutls_rsa_params dst, gnutls_rsa_params src);
int gnutls_rsa_params_import_raw(gnutls_rsa_params rsa_params, 
	const gnutls_datum *m, const gnutls_datum *e,
	const gnutls_datum *d, const gnutls_datum *p, 
	const gnutls_datum *q, const gnutls_datum *u);
int gnutls_rsa_params_generate2(gnutls_rsa_params params, int bits);
int gnutls_rsa_params_export_raw(gnutls_rsa_params params,
	gnutls_datum * m, gnutls_datum *e,
	gnutls_datum *d, gnutls_datum *p, gnutls_datum* q, 
	gnutls_datum* u, unsigned int *bits);
int gnutls_rsa_params_export_pkcs1( gnutls_rsa_params params,
        gnutls_x509_crt_fmt format, unsigned char* params_data,
        size_t* params_data_size);
int gnutls_rsa_params_import_pkcs1(gnutls_rsa_params params,
        const gnutls_datum * pkcs1_params, gnutls_x509_crt_fmt format);

/* Session stuff
 */
typedef ssize_t (*gnutls_pull_func)(gnutls_transport_ptr, void*, size_t);
typedef ssize_t (*gnutls_push_func)(gnutls_transport_ptr, const void*, size_t);
void gnutls_transport_set_ptr(gnutls_session session, gnutls_transport_ptr ptr);
void gnutls_transport_set_ptr2(gnutls_session session, gnutls_transport_ptr recv_ptr,
	gnutls_transport_ptr send_ptr);

gnutls_transport_ptr gnutls_transport_get_ptr(gnutls_session session);
void gnutls_transport_get_ptr2(gnutls_session session,
	gnutls_transport_ptr *recv_ptr,
	gnutls_transport_ptr *send_ptr);

void gnutls_transport_set_lowat( gnutls_session session, int num);


void gnutls_transport_set_push_function( gnutls_session, gnutls_push_func push_func);
void gnutls_transport_set_pull_function( gnutls_session, gnutls_pull_func pull_func);

/* session specific 
 */
void gnutls_session_set_ptr(gnutls_session session, void* ptr);
void* gnutls_session_get_ptr(gnutls_session session);

void gnutls_openpgp_send_key(gnutls_session session, gnutls_openpgp_key_status status);

/* fingerprint 
 * Actually this function returns the hash of the given data.
 */
int gnutls_fingerprint(gnutls_digest_algorithm algo, const gnutls_datum* data, 
	void* result, size_t* result_size);

#ifndef GNUTLS_UI_H
# define GNUTLS_UI_H

typedef enum gnutls_x509_subject_alt_name {
	GNUTLS_SAN_DNSNAME = 1, GNUTLS_SAN_RFC822NAME,
	GNUTLS_SAN_URI, GNUTLS_SAN_IPADDRESS
} gnutls_x509_subject_alt_name;

# ifdef LIBGNUTLS_VERSION	/* These are defined only in gnutls.h */

/* Callback prototypes for the certificate authentication
 * callbacks.
 */
typedef int gnutls_certificate_client_select_function(gnutls_session,
      const gnutls_datum * client_cert, int ncerts,
      const gnutls_datum * req_ca_cert, int nreqs);
typedef int gnutls_certificate_server_select_function(gnutls_session,
      const gnutls_datum * server_certs, int ncerts);


struct gnutls_openpgp_key_int;
typedef struct gnutls_openpgp_key_int *gnutls_openpgp_key;

struct gnutls_openpgp_privkey_int;
typedef struct gnutls_openpgp_privkey_int *gnutls_openpgp_privkey;

typedef struct gnutls_retr_st {
	gnutls_certificate_type type;
	union cert {
		gnutls_x509_crt *x509;
		gnutls_openpgp_key pgp;
	} cert;
	unsigned int ncerts;		/* one for pgp keys */

	union key {
		gnutls_x509_privkey x509;
		gnutls_openpgp_privkey pgp;
	} key;

	unsigned int deinit_all;	/* if non zero all keys will be deinited */
} gnutls_retr_st;

typedef int gnutls_certificate_client_retrieve_function(gnutls_session,
	const gnutls_datum* req_ca_rdn, int nreqs,
	const gnutls_pk_algorithm* pk_algos, int pk_algos_length,
	gnutls_retr_st *);
typedef int gnutls_certificate_server_retrieve_function(gnutls_session,
	gnutls_retr_st *);


/* Functions that allow AUTH_INFO structures handling
 */

gnutls_credentials_type gnutls_auth_get_type(gnutls_session session);

/* DH */

void gnutls_dh_set_prime_bits(gnutls_session session, int bits);
int gnutls_dh_get_prime_bits(gnutls_session);
int gnutls_dh_get_secret_bits(gnutls_session);
int gnutls_dh_get_peers_public_bits(gnutls_session);

/* RSA */
int gnutls_rsa_export_get_modulus_bits(gnutls_session session);

/* X509PKI */

#include <gnutls/compat8.h>

/* those should replaced with the functions that follow.
 */
void gnutls_certificate_client_set_select_function(gnutls_session,
	gnutls_certificate_client_select_function *) DEPRECATED;
void gnutls_certificate_server_set_select_function(gnutls_session,
	gnutls_certificate_server_select_function *) DEPRECATED;

/* These are set on the credentials structure.
 */
void gnutls_certificate_client_set_retrieve_function(gnutls_certificate_client_credentials,
	gnutls_certificate_client_retrieve_function *);
void gnutls_certificate_server_set_retrieve_function(gnutls_certificate_server_credentials,
	gnutls_certificate_server_retrieve_function *);

void gnutls_certificate_server_set_request(gnutls_session,
	gnutls_certificate_request);

/* X.509 certificate handling functions 
 */

int gnutls_pkcs3_extract_dh_params(const gnutls_datum * params,
	gnutls_x509_crt_fmt format, gnutls_datum * prime,
	gnutls_datum * generator, int *prime_bits);
int gnutls_pkcs3_export_dh_params(const gnutls_datum * prime,
	const gnutls_datum * generator, gnutls_x509_crt_fmt format,
	unsigned char *params_data, int *params_data_size);

/* get data from the session 
 */
const gnutls_datum *gnutls_certificate_get_peers(gnutls_session,
	unsigned int *list_size);
const gnutls_datum *gnutls_certificate_get_ours(gnutls_session session);

time_t gnutls_certificate_activation_time_peers(gnutls_session session);
time_t gnutls_certificate_expiration_time_peers(gnutls_session session);

int gnutls_certificate_client_get_request_status(gnutls_session);
int gnutls_certificate_verify_peers(gnutls_session);

int gnutls_pem_base64_encode(const char *header, const gnutls_datum * data,
	char *result, size_t * result_size);
int gnutls_pem_base64_decode(const char *header,
	const gnutls_datum * b64_data,
	unsigned char *result, size_t * result_size);

int gnutls_pem_base64_encode_alloc(const char *header,
	const gnutls_datum * data, gnutls_datum * result);
int gnutls_pem_base64_decode_alloc(const char *header,
	const gnutls_datum * b64_data, gnutls_datum * result);

/* key_usage will be an OR of the following values:
 */
#define GNUTLS_KEY_DIGITAL_SIGNATURE            128	/* when the key is to be
							 * used for signing.
							 */
#define GNUTLS_KEY_NON_REPUDIATION              64
#define GNUTLS_KEY_KEY_ENCIPHERMENT             32	/* when the key is to be
							 * used for encryption.
							 */
#define GNUTLS_KEY_DATA_ENCIPHERMENT            16
#define GNUTLS_KEY_KEY_AGREEMENT                8
#define GNUTLS_KEY_KEY_CERT_SIGN                4
#define GNUTLS_KEY_CRL_SIGN                     2
#define GNUTLS_KEY_ENCIPHER_ONLY                1
#define GNUTLS_KEY_DECIPHER_ONLY                32768

typedef struct gnutls_params_st {
	gnutls_params_type type;
	union params {
		gnutls_dh_params dh;
		gnutls_rsa_params rsa_export;
	} params;
	int deinit;
} gnutls_params_st;

typedef int gnutls_params_function(gnutls_session, gnutls_params_type,
	gnutls_params_st*);

void gnutls_certificate_set_params_function(gnutls_certificate_credentials res, 
	gnutls_params_function* func);
void gnutls_anon_set_params_function(gnutls_certificate_credentials res, 
	gnutls_params_function* func);



# endif				/* LIBGNUTLS_VERSION */

#endif				/* GNUTLS_UI_H */


#ifndef GNUTLS_ERRORS_IH
# define GNUTLS_ERRORS_IH

/* Gnutls error codes. The mapping to a TLS alert is also shown in
 * comments.
 */

#define GNUTLS_E_SUCCESS 0
#define	GNUTLS_E_UNKNOWN_COMPRESSION_ALGORITHM -3
#define	GNUTLS_E_UNKNOWN_CIPHER_TYPE -6
#define	GNUTLS_E_LARGE_PACKET -7
#define GNUTLS_E_UNSUPPORTED_VERSION_PACKET -8 /* GNUTLS_A_PROTOCOL_VERSION */
#define GNUTLS_E_UNEXPECTED_PACKET_LENGTH -9 /* GNUTLS_A_RECORD_OVERFLOW */
#define GNUTLS_E_INVALID_SESSION -10
#define GNUTLS_E_FATAL_ALERT_RECEIVED -12
#define GNUTLS_E_UNEXPECTED_PACKET -15 /* GNUTLS_A_UNEXPECTED_MESSAGE */
#define GNUTLS_E_WARNING_ALERT_RECEIVED -16
#define GNUTLS_E_ERROR_IN_FINISHED_PACKET -18
#define GNUTLS_E_UNEXPECTED_HANDSHAKE_PACKET -19
#define	GNUTLS_E_UNKNOWN_CIPHER_SUITE -21 /* GNUTLS_A_HANDSHAKE_FAILURE */
#define	GNUTLS_E_UNWANTED_ALGORITHM -22
#define	GNUTLS_E_MPI_SCAN_FAILED -23
#define GNUTLS_E_DECRYPTION_FAILED -24 /* GNUTLS_A_DECRYPTION_FAILED, GNUTLS_A_BAD_RECORD_MAC */
#define GNUTLS_E_MEMORY_ERROR -25
#define GNUTLS_E_DECOMPRESSION_FAILED -26 /* GNUTLS_A_DECOMPRESSION_FAILURE */
#define GNUTLS_E_COMPRESSION_FAILED -27
#define GNUTLS_E_AGAIN -28
#define GNUTLS_E_EXPIRED -29
#define GNUTLS_E_DB_ERROR -30
#define GNUTLS_E_SRP_PWD_ERROR -31
#define GNUTLS_E_INSUFFICIENT_CREDENTIALS -32
#define GNUTLS_E_INSUFICIENT_CREDENTIALS GNUTLS_E_INSUFFICIENT_CREDENTIALS /* for backwards compatibility only */
#define GNUTLS_E_INSUFFICIENT_CRED GNUTLS_E_INSUFFICIENT_CREDENTIALS
#define GNUTLS_E_INSUFICIENT_CRED GNUTLS_E_INSUFFICIENT_CREDENTIALS /* for backwards compatibility only */

#define GNUTLS_E_HASH_FAILED -33
#define GNUTLS_E_BASE64_DECODING_ERROR -34

#define	GNUTLS_E_MPI_PRINT_FAILED -35
#define GNUTLS_E_REHANDSHAKE -37 /* GNUTLS_A_NO_RENEGOTIATION */
#define GNUTLS_E_GOT_APPLICATION_DATA -38
#define GNUTLS_E_RECORD_LIMIT_REACHED -39
#define GNUTLS_E_ENCRYPTION_FAILED -40

#define GNUTLS_E_PK_ENCRYPTION_FAILED -44
#define GNUTLS_E_PK_DECRYPTION_FAILED -45
#define GNUTLS_E_PK_SIGN_FAILED -46
#define GNUTLS_E_X509_UNSUPPORTED_CRITICAL_EXTENSION -47
#define GNUTLS_E_KEY_USAGE_VIOLATION -48
#define GNUTLS_E_NO_CERTIFICATE_FOUND -49 /* GNUTLS_A_BAD_CERTIFICATE */
#define GNUTLS_E_INVALID_REQUEST -50
#define GNUTLS_E_SHORT_MEMORY_BUFFER -51
#define GNUTLS_E_INTERRUPTED -52
#define GNUTLS_E_PUSH_ERROR -53
#define GNUTLS_E_PULL_ERROR -54
#define GNUTLS_E_RECEIVED_ILLEGAL_PARAMETER -55  /* GNUTLS_A_ILLEGAL_PARAMETER */
#define GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE -56
#define GNUTLS_E_PKCS1_WRONG_PAD -57
#define GNUTLS_E_RECEIVED_ILLEGAL_EXTENSION -58
#define GNUTLS_E_INTERNAL_ERROR -59
#define GNUTLS_E_DH_PRIME_UNACCEPTABLE -63
#define GNUTLS_E_FILE_ERROR -64
#define GNUTLS_E_TOO_MANY_EMPTY_PACKETS -78
#define GNUTLS_E_UNKNOWN_PK_ALGORITHM -80


/* returned if libextra functionality was requested but
 * gnutls_global_init_extra() was not called.
 */
#define GNUTLS_E_INIT_LIBEXTRA -82
#define GNUTLS_E_LIBRARY_VERSION_MISMATCH -83


/* returned if you need to generate temporary RSA 
 * parameters. These are needed for export cipher suites.
 */
#define GNUTLS_E_NO_TEMPORARY_RSA_PARAMS -84

#define GNUTLS_E_LZO_INIT_FAILED -85
#define GNUTLS_E_NO_COMPRESSION_ALGORITHMS -86
#define GNUTLS_E_NO_CIPHER_SUITES -87

#define GNUTLS_E_OPENPGP_GETKEY_FAILED -88
#define GNUTLS_E_PK_SIG_VERIFY_FAILED -89

#define GNUTLS_E_ILLEGAL_SRP_USERNAME -90
#define GNUTLS_E_SRP_PWD_PARSING_ERROR -91
#define GNUTLS_E_NO_TEMPORARY_DH_PARAMS -93

/* For certificate and key stuff
 */
#define GNUTLS_E_ASN1_ELEMENT_NOT_FOUND -67
#define GNUTLS_E_ASN1_IDENTIFIER_NOT_FOUND -68
#define GNUTLS_E_ASN1_DER_ERROR -69
#define GNUTLS_E_ASN1_VALUE_NOT_FOUND -70
#define GNUTLS_E_ASN1_GENERIC_ERROR -71
#define GNUTLS_E_ASN1_VALUE_NOT_VALID -72
#define GNUTLS_E_ASN1_TAG_ERROR -73
#define GNUTLS_E_ASN1_TAG_IMPLICIT -74
#define GNUTLS_E_ASN1_TYPE_ANY_ERROR -75
#define GNUTLS_E_ASN1_SYNTAX_ERROR -76
#define GNUTLS_E_ASN1_DER_OVERFLOW -77
#define GNUTLS_E_OPENPGP_TRUSTDB_VERSION_UNSUPPORTED -81
#define GNUTLS_E_OPENPGP_UID_REVOKED -79
#define GNUTLS_E_CERTIFICATE_ERROR -43
#define GNUTLS_E_X509_CERTIFICATE_ERROR GNUTLS_E_CERTIFICATE_ERROR
#define GNUTLS_E_CERTIFICATE_KEY_MISMATCH -60
#define GNUTLS_E_UNSUPPORTED_CERTIFICATE_TYPE -61 /* GNUTLS_A_UNSUPPORTED_CERTIFICATE */
#define GNUTLS_E_X509_UNKNOWN_SAN -62
#define GNUTLS_E_OPENPGP_FINGERPRINT_UNSUPPORTED -94
#define GNUTLS_E_X509_UNSUPPORTED_ATTRIBUTE -95
#define GNUTLS_E_UNKNOWN_HASH_ALGORITHM -96
#define GNUTLS_E_UNKNOWN_PKCS_CONTENT_TYPE -97
#define GNUTLS_E_UNKNOWN_PKCS_BAG_TYPE -98
#define GNUTLS_E_INVALID_PASSWORD -99
#define GNUTLS_E_MAC_VERIFY_FAILED -100 /* for PKCS #12 MAC */

#define GNUTLS_E_BASE64_ENCODING_ERROR -201
#define GNUTLS_E_INCOMPATIBLE_GCRYPT_LIBRARY -202
#define GNUTLS_E_INCOMPATIBLE_LIBTASN1_LIBRARY -203

#define GNUTLS_E_OPENPGP_KEYRING_ERROR -204
#define GNUTLS_E_X509_UNSUPPORTED_OID -205

#define GNUTLS_E_UNIMPLEMENTED_FEATURE -1250



#endif /* GNUTLS_ERRORS_IH */

#ifdef __cplusplus
}
#endif
#endif /* GNUTLS_H */

