#ifndef	H_RPMIO
#define	H_RPMIO

/** \ingroup rpmio
 * \file rpmio/rpmio.h
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
/*@-noparams@*/
#include <glob.h>
/*@=noparams@*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 */
typedef /*@abstract@*/ struct pgpDig_s * pgpDig;

/**
 */
typedef /*@abstract@*/ struct pgpDigParams_s * pgpDigParams;

/** \ingroup rpmio
 * Hide libio API lossage.
 * The libio interface changed after glibc-2.1.3 to pass the seek offset
 * argument as a pointer rather than as an off_t. The snarl below defines
 * typedefs to isolate the lossage.
 */
/*@{*/
#if !defined(__LCLINT__) && defined(__GLIBC__) && \
	(__GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 2))
#define USE_COOKIE_SEEK_POINTER 1
typedef _IO_off64_t 	_libio_off_t;
typedef _libio_off_t *	_libio_pos_t;
#else
typedef off_t 		_libio_off_t;
typedef off_t 		_libio_pos_t;
#endif
/*@}*/

/** \ingroup rpmio
 */
typedef	/*@abstract@*/ /*@refcounted@*/ struct _FD_s * FD_t;

/** \ingroup rpmio
 */
typedef /*@observer@*/ struct FDIO_s * FDIO_t;

