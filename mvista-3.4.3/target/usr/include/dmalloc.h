/*
 * Defines for the dmalloc library
 *
 * Copyright 2000 by Gray Watson
 *
 * This file is part of the dmalloc package.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose and without fee is hereby granted, provided
 * that the above copyright notice and this permission notice appear
 * in all copies, and that the name of Gray Watson not be used in
 * advertising or publicity pertaining to distribution of the document
 * or software without specific, written prior permission.
 *
 * Gray Watson makes no representations about the suitability of the
 * software described herein for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * The author may be contacted via http://dmalloc.com/
 *
 * $Id: dmalloc.h.1,v 1.13 2000/03/21 18:19:10 gray Exp $
 */

#ifndef __DMALLOC_H__
#define __DMALLOC_H__

/* this is dmalloc.h.2 */
/* produced by configure, inserted into dmalloc.h */

/* const is available */
/* strdup is not a macro */
#undef DMALLOC_STRDUP_MACRO

/*
 * the definition of DMALLOC_SIZE
 *
 * NOTE: some architectures have malloc, realloc, etc.
 * using unsigned instead of unsigned long.  You may
 * have to edit this by hand to fix any compilation
 * warnings or errors.
 */
#include <sys/types.h>
#define DMALLOC_SIZE size_t

/*
 * We use stdarg.h for the dmalloc_message and
 * dmalloc_vmessage functions.
 */
#include <stdarg.h>
#define DMALLOC_STDARG 1

/* NOTE: start of $Id: dmalloc.h.4,v 1.5 2003/09/29 23:13:34 gray Exp $ */

/* dmalloc version defines */
#define DMALLOC_VERSION_MAJOR	5	/* X.0.0-b0 */
#define DMALLOC_VERSION_MINOR	2	/* 0.X.0-b0 */
#define DMALLOC_VERSION_PATCH	3	/* 0.0.X-b0 */

/* this defines what type the standard void memory-pointer is */
#if (defined(__STDC__) && __STDC__ == 1) || defined(__cplusplus) || defined(STDC_HEADERS)
#define DMALLOC_PNT		void *
#define DMALLOC_FREE_RET	void
#else
#define DMALLOC_PNT		char *
#define DMALLOC_FREE_RET	int
#endif

/*
 * Malloc function return codes
 */
#define CALLOC_ERROR		0L		/* error from calloc */
#define MALLOC_ERROR		0L		/* error from malloc */
#define REALLOC_ERROR		0L		/* error from realloc */

/* NOTE: this if for non- __STDC__ systems only */
#define FREE_ERROR		0		/* error from free */
#define FREE_NOERROR		1		/* no error from free */

#define DMALLOC_ERROR		0		/* function failed */
#define DMALLOC_NOERROR		1		/* function succeeded */

#define DMALLOC_VERIFY_ERROR	0		/* function failed */
#define DMALLOC_VERIFY_NOERROR	1		/* function succeeded */
#define MALLOC_VERIFY_ERROR	DMALLOC_VERIFY_ERROR
#define MALLOC_VERIFY_NOERROR	DMALLOC_VERIFY_NOERROR

/*
 * Dmalloc function IDs for the dmalloc_track_t callback function.
 */
#define DMALLOC_FUNC_MALLOC	10	/* malloc function called */
#define DMALLOC_FUNC_CALLOC	11	/* calloc function called */
#define DMALLOC_FUNC_REALLOC	12	/* realloc function called */
#define DMALLOC_FUNC_RECALLOC	13	/* recalloc called */
#define DMALLOC_FUNC_MEMALIGN	14	/* memalign function called */
#define DMALLOC_FUNC_VALLOC	15	/* valloc function called */
#define DMALLOC_FUNC_STRDUP	16	/* strdup function called */
#define DMALLOC_FUNC_FREE	17	/* free function called */
#define DMALLOC_FUNC_CFREE	18	/* cfree function called */

#define DMALLOC_FUNC_NEW	20	/* new function called */
#define DMALLOC_FUNC_NEW_ARRAY	21	/* new[] function called */
#define DMALLOC_FUNC_DELETE	22	/* delete function called */
#define DMALLOC_FUNC_DELETE_ARRAY 23	/* delete[] function called */

