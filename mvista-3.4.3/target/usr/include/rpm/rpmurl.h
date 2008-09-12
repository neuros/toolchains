#ifndef H_RPMURL
#define H_RPMURL

/** \ingroup rpmio
 * \file rpmio/rpmurl.h
 */

#include <assert.h>

/**
 * Supported URL types.
 */
typedef enum urltype_e {
    URL_IS_UNKNOWN	= 0,	/*!< unknown (aka a file) */
    URL_IS_DASH		= 1,	/*!< stdin/stdout */
    URL_IS_PATH		= 2,	/*!< file://... */
    URL_IS_FTP		= 3,	/*!< ftp://... */
    URL_IS_HTTP		= 4	/*!< http://... */
} urltype;

#define	URLMAGIC	0xd00b1ed0
#define	URLSANE(u)	assert(u && u->magic == URLMAGIC)

typedef /*@abstract@*/ /*@refcounted@*/ struct urlinfo_s * urlinfo;

/**
 * URL control structure.
 */
struct urlinfo_s {
/*@refs@*/ int nrefs;		/*!< no. of references */
/*@owned@*/ /*@null@*/
    const char * url;		/*!< copy of original url */
/*@owned@*/ /*@null@*/
    const char * service;
/*@owned@*/ /*@null@*/
    const char * user;
/*@owned@*/ /*@null@*/
    const char * password;
/*@owned@*/ /*@null@*/
    const char * host;
/*@owned@*/ /*@null@*/
    const char * portstr;
/*@owned@*/ /*@null@*/
    const char * proxyu;	/*!< FTP: proxy user */
/*@owned@*/ /*@null@*/
    const char * proxyh;	/*!< FTP/HTTP: proxy host */
    int proxyp;			/*!< FTP/HTTP: proxy port */
    int	port;
    int urltype;
    FD_t ctrl;			/*!< control channel */
    FD_t data;			/*!< per-xfer data channel */
    int bufAlloced;		/*!< sizeof I/O buffer */
/*@owned@*/
    char * buf;			/*!< I/O buffer */
    int openError;		/*!< Type of open failure */
    int httpVersion;
    int httpHasRange;
    int magic;
};

#ifdef __cplusplus
extern "C" {
#endif

/*@unchecked@*/
extern int _url_count;		/*!< No. of cached URL's. */

/*@unchecked@*/
/*@only@*/ /*@null@*/
extern urlinfo * _url_cache;	/*!< URL cache. */

/*@unchecked@*/
extern int _url_iobuf_size;	/*!< Initial size of URL I/O buffer. */
#define RPMURL_IOBUF_SIZE	4096

/*@unchecked@*/
extern int _url_debug;		/*!< URL debugging? */
#define RPMURL_DEBUG_IO		0x40000000
#define RPMURL_DEBUG_REFS	0x20000000


/**
 * Create a URL control structure instance.
 * @param msg		debugging identifier (unused)
 * @return		new instance
 */
/*@unused@*/ urlinfo	urlNew(const char * msg)	/*@*/;

/** @todo Remove debugging entry from the ABI. */
urlinfo	XurlNew(const char * msg, const char * file, unsigned line)	/*@*/;
#define	urlNew(_msg) XurlNew(_msg, __FILE__, __LINE__)

/**
 * Reference a URL control structure instance.
 * @param u		URL control structure
 * @param msg		debugging identifier (unused)
 * @return		referenced instance
 */
/*@unused@*/ urlinfo	urlLink(urlinfo u, const char * msg)
	/*@modifies u @*/;

/** @todo Remove debugging entry from the ABI. */
urlinfo	XurlLink(urlinfo u, const char * msg, const char * file, unsigned line)
	/*@modifies u @*/;
#define	urlLink(_u, _msg) XurlLink(_u, _msg, __FILE__, __LINE__)

/**
 * Dereference a URL control structure instance.
 * @param u		URL control structure
 * @param msg		debugging identifier (unused)
 * @return		dereferenced instance (NULL if freed)
 */
/*@unused@*/ urlinfo	urlFree( /*@killref@*/ urlinfo u, const char * msg)
	/*@globals fileSystem, internalState @*/
	/*@modifies u, fileSystem, internalState @*/;

/** @todo Remove debugging entry from the ABI. */
urlinfo	XurlFree( /*@killref@*/ urlinfo u, const char * msg,
		const char * file, unsigned line)
	/*@globals fileSystem, internalState @*/
	/*@modifies u, fileSystem, internalState @*/;
#define	urlFree(_u, _msg) XurlFree(_u, _msg, __FILE__, __LINE__)

/**
 * Free cached URL control structures.
 */
void urlFreeCache(void)
	/*@globals _url_cache, _url_count, fileSystem, internalState @*/
	/*@modifies _url_cache, _url_count, fileSystem, internalState @*/;

/**
 * Return type of URL.
 * @param url		url string
 * @return		type of url
 */
urltype	urlIsURL(const char * url)
	/*@*/;

/**
 * Return path component of URL.
 * @param url		url string
 * @retval pathp	pointer to path component of url
 * @return		type of url
 */
/*@-incondefs@*/
urltype	urlPath(const char * url, /*@out@*/ const char ** pathp)
	/*@ensures maxSet(*pathp) == 0 /\ maxRead(*pathp) == 0 @*/
	/*@modifies *pathp @*/;
/*@=incondefs@*/

/**
 * Parse URL string into a control structure.
 * @param url		url string
 * @retval uret		address of new control instance pointer
 * @return		0 on success, -1 on error
 */
int urlSplit(const char * url, /*@out@*/ urlinfo * uret)
	/*@globals h_errno, internalState @*/
	/*@modifies *uret, internalState @*/;

/**
 * Copy data from URL to local file.
 * @param url		url string of source
 * @param dest		file name of destination
 * @return		0 on success, otherwise FTPERR_* code
 */
int urlGetFile(const char * url, /*@null@*/ const char * dest)
	/*@globals h_errno, fileSystem, internalState @*/
	/*@modifies fileSystem, internalState @*/;

#ifdef __cplusplus
}
#endif

#endif	/* H_RPMURL */