#ifdef __cplusplus
extern "C" {
#endif

/** \ingroup rpmio
 * \name RPMIO Vectors.
 */
/*@{*/

/**
 */
typedef ssize_t (*fdio_read_function_t) (void *cookie, char *buf, size_t nbytes)
	/*@globals errno, fileSystem @*/
	/*@modifies *cookie, errno, fileSystem @*/
	/*@requires maxSet(buf) >= (nbytes - 1) @*/
	/*@ensures maxRead(buf) == result @*/ ;

/**
 */
typedef ssize_t (*fdio_write_function_t) (void *cookie, const char *buf, size_t nbytes)
	/*@globals errno, fileSystem @*/
	/*@modifies *cookie, errno, fileSystem @*/;

/**
 */
typedef int (*fdio_seek_function_t) (void *cookie, _libio_pos_t pos, int whence)
	/*@globals errno, fileSystem @*/
	/*@modifies *cookie, errno, fileSystem @*/;

/**
 */
typedef int (*fdio_close_function_t) (void *cookie)
	/*@globals errno, fileSystem, systemState @*/
	/*@modifies *cookie, errno, fileSystem, systemState @*/;


/**
 */
typedef /*@only@*/ /*@null@*/ FD_t (*fdio_ref_function_t) ( /*@only@*/ void * cookie,
		const char * msg, const char * file, unsigned line)
	/*@globals fileSystem @*/
	/*@modifies fileSystem @*/;

/**
 */
typedef /*@only@*/ /*@null@*/ FD_t (*fdio_deref_function_t) ( /*@only@*/ FD_t fd,
		const char * msg, const char * file, unsigned line)
	/*@globals fileSystem @*/
	/*@modifies fd, fileSystem @*/;


/**
 */
typedef /*@only@*/ /*@null@*/ FD_t (*fdio_new_function_t) (const char * msg,
		const char * file, unsigned line)
	/*@globals fileSystem @*/
	/*@modifies fileSystem @*/;


/**
 */
typedef int (*fdio_fileno_function_t) (void * cookie)
	/*@globals fileSystem @*/
	/*@modifies *cookie, fileSystem @*/;


/**
 */
typedef FD_t (*fdio_open_function_t) (const char * path, int flags, mode_t mode)
	/*@globals errno, fileSystem @*/
	/*@modifies errno, fileSystem @*/;

/**
 */
typedef FD_t (*fdio_fopen_function_t) (const char * path, const char * fmode)
	/*@globals fileSystem @*/
	/*@modifies fileSystem @*/;

/**
 */
typedef void * (*fdio_ffileno_function_t) (FD_t fd)
	/*@globals fileSystem @*/
	/*@modifies fileSystem @*/;

/**
 */
typedef int (*fdio_fflush_function_t) (FD_t fd)
	/*@globals fileSystem @*/
	/*@modifies fileSystem @*/;
/*@}*/


/** \ingroup rpmrpc
 * \name RPMRPC Vectors.
 */
/*@{*/

/**
 */
typedef int (*fdio_mkdir_function_t) (const char * path, mode_t mode)
	/*@globals errno, fileSystem @*/
	/*@modifies errno, fileSystem @*/;

/**
 */
typedef int (*fdio_chdir_function_t) (const char * path)
	/*@globals errno, fileSystem @*/
	/*@modifies errno, fileSystem @*/;

/**
 */
typedef int (*fdio_rmdir_function_t) (const char * path)
	/*@globals errno, fileSystem @*/
	/*@modifies errno, fileSystem @*/;

/**
 */
typedef int (*fdio_rename_function_t) (const char * oldpath, const char * newpath)
	/*@globals errno, fileSystem @*/
	/*@modifies errno, fileSystem @*/;

/**
 */
typedef int (*fdio_unlink_function_t) (const char * path)
	/*@globals errno, fileSystem @*/
	/*@modifies errno, fileSystem @*/;
/*@-typeuse@*/

/**
 */
typedef int (*fdio_stat_function_t) (const char * path, /*@out@*/ struct stat * st)
	/*@globals errno, fileSystem @*/
	/*@modifies *st, errno, fileSystem @*/;

/**
 */
typedef int (*fdio_lstat_function_t) (const char * path, /*@out@*/ struct stat * st)
	/*@globals errno, fileSystem @*/
	/*@modifies *st, errno, fileSystem @*/;

/**
 */
typedef int (*fdio_access_function_t) (const char * path, int amode)
	/*@globals errno, fileSystem @*/
	/*@modifies errno, fileSystem @*/;
/*@=typeuse@*/
/*@}*/


/** \ingroup rpmio
 */
struct FDIO_s {
  fdio_read_function_t		read;
  fdio_write_function_t		write;
  fdio_seek_function_t		seek;
  fdio_close_function_t		close;

  fdio_ref_function_t		_fdref;
  fdio_deref_function_t		_fdderef;
  fdio_new_function_t		_fdnew;
  fdio_fileno_function_t	_fileno;

  fdio_open_function_t		_open;
  fdio_fopen_function_t		_fopen;
  fdio_ffileno_function_t	_ffileno;
  fdio_fflush_function_t	_fflush;

  fdio_mkdir_function_t		_mkdir;
  fdio_chdir_function_t		_chdir;
  fdio_rmdir_function_t		_rmdir;
  fdio_rename_function_t	_rename;
  fdio_unlink_function_t	_unlink;
};


/** \ingroup rpmio
 * \name RPMIO Interface.
 */
/*@{*/

/**
 * strerror(3) clone.
 */
/*@observer@*/ const char * Fstrerror(/*@null@*/ FD_t fd)
	/*@*/;

/**
 * fread(3) clone.
 */
/*@-incondefs@*/
size_t Fread(/*@out@*/ void * buf, size_t size, size_t nmemb, FD_t fd)
	/*@globals fileSystem @*/
	/*@modifies fd, *buf, fileSystem @*/
	/*@requires maxSet(buf) >= (nmemb - 1) @*/
	/*@ensures maxRead(buf) == result @*/;
/*@=incondefs@*/

/**
 * fwrite(3) clone.
 */
/*@-incondefs@*/
size_t Fwrite(const void * buf, size_t size, size_t nmemb, FD_t fd)
	/*@globals fileSystem @*/
	/*@modifies fd, fileSystem @*/
	/*@requires maxRead(buf) >= nmemb @*/;
/*@=incondefs@*/

/**
 * fseek(3) clone.
 */
int Fseek(FD_t fd, _libio_off_t offset, int whence)
	/*@globals fileSystem @*/
	/*@modifies fileSystem @*/;

/**
 * fclose(3) clone.
 */
int Fclose( /*@killref@*/ FD_t fd)
	/*@globals fileSystem, internalState @*/
	/*@modifies fd, fileSystem, internalState @*/;

/**
 */
/*@null@*/ FD_t	Fdopen(FD_t ofd, const char * fmode)
	/*@globals fileSystem @*/
	/*@modifies ofd, fileSystem @*/;

/**
 * fopen(3) clone.
 */
/*@null@*/ FD_t	Fopen(/*@null@*/ const char * path,
			/*@null@*/ const char * fmode)
	/*@globals h_errno, fileSystem, internalState @*/
	/*@modifies fileSystem, internalState @*/;


/**
 * fflush(3) clone.
 */
int Fflush(/*@null@*/ FD_t fd)
	/*@globals fileSystem @*/
	/*@modifies fd, fileSystem @*/;

/**
 * ferror(3) clone.
 */
int Ferror(/*@null@*/ FD_t fd)
	/*@*/;

/**
 * fileno(3) clone.
 */
int Fileno(FD_t fd)
	/*@globals fileSystem @*/
	/*@modifies fileSystem@*/;

/**
 * fcntl(2) clone.
 */
/*@unused@*/
int Fcntl(FD_t fd, int op, void *lip)
	/*@globals errno, fileSystem @*/
	/*@modifies fd, *lip, errno, fileSystem @*/;

/*@}*/

/** \ingroup rpmrpc
 * \name RPMRPC Interface.
 */
/*@{*/

/**
 * mkdir(2) clone.
 */
int Mkdir(const char * path, mode_t mode)
	/*@globals errno, h_errno, fileSystem, internalState @*/
	/*@modifies errno, fileSystem, internalState @*/;

/**
 * chdir(2) clone.
 */
int Chdir(const char * path)
	/*@globals errno, h_errno, fileSystem, internalState @*/
	/*@modifies errno, fileSystem, internalState @*/;

/**
 * rmdir(2) clone.
 */
int Rmdir(const char * path)
	/*@globals errno, h_errno, fileSystem, internalState @*/
	/*@modifies errno, fileSystem, internalState @*/;

/**
 * rename(2) clone.
 */
int Rename(const char * oldpath, const char * newpath)
	/*@globals errno, h_errno, fileSystem, internalState @*/
	/*@modifies errno, fileSystem, internalState @*/;

/**
 * link(2) clone.
 */
int Link(const char * oldpath, const char * newpath)
	/*@globals errno, fileSystem, internalState @*/
	/*@modifies errno, fileSystem, internalState @*/;

/**
 * unlink(2) clone.
 */
int Unlink(const char * path)
	/*@globals errno, h_errno, fileSystem, internalState @*/
	/*@modifies errno, fileSystem, internalState @*/;

/**
 * readlink(2) clone.
 */
/*@-incondefs@*/
int Readlink(const char * path, /*@out@*/ char * buf, size_t bufsiz)
	/*@globals errno, h_errno, fileSystem, internalState @*/
	/*@modifies *buf, errno, fileSystem, internalState @*/
	/*@requires maxSet(buf) >= (bufsiz - 1) @*/
	/*@ensures maxRead(buf) <= bufsiz @*/;
/*@=incondefs@*/

/**
 * stat(2) clone.
 */
int Stat(const char * path, /*@out@*/ struct stat * st)
	/*@globals errno, h_errno, fileSystem, internalState @*/
	/*@modifies *st, errno, fileSystem, internalState @*/;

/**
 * lstat(2) clone.
 */
int Lstat(const char * path, /*@out@*/ struct stat * st)
	/*@globals errno, h_errno, fileSystem, internalState @*/
	/*@modifies *st, errno, fileSystem, internalState @*/;

/**
 * access(2) clone.
 */
int Access(const char * path, int amode)
	/*@globals errno, fileSystem @*/
	/*@modifies errno, fileSystem @*/;

/**
 * glob_pattern_p(3) clone.
 */
int Glob_pattern_p (const char *pattern, int quote)
	/*@*/;

/**
 * glob_error(3) clone.
 */
int Glob_error(const char * epath, int eerrno)
	/*@*/;

/**
 * glob(3) clone.
 */
int Glob(const char * pattern, int flags,
		int errfunc(const char * epath, int eerrno),
		/*@out@*/ glob_t * pglob)
	/*@globals fileSystem @*/
	/*@modifies *pglob, fileSystem @*/;

/**
 * globfree(3) clone.
 */
void Globfree( /*@only@*/ glob_t * pglob)
	/*@globals fileSystem @*/
	/*@modifies *pglob, fileSystem @*/;


/**
 * opendir(3) clone.
 */
/*@null@*/
DIR * Opendir(const char * path)
	/*@globals errno, h_errno, fileSystem, internalState @*/
	/*@modifies errno, fileSystem, internalState @*/;

/**
 * readdir(3) clone.
 */
/*@dependent@*/ /*@null@*/
struct dirent * Readdir(DIR * dir)
	/*@globals errno, fileSystem @*/
	/*@modifies *dir, errno, fileSystem @*/;

/**
 * closedir(3) clone.
 */
int Closedir(/*@only@*/ DIR * dir)
	/*@globals errno, fileSystem @*/
	/*@modifies *dir, errno, fileSystem @*/;

/*@}*/


/** \ingroup rpmio
 * \name RPMIO Utilities.
 */
/*@{*/

/**
 */
off_t	fdSize(FD_t fd)
	/*@globals fileSystem @*/
	/*@modifies fd, fileSystem@*/;

/**
 */
/*@null@*/ FD_t fdDup(int fdno)
	/*@globals fileSystem, internalState @*/
	/*@modifies fileSystem, internalState @*/;

#ifdef UNUSED
/*@null@*/ FILE *fdFdopen( /*@only@*/ void * cookie, const char * mode);
#endif

/* XXX Legacy interfaces needed by gnorpm, rpmfind et al */

/*@-exportlocal@*/
/**
 */
#ifndef H_RPMIO_INTERNAL	/* XXX avoid gcc warning */
/*@unused@*/ int fdFileno(void * cookie)
	/*@*/;
#define	fdFileno(_fd)		fdio->_fileno(_fd)
#endif

/**
 */
/*@null@*/ FD_t fdOpen(const char *path, int flags, mode_t mode)
	/*@globals errno, fileSystem, internalState @*/
	/*@modifies errno, fileSystem, internalState @*/;
#define	fdOpen(_path, _flags, _mode)	fdio->_open((_path), (_flags), (_mode))

/**
 */
/*@-incondefs@*/
ssize_t fdRead(void * cookie, /*@out@*/ char * buf, size_t count)
	/*@globals errno, fileSystem, internalState @*/
	/*@modifies *cookie, *buf, errno, fileSystem, internalState @*/
	/*@requires maxSet(buf) >= (count - 1) @*/
	/*@ensures maxRead(buf) == result @*/ ;
#define	fdRead(_fd, _buf, _count)	fdio->read((_fd), (_buf), (_count))
/*@=incondefs@*/

/**
 */
ssize_t	fdWrite(void * cookie, const char * buf, size_t count)
	/*@globals errno, fileSystem, internalState @*/
	/*@modifies *cookie, errno, fileSystem, internalState @*/;
#define	fdWrite(_fd, _buf, _count)	fdio->write((_fd), (_buf), (_count))

/**
 */
int fdClose( /*@only@*/ void * cookie)
	/*@globals errno, fileSystem, systemState, internalState @*/
	/*@modifies *cookie, errno, fileSystem, systemState, internalState @*/;
#define	fdClose(_fd)		fdio->close(_fd)

/**
 */
/*@unused@*/
/*@only@*/ /*@null@*/
FD_t fdLink (/*@only@*/ void * cookie, const char * msg)
	/*@globals fileSystem @*/
	/*@modifies *cookie, fileSystem @*/;
#define	fdLink(_fd, _msg)	fdio->_fdref(_fd, _msg, __FILE__, __LINE__)

/**
 */
/*@unused@*/
/*@only@*/ /*@null@*/
FD_t fdFree(/*@only@*/ FD_t fd, const char * msg)
	/*@globals fileSystem @*/
	/*@modifies fd, fileSystem @*/;
#define	fdFree(_fd, _msg)	fdio->_fdderef(_fd, _msg, __FILE__, __LINE__)

/**
 */
/*@unused@*/
/*@only@*/ /*@null@*/
FD_t fdNew (const char * msg)
	/*@globals fileSystem @*/
	/*@modifies fileSystem @*/;
#define	fdNew(_msg)		fdio->_fdnew(_msg, __FILE__, __LINE__)

/**
 */
int fdWritable(FD_t fd, int secs)
	/*@globals errno, fileSystem @*/
	/*@modifies fd, errno, fileSystem @*/;

/**
 */
int fdReadable(FD_t fd, int secs)
	/*@globals errno @*/
	/*@modifies fd, errno @*/;
/*@=exportlocal@*/

/**
 * Insure that directories in path exist, creating as needed.
 * @param path		diretory path
 * @param mode		directory mode (if created)
 * @param uid		directory uid (if created), or -1 to skip
 * @param gid		directory uid (if created), or -1 to skip
 * @return		0 on success, errno (or -1) on error
 */
int rpmioMkpath(const char * path, mode_t mode, uid_t uid, gid_t gid)
	/*@globals h_errno, fileSystem, internalState @*/
	/*@modifies fileSystem, internalState @*/;

/**
 * FTP and HTTP error codes.
 */
/*@-typeuse@*/
typedef enum ftperrCode_e {
    FTPERR_BAD_SERVER_RESPONSE	= -1,	/*!< Bad server response */
    FTPERR_SERVER_IO_ERROR	= -2,	/*!< Server I/O error */
    FTPERR_SERVER_TIMEOUT	= -3,	/*!< Server timeout */
    FTPERR_BAD_HOST_ADDR	= -4,	/*!< Unable to lookup server host address */
    FTPERR_BAD_HOSTNAME		= -5,	/*!< Unable to lookup server host name */
    FTPERR_FAILED_CONNECT	= -6,	/*!< Failed to connect to server */
    FTPERR_FILE_IO_ERROR	= -7,	/*!< Failed to establish data connection to server */
    FTPERR_PASSIVE_ERROR	= -8,	/*!< I/O error to local file */
    FTPERR_FAILED_DATA_CONNECT	= -9,	/*!< Error setting remote server to passive mode */
    FTPERR_FILE_NOT_FOUND	= -10,	/*!< File not found on server */
    FTPERR_NIC_ABORT_IN_PROGRESS= -11,	/*!< Abort in progress */
    FTPERR_UNKNOWN		= -100	/*!< Unknown or unexpected error */
} ftperrCode;
/*@=typeuse@*/

/**
 */
/*@-redecl@*/
/*@observer@*/ const char *const ftpStrerror(int errorNumber)
	/*@*/;
/*@=redecl@*/

/**
 */
/*@unused@*/
/*@dependent@*/ /*@null@*/ void * ufdGetUrlinfo(FD_t fd)
	/*@modifies fd @*/;

/**
 */
/*@-redecl@*/
/*@unused@*/
/*@observer@*/ const char * urlStrerror(const char * url)
	/*@globals h_errno, internalState @*/
	/*@modifies internalState @*/;
/*@=redecl@*/

/**
 */
/*@-exportlocal@*/
int ufdCopy(FD_t sfd, FD_t tfd)
	/*@globals fileSystem @*/
	/*@modifies sfd, tfd, fileSystem @*/;
/*@=exportlocal@*/

/**
 */
int ufdGetFile( /*@killref@*/ FD_t sfd, FD_t tfd)
	/*@globals fileSystem, internalState @*/
	/*@modifies sfd, tfd, fileSystem, internalState @*/;

/**
 */
/*@unused@*/ int timedRead(FD_t fd, /*@out@*/ void * bufptr, int length)
	/*@globals fileSystem @*/
	/*@modifies fd, *bufptr, fileSystem @*/;
#define	timedRead	ufdio->read

/*@-exportlocal@*/
/**
 */
/*@observer@*/ /*@unchecked@*/ extern FDIO_t fdio;

/**
 */
/*@observer@*/ /*@unchecked@*/ extern FDIO_t fpio;

/**
 */
/*@observer@*/ /*@unchecked@*/ extern FDIO_t ufdio;

/**
 */
/*@observer@*/ /*@unchecked@*/ extern FDIO_t gzdio;

/**
 */
/*@observer@*/ /*@unchecked@*/ extern FDIO_t bzdio;

/**
 */
/*@observer@*/ /*@unchecked@*/ extern FDIO_t fadio;
/*@=exportlocal@*/
/*@}*/

/*@unused@*/ static inline int xislower(int c) /*@*/ {
    return (c >= 'a' && c <= 'z');
}
/*@unused@*/ static inline int xisupper(int c) /*@*/ {
    return (c >= 'A' && c <= 'Z');
}
/*@unused@*/ static inline int xisalpha(int c) /*@*/ {
    return (xislower(c) || xisupper(c));
}
/*@unused@*/ static inline int xisdigit(int c) /*@*/ {
    return (c >= '0' && c <= '9');
}
/*@unused@*/ static inline int xisalnum(int c) /*@*/ {
    return (xisalpha(c) || xisdigit(c));
}
/*@unused@*/ static inline int xisblank(int c) /*@*/ {
    return (c == ' ' || c == '\t');
}
/*@unused@*/ static inline int xisspace(int c) /*@*/ {
    return (xisblank(c) || c == '\n' || c == '\r' || c == '\f' || c == '\v');
}

/*@unused@*/ static inline int xtolower(int c) /*@*/ {
    return ((xisupper(c)) ? (c | ('a' - 'A')) : c);
}
/*@unused@*/ static inline int xtoupper(int c) /*@*/ {
    return ((xislower(c)) ? (c & ~('a' - 'A')) : c);
}

/** \ingroup rpmio
 * Locale insensitive strcasecmp(3).
 */
int xstrcasecmp(const char * s1, const char * s2)		/*@*/;

/** \ingroup rpmio
 * Locale insensitive strncasecmp(3).
 */
int xstrncasecmp(const char *s1, const char * s2, size_t n)	/*@*/;

#ifdef __cplusplus
}
#endif

#endif	/* H_RPMIO */