#ifdef __cplusplus
extern "C" {
#endif

typedef void  (*dmalloc_track_t)(const char *file, const unsigned int line,
				 const int func_id,
				 const DMALLOC_SIZE byte_size,
				 const DMALLOC_SIZE alignment,
				 const DMALLOC_PNT old_addr,
				 const DMALLOC_PNT new_addr);


/* internal dmalloc error number for reference purposes only */
extern
int		dmalloc_errno;

/* logfile for dumping dmalloc info, DMALLOC_LOGFILE env var overrides this */
extern
char		*dmalloc_logpath;

/*
 * void dmalloc_shutdown
 *
 * DESCRIPTION:
 *
 * Shutdown the dmalloc library and provide statistics if necessary.
 *
 * RETURNS:
 *
 * None.
 *
 * ARGUMENTS:
 *
 * None.
 */
extern
void	dmalloc_shutdown(void);

#if FINI_DMALLOC
/*
 * void __fini_dmalloc
 *
 * DESCRIPTION:
 *
 * Automatic function to close dmalloc supported by some operating
 * systems.  Pretty cool OS/compiler hack.  By default it is not
 * necessary because we use atexit() and on_exit() to register the
 * close functions which are more portable.
 *
 * RETURNS:
 *
 * None.
 *
 * ARGUMENTS:
 *
 * None.
 */
extern
void	__fini_dmalloc(void);
#endif /* if FINI_DMALLOC */

/*
 * DMALLOC_PNT dmalloc_malloc
 *
 * DESCRIPTION:
 *
 * Allocate and return a memory block of a certain size.
 *
 * RETURNS:
 *
 * Success - Valid pointer.
 *
 * Failure - 0L
 *
 * ARGUMENTS:
 *
 * file -> File-name or return-address of the caller.
 *
 * line -> Line-number of the caller.
 *
 * size -> Number of bytes requested.
 *
 * func_id -> Function-id to identify the type of call.  See
 * dmalloc.h.
 *
 * alignment -> To align the new block to a certain number of bytes,
 * set this to a value greater than 0.
 *
 * xalloc_b -> If set to 1 then print an error and exit if we run out
 * of memory.
 */
extern
DMALLOC_PNT	dmalloc_malloc(const char *file, const int line,
			       const DMALLOC_SIZE size, const int func_id,
			       const DMALLOC_SIZE alignment,
			       const int xalloc_b);

/*
 * DMALLOC_PNT dmalloc_realloc
 *
 * DESCRIPTION:
 *
 * Resizes and old pointer to a new number of bytes.
 *
 * RETURNS:
 *
 * Success - Valid pointer.
 *
 * Failure - 0L
 *
 * ARGUMENTS:
 *
 * file -> File-name or return-address of the caller.
 *
 * line -> Line-number of the caller.
 *
 * old_pnt -> Pointer to an existing memory chunk that we are
 * resizing.  If this is NULL then it basically does a malloc.
 *
 * new_size -> New number of bytes requested for the old pointer.
 *
 * func_id -> Function-id to identify the type of call.  See
 * dmalloc.h.
 *
 * xalloc_b -> If set to 1 then print an error and exit if we run out
 * of memory.
 */
extern
DMALLOC_PNT	dmalloc_realloc(const char *file, const int line,
				DMALLOC_PNT old_pnt, DMALLOC_SIZE new_size,
				const int func_id, const int xalloc_b);

/*
 * int dmalloc_free
 *
 * DESCRIPTION:
 *
 * Release a pointer back into the heap.
 *
 * RETURNS:
 *
 * Success - FREE_NOERROR
 *
 * Failure - FREE_ERROR
 *
 * Note: many operating systems define free to return (void) so this
 * return value may be filtered.  Dumb.
 *
 * ARGUMENTS:
 *
 * file -> File-name or return-address of the caller.
 *
 * line -> Line-number of the caller.
 *
 * pnt -> Existing pointer we are freeing.
 *
 * func_id -> Function-id to identify the type of call.  See
 * dmalloc.h.
 */
extern
int	dmalloc_free(const char *file, const int line, DMALLOC_PNT pnt,
		     const int func_id);

/*
 * DMALLOC_PNT dmalloc_strdup
 *
 * DESCRIPTION:
 *
 * Allocate and return an allocated block of memory holding a copy of
 * a string.
 *
 * RETURNS:
 *
 * Success - Valid pointer.
 *
 * Failure - 0L
 *
 * ARGUMENTS:
 *
 * file -> File-name or return-address of the caller.
 *
 * line -> Line-number of the caller.
 *
 * string -> String we are duplicating.
 *
 * xalloc_b -> If set to 1 then print an error and exit if we run out
 * of memory.
 */
extern
char	*dmalloc_strdup(const char *file, const int line,
			const char *string, const int xalloc_b);

/*
 * DMALLOC_PNT malloc
 *
 * DESCRIPTION:
 *
 * Overloading the malloc(3) function.  Allocate and return a memory
 * block of a certain size.
 *
 * RETURNS:
 *
 * Success - Valid pointer.
 *
 * Failure - 0L
 *
 * ARGUMENTS:
 *
 * size -> Number of bytes requested.
 */
extern
DMALLOC_PNT	malloc(DMALLOC_SIZE size);

/*
 * DMALLOC_PNT malloc
 *
 * DESCRIPTION:
 *
 * Overloading the calloc(3) function.  Returns a block of zeroed memory.
 *
 * RETURNS:
 *
 * Success - Valid pointer.
 *
 * Failure - 0L
 *
 * ARGUMENTS:
 *
 * num_elements -> Number of elements being allocated.
 *
 * size -> The number of bytes in each element.
 */
extern
DMALLOC_PNT	calloc(DMALLOC_SIZE num_elements, DMALLOC_SIZE size);

/*
 * DMALLOC_PNT realloc
 *
 * DESCRIPTION:
 *
 * Overload of realloc(3).  Resizes and old pointer to a new number of
 * bytes.
 *
 * RETURNS:
 *
 * Success - Valid pointer.
 *
 * Failure - 0L
 *
 * ARGUMENTS:
 *
 * old_pnt -> Pointer to an existing memory chunk that we are
 * resizing.  If this is NULL then it basically does a malloc.
 *
 * new_size -> New number of bytes requested for the old pointer.
 */
extern
DMALLOC_PNT	realloc(DMALLOC_PNT old_pnt, DMALLOC_SIZE new_size);

/*
 * DMALLOC_PNT recalloc
 *
 * DESCRIPTION:
 *
 * Overload of recalloc(3) which exists on some systems.  Resizes and
 * old pointer to a new number of bytes.  If we are expanding, then
 * any new bytes will be zeroed.
 *
 * RETURNS:
 *
 * Success - Valid pointer.
 *
 * Failure - 0L
 *
 * ARGUMENTS:
 *
 * old_pnt -> Pointer to an existing memory chunk that we are
 * resizing.
 *
 * new_size -> New number of bytes requested for the old pointer.
 */
extern
DMALLOC_PNT	recalloc(DMALLOC_PNT old_pnt, DMALLOC_SIZE new_size);

/*
 * DMALLOC_PNT memalign
 *
 * DESCRIPTION:
 *
 * Overloading the memalign(3) function.  Allocate and return a memory
 * block of a certain size which have been aligned to a certain
 * alignment.
 *
 * RETURNS:
 *
 * Success - Valid pointer.
 *
 * Failure - 0L
 *
 * ARGUMENTS:
 *
 * alignment -> Value to which the allocation must be aligned.  This
 * should probably be a multiple of 2 with a maximum value equivalent
 * to the block-size which is often 1k or 4k.
 *
 * size -> Number of bytes requested.
 */
extern
DMALLOC_PNT	memalign(DMALLOC_SIZE alignment, DMALLOC_SIZE size);

/*
 * DMALLOC_PNT valloc
 *
 * DESCRIPTION:
 *
 * Overloading the valloc(3) function.  Allocate and return a memory
 * block of a certain size which have been aligned to page boundaries
 * which are often 1k or 4k.
 *
 * RETURNS:
 *
 * Success - Valid pointer.
 *
 * Failure - 0L
 *
 * ARGUMENTS:
 *
 * size -> Number of bytes requested.
 */
extern
DMALLOC_PNT	valloc(DMALLOC_SIZE size);

#ifndef DMALLOC_STRDUP_MACRO
/*
 * DMALLOC_PNT strdup
 *
 * DESCRIPTION:
 *
 * Overload of strdup(3).  Allocate and return an allocated block of
 * memory holding a copy of a string.
 *
 * RETURNS:
 *
 * Success - Valid pointer.
 *
 * Failure - 0L
 *
 * ARGUMENTS:
 *
 * string -> String we are duplicating.
 */
extern
char	*strdup(const char *string);
#endif /* ifndef DMALLOC_STRDUP_MACRO */

/*
 * DMALLOC_FREE_RET free
 *
 * DESCRIPTION:
 *
 * Release a pointer back into the heap.
 *
 * RETURNS:
 *
 * Returns FREE_ERROR, FREE_NOERROR or void depending on whether STDC
 * is defined by your compiler.
 *
 * ARGUMENTS:
 *
 * pnt -> Existing pointer we are freeing.
 */
extern
DMALLOC_FREE_RET	free(DMALLOC_PNT pnt);

/*
 * DMALLOC_FREE_RET cfree
 *
 * DESCRIPTION:
 *
 * Same as free.
 *
 * RETURNS:
 *
 * Returns FREE_ERROR, FREE_NOERROR or void depending on whether STDC
 * is defined by your compiler.
 *
 * ARGUMENTS:
 *
 * pnt -> Existing pointer we are freeing.
 */
extern
DMALLOC_FREE_RET	cfree(DMALLOC_PNT pnt);

/*
 * int dmalloc_verify
 *
 * DESCRIPTION:
 *
 * Verify a pointer which has previously been allocated by the
 * library.
 *
 * RETURNS:
 *
 * Success - MALLOC_VERIFY_NOERROR
 *
 * Failure - MALLOC_VERIFY_ERROR
 *
 * ARGUMENTS:
 *
 * pnt -> Pointer we are verifying.  If 0L then check the entire heap.
 */
extern
int	dmalloc_verify(const DMALLOC_PNT pnt);

/*
 * int malloc_verify
 *
 * DESCRIPTION:
 *
 * Verify a pointer which has previously been allocated by the
 * library.  Same as dmalloc_verify.
 *
 * RETURNS:
 *
 * Success - MALLOC_VERIFY_NOERROR
 *
 * Failure - MALLOC_VERIFY_ERROR
 *
 * ARGUMENTS:
 *
 * pnt -> Pointer we are verifying.  If 0L then check the entire heap.
 */
extern
int	malloc_verify(const DMALLOC_PNT pnt);

/*
 * unsigned int dmalloc_debug
 *
 * DESCRIPTION:
 *
 * Set the global debug functionality flags.  You can also use
 * dmalloc_debug_setup.
 *
 * Note: you cannot add or remove certain flags such as signal
 * handlers since they are setup at initialization time only.
 *
 * RETURNS:
 *
 * The old debug flag value.
 *
 * ARGUMENTS:
 *
 * flags -> Flag value to set.  Pass in 0 to disable all debugging.
 */
extern
unsigned int	dmalloc_debug(const unsigned int flags);

/*
 * unsigned int dmalloc_debug_current
 *
 * DESCRIPTION:
 *
 * Returns the current debug functionality flags.  This allows you to
 * save a dmalloc library state to be restored later.
 *
 * RETURNS:
 *
 * Current debug flags.
 *
 * ARGUMENTS:
 *
 * None.
 */
extern
unsigned int	dmalloc_debug_current(void);

/*
 * void dmalloc_debug_setup
 *
 * DESCRIPTION:
 *
 * Set the global debugging functionality as an option string.
 * Normally this would be pased in in the DMALLOC_OPTIONS
 * environmental variable.  This is here to override the env or for
 * circumstances where modifying the environment is not possible or
 * does not apply such as servers or cgi-bin programs.
 *
 * RETURNS:
 *
 * None.
 *
 * ARGUMENTS:
 *
 * options_str -> Options string to set the library flags.
 */
extern
void	dmalloc_debug_setup(const char *options_str);

/*
 * int dmalloc_examine
 *
 * DESCRIPTION:
 *
 * Examine a pointer and pass back information on its allocation size
 * as well as the file and line-number where it was allocated.  If the
 * file and line number is not available, then it will pass back the
 * allocation location's return-address if available.
 *
 * RETURNS:
 *
 * Success - DMALLOC_NOERROR
 *
 * Failure - DMALLOC_ERROR
 *
 * ARGUMENTS:
 *
 * pnt -> Pointer we are checking.
 *
 * user_size_p <- Pointer to a DMALLOC_SIZE type variable which, if
 * not NULL, will be set to the size of bytes from the pointer.
 *
 * total_size_p <- Poiner to a DMALLOC_SIZE type variable which, if
 * not NULL, will be set to the total size given for this allocation
 * including administrative overhead.
 *
 * file_p <- Pointer to a character pointer which, if not NULL, will
 * be set to the file where the pointer was allocated.
 *
 * line_p <- Pointer to an unsigned integer which, if not NULL, will
 * be set to the line-number where the pointer was allocated.
 *
 * ret_attr_p <- Pointer to a void pointer, if not NULL, will be set
 * to the return-address where the pointer was allocated.
 *
 * used_mark_p <- Poiner to an unsigned integer which, if not NULL,
 * will be set to the mark of when the pointer was last "used".  This
 * could be when it was allocated, reallocated, or freed.
 *
 * seen_p <- Poiner to an unsigned long which, if not NULL, will be
 * set to the number of times that this pointer has been allocated,
 * realloced, or freed.
 */
extern
int	dmalloc_examine(const DMALLOC_PNT pnt, DMALLOC_SIZE *user_size_p,
			DMALLOC_SIZE *total_size_p, char **file_p,
			unsigned int *line_p, DMALLOC_PNT *ret_attr_p,
			unsigned long *used_mark_p, unsigned long *seen_p);

/*
 * void dmalloc_track
 *
 * DESCRIPTION:
 *
 * Register an allocation tracking function which will be called each
 * time an allocation occurs.
 *
 * RETURNS:
 *
 * None.
 *
 * ARGUMENTS:
 *
 * track_func -> Function to register as the tracking function.  Set
 * to NULL to disable.
 */
extern
void	dmalloc_track(const dmalloc_track_t track_func);

/*
 * unsigned long dmalloc_mark
 *
 * DESCRIPTION:
 *
 * Return to the caller the current "mark" which can be used later by
 * dmalloc_log_changed to log the changed pointers since this point.
 * Multiple marks can be saved and used.
 *
 * RETURNS:
 *
 * Current mark value
 *
 * ARGUMENTS:
 *
 * None.
 */
extern
unsigned long	dmalloc_mark(void);

/*
 * unsigned int dmalloc_page_size
 *
 * DESCRIPTION:
 *
 * Get the page-size being used by dmalloc.
 *
 * RETURNS:
 *
 * Page size.
 *
 * ARGUMENTS:
 *
 * None.
 */
extern
unsigned int	dmalloc_page_size(void);

/*
 * void dmalloc_log_status
 *
 * DESCRIPTION:
 *
 * Dump dmalloc statistics to logfile.
 *
 * RETURNS:
 *
 * None.
 *
 * ARGUMENTS:
 *
 * None.
 */
extern
void	dmalloc_log_stats(void);

/*
 * void dmalloc_log_unfreed
 *
 * DESCRIPTION:
 *
 * Dump unfreed-memory info to logfile.
 *
 * RETURNS:
 *
 * None.
 *
 * ARGUMENTS:
 *
 * None.
 */
extern
void	dmalloc_log_unfreed(void);

/*
 * void dmalloc_log_changed
 *
 * DESCRIPTION:
 *
 * Dump the pointers that have changed since a point in time.
 *
 * RETURNS:
 *
 * mark -> Sets the point to compare against.  You can use
 * dmalloc_mark to get the current mark value which can later be
 * passed in here.  Pass in 0 to log what has changed since the
 * program started.
 *
 * not_freed_b -> Set to 1 to log the new pointers that are non-freed.
 *
 * free_b -> Set to 1 to log the new pointers that are freed.
 *
 * details_b -> Set to 1 to dump the individual pointers that have
 * changed otherwise the summaries will be logged.
 */
extern
void	dmalloc_log_changed(const unsigned long mark, const int not_freed_b,
			    const int free_b, const int details_b);

/*
 * void dmalloc_vmessage
 *
 * DESCRIPTION:
 *
 * Message writer with vprintf like arguments which adds a line to the
 * dmalloc logfile.
 *
 * RETURNS:
 *
 * None.
 *
 * ARGUMENTS:
 *
 * format -> Printf-style format statement.
 *
 * args -> Already converted pointer to a stdarg list.
 */
extern
void	dmalloc_vmessage(const char *format, va_list args);

/*
 * void dmalloc_message
 *
 * DESCRIPTION:
 *
 * Message writer with printf like arguments which adds a line to the
 * dmalloc logfile.
 *
 * RETURNS:
 *
 * None.
 *
 * ARGUMENTS:
 *
 * format -> Printf-style format statement.
 *
 * ... -> Variable argument list.
 */
extern
void	dmalloc_message(const char *format, ...)
#ifdef __GNUC__
  __attribute__ ((format (printf, 1, 2)))
#endif
;

/*
 * const char *dmalloc_strerror
 *
 * DESCRIPTION:
 *
 * Convert a dmalloc error code into its string equivalent.
 *
 * RETURNS:
 *
 * Success - String version of the error
 *
 * Failure - The string "unknown error"
 *
 * ARGUMENTS:
 *
 * error_num -> Error number we are converting.
 */
extern
const char	*dmalloc_strerror(const int error_num);


#ifdef __cplusplus
}
#endif

/*
 * alloc macros to provide for memory FILE/LINE debugging information.
 */

#ifndef DMALLOC_DISABLE

#undef malloc
#define malloc(size) \
  dmalloc_malloc(__FILE__, __LINE__, (size), DMALLOC_FUNC_MALLOC, 0, 0)
#undef calloc
#define calloc(count, size) \
  dmalloc_malloc(__FILE__, __LINE__, (count)*(size), DMALLOC_FUNC_CALLOC, 0, 0)
#undef realloc
#define realloc(ptr, size) \
  dmalloc_realloc(__FILE__, __LINE__, (ptr), (size), DMALLOC_FUNC_REALLOC, 0)
#undef recalloc
#define recalloc(ptr, size) \
  dmalloc_realloc(__FILE__, __LINE__, (ptr), (size), DMALLOC_FUNC_RECALLOC, 0)
#undef memalign
#define memalign(alignment, size) \
  dmalloc_malloc(__FILE__, __LINE__, (size), DMALLOC_FUNC_MEMALIGN, \
		 (alignment), 0 /* no xalloc */)
#undef valloc
#define valloc(size) \
  dmalloc_malloc(__FILE__, __LINE__, (size), DMALLOC_FUNC_VALLOC, \
		0 /* special case */, 0 /* no xalloc */)
#ifndef DMALLOC_STRDUP_MACRO
#undef strdup
#define strdup(str) \
  dmalloc_strdup(__FILE__, __LINE__, (str), 0)
#endif
#undef free
#define free(ptr) \
  dmalloc_free(__FILE__, __LINE__, (ptr), DMALLOC_FUNC_FREE)

#undef xmalloc
#define xmalloc(size) \
  dmalloc_malloc(__FILE__, __LINE__, (size), DMALLOC_FUNC_MALLOC, 0, 1)
#undef xcalloc
#define xcalloc(count, size) \
  dmalloc_malloc(__FILE__, __LINE__, (count)*(size), DMALLOC_FUNC_CALLOC, 0, 1)
#undef xrealloc
#define xrealloc(ptr, size) \
  dmalloc_realloc(__FILE__, __LINE__, (ptr), (size), DMALLOC_FUNC_REALLOC, 1)
#undef xrecalloc
#define xrecalloc(ptr, size) \
  dmalloc_realloc(__FILE__, __LINE__, (ptr), (size), DMALLOC_FUNC_RECALLOC, 1)
#undef xmemalign
#define xmemalign(alignment, size) \
  dmalloc_malloc(__FILE__, __LINE__, (size), DMALLOC_FUNC_MEMALIGN, \
		 (alignment), 1)
#undef xvalloc
#define xvalloc(size) \
  dmalloc_malloc(__FILE__, __LINE__, (size), DMALLOC_FUNC_VALLOC, 0, 1)
#undef xstrdup
#define xstrdup(str) \
  dmalloc_strdup(__FILE__, __LINE__, (str), 1)
#undef xfree
#define xfree(ptr) \
  dmalloc_free(__FILE__, __LINE__, (ptr), DMALLOC_FUNC_FREE)

#ifdef DMALLOC_FUNC_CHECK

/*
 * do debugging on the following functions.  this may cause compilation or
 * other problems depending on your architecture.
 */
#undef bcmp
#define bcmp(b1, b2, len)		_dmalloc_bcmp((b1), (b2), (len))
#undef bcopy
#define bcopy(from, to, len)		_dmalloc_bcopy((from), (to), (len))

#undef memcmp
#define memcmp(b1, b2, len)		_dmalloc_memcmp((b1), (b2), (len))
#undef memcpy
#define memcpy(to, from, len)		_dmalloc_memcpy((to), (from), (len))
#undef memset
#define memset(buf, ch, len)		_dmalloc_memset((buf), (ch), (len))

#undef index
#define index(str, ch)			_dmalloc_index((str), (ch))
#undef rindex
#define rindex(str, ch)			_dmalloc_rindex((str), (ch))

#undef strcat
#define strcat(to, from)		_dmalloc_strcat((to), (from))
#undef strcmp
#define strcmp(s1, s2)			_dmalloc_strcmp((s1), (s2))
#undef strlen
#define strlen(str)			_dmalloc_strlen((str))
#undef strtok
#define strtok(str, sep)		_dmalloc_strtok((str), (sep))

#undef bzero
#define bzero(buf, len)			_dmalloc_bzero((buf), (len))

#undef memccpy
#define memccpy(s1, s2, ch, len)	_dmalloc_memccpy((s1), (s2),(ch),(len))
#undef memchr
#define memchr(s1, ch, len)		_dmalloc_memchr((s1), (ch), (len))

#undef strchr
#define strchr(str, ch)			_dmalloc_strchr((str), (ch))
#undef strrchr
#define strrchr(str, ch)		_dmalloc_strrchr((str), (ch))

#undef strcpy
#define strcpy(to, from)		_dmalloc_strcpy((to), (from))
#undef strncpy
#define strncpy(to, from, len)		_dmalloc_strncpy((to), (from), (len))
#undef strcasecmp
#define strcasecmp(s1, s2)		_dmalloc_strcasecmp((s1), (s2))
#undef strncasecmp
#define strncasecmp(s1, s2, len)	_dmalloc_strncasecmp((s1), (s2), (len))
#undef strspn
#define strspn(str, list)		_dmalloc_strspn((str), (list))
#undef strcspn
#define strcspn(str, list)		_dmalloc_strcspn((str), (list))
#undef strncat
#define strncat(to, from, len)		_dmalloc_strncat((to), (from), (len))
#undef strncmp
#define strncmp(s1, s2, len)		_dmalloc_strncmp((s1), (s2), (len))
#undef strpbrk
#define strpbrk(str, list)		_dmalloc_strpbrk((str), (list))
#undef strstr
#define strstr(str, pat)		_dmalloc_strstr((str), (pat))

#endif /* DMALLOC_FUNC_CHECK */
#endif /* ! DMALLOC_DISABLE */

/*
 * feel free to add your favorite functions here and to arg_check.[ch]
 */

#ifdef __cplusplus
extern "C" {
#endif


/*
 * Dummy function for checking bcmp's arguments.
 */
extern
int	_dmalloc_bcmp(const void *b1, const void *b2, const DMALLOC_SIZE len);

/*
 * Dummy function for checking bcopy's arguments.
 */
extern
void	_dmalloc_bcopy(const void *from, void *to, const DMALLOC_SIZE len);

/*
 * Dummy function for checking memcmp's arguments.
 */
extern
int	_dmalloc_memcmp(const void *b1, const void *b2, const DMALLOC_SIZE len);

/*
 * Dummy function for checking memcpy's arguments.
 */
extern
void	*_dmalloc_memcpy(void *to, const void *from, const DMALLOC_SIZE len);

/*
 * Dummy function for checking memset's arguments.
 */
extern
void	*_dmalloc_memset(void *buf, const int ch, const DMALLOC_SIZE len);

/*
 * Dummy function for checking index's arguments.
 */
extern
char	*_dmalloc_index(const char *str, const char ch);

/*
 * Dummy function for checking rindex's arguments.
 */
extern
char	*_dmalloc_rindex(const char *str, const char ch);

/*
 * Dummy function for checking strcat's arguments.
 */
extern
char	*_dmalloc_strcat(char *to, const char *from);

/*
 * Dummy function for checking strcmp's arguments.
 */
extern
int	_dmalloc_strcmp(const char *s1, const char *s2);

/*
 * Dummy function for checking strlen's arguments.
 */
extern
DMALLOC_SIZE	_dmalloc_strlen(const char *str);

/*
 * Dummy function for checking strtok's arguments.
 */
extern
char	*_dmalloc_strtok(char *str, const char *sep);

/*
 * Dummy function for checking bzero's arguments.
 */
extern
void	_dmalloc_bzero(void *buf, const DMALLOC_SIZE len);

/*
 * Dummy function for checking memccpy's arguments.
 */
extern
void	*_dmalloc_memccpy(void *s1, const void *s2, const int ch,
			  const DMALLOC_SIZE len);

/*
 * Dummy function for checking memchr's arguments.
 */
extern
void	*_dmalloc_memchr(const void *s1, const int ch, const DMALLOC_SIZE len);

/*
 * Dummy function for checking strchr's arguments.
 */
extern
char	*_dmalloc_strchr(const char *str, const int ch);

/*
 * Dummy function for checking strrchr's arguments.
 */
extern
char	*_dmalloc_strrchr(const char *str, const int ch);

/*
 * Dummy function for checking strcpy's arguments.
 */
extern
char	*_dmalloc_strcpy(char *to, const char *from);

/*
 * Dummy function for checking strncpy's arguments.
 */
extern
char	*_dmalloc_strncpy(char *to, const char *from, const DMALLOC_SIZE len);

/*
 * Dummy function for checking strcasecmp's arguments.
 */
extern
int	_dmalloc_strcasecmp(const char *s1, const char *s2);

/*
 * Dummy function for checking strncasecmp's arguments.
 */
extern
int	_dmalloc_strncasecmp(const char *s1, const char *s2,
			     const DMALLOC_SIZE len);

/*
 * Dummy function for checking strspn's arguments.
 */
extern
int	_dmalloc_strspn(const char *str, const char *list);

/*
 * Dummy function for checking strcspn's arguments.
 */
extern
int	_dmalloc_strcspn(const char *str, const char *list);

/*
 * Dummy function for checking strncat's arguments.
 */
extern
char	*_dmalloc_strncat(char *to, const char *from, const DMALLOC_SIZE len);

/*
 * Dummy function for checking strncmp's arguments.
 */
extern
int	_dmalloc_strncmp(const char *s1, const char *s2,
			 const DMALLOC_SIZE len);

/*
 * Dummy function for checking strpbrk's arguments.
 */
extern
char	*_dmalloc_strpbrk(const char *str, const char *list);

/*
 * Dummy function for checking strstr's arguments.
 */
extern
char	*_dmalloc_strstr(const char *str, const char *pat);


#ifdef __cplusplus
}
#endif

#endif /* ! __DMALLOC_H__ */
