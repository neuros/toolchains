#ifndef H_RPMCLI
#define	H_RPMCLI

/** \ingroup rpmcli rpmbuild
 * \file lib/rpmcli.h
 */

#include "rpmlib.h"
#include "rpmurl.h"
#include "rpmmacro.h"

/** \ingroup rpmcli
 * Should version 3 packages be produced?
 */
/*@-redecl@*/
/*@unchecked@*/
extern int _noDirTokens;
/*@=redecl@*/

#ifdef __cplusplus
extern "C" {
#endif

/** \ingroup rpmcli
 * Popt option table for options shared by all modes and executables.
 */
/*@unchecked@*/
extern struct poptOption		rpmcliAllPoptTable[];

/*@unchecked@*/ /*@observer@*/ /*@null@*/
extern const char * rpmcliPipeOutput;

/*@unchecked@*/ /*@observer@*/ /*@null@*/
extern const char * rpmcliRcfile;

/*@unchecked@*/ /*@observer@*/ /*@null@*/
extern const char * rpmcliRootDir;

/**
 * Initialize most everything needed by an rpm CLI executable context.
 * @param argc			no. of args
 * @param argv			arg array
 * @param optionsTable		popt option table
 * @return			popt context (or NULL)
 */
/*@null@*/
poptContext
rpmcliInit(int argc, char *const argv[], struct poptOption * optionsTable)
	/*@globals rpmCLIMacroContext, rpmGlobalMacroContext, h_errno, stderr, 
		fileSystem, internalState @*/
	/*@modifies rpmCLIMacroContext, rpmGlobalMacroContext, stderr, 
		fileSystem, internalState @*/;

/**
 * Make sure that rpm configuration has been read.
 * @warning Options like --rcfile and --verbose must precede callers option.
 */
/*@mayexit@*/
void rpmcliConfigured(char * target)
	/*@globals rpmCLIMacroContext, rpmGlobalMacroContext, h_errno,
		fileSystem, internalState @*/
	/*@modifies rpmCLIMacroContext, rpmGlobalMacroContext,
		fileSystem, internalState @*/;

/**
 * Destroy most everything needed by an rpm CLI executable context.
 * @param optCon		popt context
 * @return			NULL always
 */
poptContext
rpmcliFini(/*@only@*/ /*@null@*/ poptContext optCon)
	/*@modifies optCon @*/;

/**
 * Common/global popt tokens used for command line option tables.
 */
#define	RPMCLI_POPT_NODEPS		-1025
#define	RPMCLI_POPT_FORCE		-1026
#define	RPMCLI_POPT_NOMD5		-1027
#define	RPMCLI_POPT_NOSCRIPTS		-1028
#define	RPMCLI_POPT_NOSIGNATURE		-1029
#define	RPMCLI_POPT_NODIGEST		-1030
#define	RPMCLI_POPT_NOHDRCHK		-1031
#define	RPMCLI_POPT_NOCONTEXTS		-1032

/* ==================================================================== */
/** \name RPMQV */
/*@{*/

/** \ingroup rpmcli
 * Query/Verify argument qualifiers.
 * @todo Reassign to tag values.
 */
typedef enum rpmQVSources_e {
    RPMQV_PACKAGE = 0,	/*!< ... from package name db search. */
    RPMQV_PATH,		/*!< ... from file path db search. */
    RPMQV_ALL,		/*!< ... from each installed package. */
    RPMQV_RPM, 		/*!< ... from reading binary rpm package. */
    RPMQV_GROUP,	/*!< ... from group db search. */
    RPMQV_WHATPROVIDES,	/*!< ... from provides db search. */
    RPMQV_WHATREQUIRES,	/*!< ... from requires db search. */
    RPMQV_TRIGGEREDBY,	/*!< ... from trigger db search. */
    RPMQV_DBOFFSET,	/*!< ... from database header instance. */
    RPMQV_SPECFILE,	/*!< ... from spec file parse (query only). */
    RPMQV_PKGID,	/*!< ... from package id (header+payload MD5). */
    RPMQV_HDRID,	/*!< ... from header id (immutable header SHA1). */
    RPMQV_FILEID,	/*!< ... from file id (file MD5). */
    RPMQV_TID		/*!< ... from install transaction id (time stamp). */
} rpmQVSources;

/** \ingroup rpmcli
 * Bit(s) for rpmVerifyFile() attributes and result.
 */
typedef enum rpmVerifyAttrs_e {
    RPMVERIFY_NONE	= 0,		/*!< */
    RPMVERIFY_MD5	= (1 << 0),	/*!< from %verify(md5) */
    RPMVERIFY_FILESIZE	= (1 << 1),	/*!< from %verify(size) */
    RPMVERIFY_LINKTO	= (1 << 2),	/*!< from %verify(link) */
    RPMVERIFY_USER	= (1 << 3),	/*!< from %verify(user) */
    RPMVERIFY_GROUP	= (1 << 4),	/*!< from %verify(group) */
    RPMVERIFY_MTIME	= (1 << 5),	/*!< from %verify(mtime) */
    RPMVERIFY_MODE	= (1 << 6),	/*!< from %verify(mode) */
    RPMVERIFY_RDEV	= (1 << 7),	/*!< from %verify(rdev) */
	/* bits 8-14 unused, reserved for rpmVerifyAttrs */
    RPMVERIFY_CONTEXTS	= (1 << 15),	/*!< verify: from --nocontexts */
	/* bits 16-22 used in rpmVerifyFlags */
	/* bits 23-27 used in rpmQueryFlags */
    RPMVERIFY_READLINKFAIL= (1 << 28),	/*!< readlink failed */
    RPMVERIFY_READFAIL	= (1 << 29),	/*!< file read failed */
    RPMVERIFY_LSTATFAIL	= (1 << 30),	/*!< lstat failed */
    RPMVERIFY_LGETFILECONFAIL	= (1 << 31)	/*!< lgetfilecon failed */
} rpmVerifyAttrs;
#define	RPMVERIFY_ALL		~(RPMVERIFY_NONE)
#define	RPMVERIFY_FAILURES	\
  (RPMVERIFY_LSTATFAIL|RPMVERIFY_READFAIL|RPMVERIFY_READLINKFAIL|RPMVERIFY_LGETFILECONFAIL)

/** \ingroup rpmcli
 * Bit(s) to control rpmQuery() operation, stored in qva_flags.
 * @todo Merge rpmQueryFlags, rpmVerifyFlags, and rpmVerifyAttrs?.
 */
typedef enum rpmQueryFlags_e {
/*@-enummemuse@*/
    QUERY_FOR_DEFAULT	= 0,		/*!< */
    QUERY_MD5		= (1 << 0),	/*!< from --nomd5 */
    QUERY_SIZE		= (1 << 1),	/*!< from --nosize */
    QUERY_LINKTO	= (1 << 2),	/*!< from --nolink */
    QUERY_USER		= (1 << 3),	/*!< from --nouser) */
    QUERY_GROUP		= (1 << 4),	/*!< from --nogroup) */
    QUERY_MTIME		= (1 << 5),	/*!< from --nomtime) */
    QUERY_MODE		= (1 << 6),	/*!< from --nomode) */
    QUERY_RDEV		= (1 << 7),	/*!< from --nodev */
	/* bits 8-14 unused, reserved for rpmVerifyAttrs */
    QUERY_CONTEXTS	= (1 << 15),	/*!< verify: from --nocontexts */
    QUERY_FILES		= (1 << 16),	/*!< verify: from --nofiles */
    QUERY_DEPS		= (1 << 17),	/*!< verify: from --nodeps */
    QUERY_SCRIPT	= (1 << 18),	/*!< verify: from --noscripts */
    QUERY_DIGEST	= (1 << 19),	/*!< verify: from --nodigest */
    QUERY_SIGNATURE	= (1 << 20),	/*!< verify: from --nosignature */
    QUERY_PATCHES	= (1 << 21),	/*!< verify: from --nopatches */
    QUERY_HDRCHK	= (1 << 22),	/*!< verify: from --nohdrchk */
/*@=enummemuse@*/
    QUERY_FOR_LIST	= (1 << 23),	/*!< query:  from --list */
    QUERY_FOR_STATE	= (1 << 24),	/*!< query:  from --state */
    QUERY_FOR_DOCS	= (1 << 25),	/*!< query:  from --docfiles */
    QUERY_FOR_CONFIG	= (1 << 26),	/*!< query:  from --configfiles */
    QUERY_FOR_DUMPFILES	= (1 << 27)	/*!< query:  from --dump */
} rpmQueryFlags;

#define	_QUERY_FOR_BITS	\
   (QUERY_FOR_LIST|QUERY_FOR_STATE|QUERY_FOR_DOCS|QUERY_FOR_CONFIG|\
    QUERY_FOR_DUMPFILES)

/** \ingroup rpmcli
 * Bit(s) from common command line options.
 */
/*@unchecked@*/
extern rpmQueryFlags rpmcliQueryFlags;

/** \ingroup rpmcli
 * Bit(s) to control rpmVerify() operation, stored in qva_flags.
 * @todo Merge rpmQueryFlags, rpmVerifyFlags, and rpmVerifyAttrs values?.
 */
typedef enum rpmVerifyFlags_e {
/*@-enummemuse@*/
    VERIFY_DEFAULT	= 0,		/*!< */
/*@=enummemuse@*/
    VERIFY_MD5		= (1 << 0),	/*!< from --nomd5 */
    VERIFY_SIZE		= (1 << 1),	/*!< from --nosize */
    VERIFY_LINKTO	= (1 << 2),	/*!< from --nolinkto */
    VERIFY_USER		= (1 << 3),	/*!< from --nouser */
    VERIFY_GROUP	= (1 << 4),	/*!< from --nogroup */
    VERIFY_MTIME	= (1 << 5),	/*!< from --nomtime */
    VERIFY_MODE		= (1 << 6),	/*!< from --nomode */
    VERIFY_RDEV		= (1 << 7),	/*!< from --nodev */
	/* bits 8-14 unused, reserved for rpmVerifyAttrs */
    VERIFY_CONTEXTS	= (1 << 15),	/*!< verify: from --nocontexts */
    VERIFY_FILES	= (1 << 16),	/*!< verify: from --nofiles */
    VERIFY_DEPS		= (1 << 17),	/*!< verify: from --nodeps */
    VERIFY_SCRIPT	= (1 << 18),	/*!< verify: from --noscripts */
    VERIFY_DIGEST	= (1 << 19),	/*!< verify: from --nodigest */
    VERIFY_SIGNATURE	= (1 << 20),	/*!< verify: from --nosignature */
    VERIFY_PATCHES	= (1 << 21),	/*!< verify: from --nopatches */
    VERIFY_HDRCHK	= (1 << 22),	/*!< verify: from --nohdrchk */
/*@-enummemuse@*/
    VERIFY_FOR_LIST	= (1 << 23),	/*!< query:  from --list */
    VERIFY_FOR_STATE	= (1 << 24),	/*!< query:  from --state */
    VERIFY_FOR_DOCS	= (1 << 25),	/*!< query:  from --docfiles */
    VERIFY_FOR_CONFIG	= (1 << 26),	/*!< query:  from --configfiles */
    VERIFY_FOR_DUMPFILES= (1 << 27)	/*!< query:  from --dump */
/*@=enummemuse@*/
	/* bits 28-31 used in rpmVerifyAttrs */
} rpmVerifyFlags;

#define	VERIFY_ATTRS	\
  ( VERIFY_MD5 | VERIFY_SIZE | VERIFY_LINKTO | VERIFY_USER | VERIFY_GROUP | \
    VERIFY_MTIME | VERIFY_MODE | VERIFY_RDEV | VERIFY_CONTEXTS )
#define	VERIFY_ALL	\
  ( VERIFY_ATTRS | VERIFY_FILES | VERIFY_DEPS | VERIFY_SCRIPT | VERIFY_DIGEST |\
    VERIFY_SIGNATURE | VERIFY_HDRCHK )

/** \ingroup rpmcli
 */
typedef struct rpmQVKArguments_s * QVA_t;

/** \ingroup rpmcli
 * Function to display iterator matches.
 *
 * @param qva		parsed query/verify options
 * @param ts		transaction set
 * @param h		header to use for query/verify
 * @return		0 on success
 */
typedef	int (*QVF_t) (QVA_t qva, rpmts ts, Header h)
	/*@globals fileSystem @*/
	/*@modifies qva, ts, fileSystem @*/;

/** \ingroup rpmcli
 * Function to query spec file.
 *
 * @param ts		transaction set
 * @param qva		parsed query/verify options
 * @param arg		query argument
 * @return		0 on success
 */
typedef	int (*QSpecF_t) (rpmts ts, QVA_t qva, const char * arg)
	/*@globals rpmGlobalMacroContext, h_errno,
		fileSystem, internalState @*/
	/*@modifies ts, qva, rpmGlobalMacroContext,
		fileSystem, internalState @*/;

/** \ingroup rpmcli
 * Describe query/verify/signature command line operation.
 */
struct rpmQVKArguments_s {
    rpmQVSources qva_source;	/*!< Identify CLI arg type. */
    int 	qva_sourceCount;/*!< Exclusive option check (>1 is error). */
    rpmQueryFlags qva_flags;	/*!< Bit(s) to control operation. */
    rpmfileAttrs qva_fflags;	/*!< Bit(s) to filter on attribute. */
/*@only@*/ /*@null@*/
    rpmdbMatchIterator qva_mi;	/*!< Match iterator on selected headers. */
/*@null@*/
    QVF_t qva_showPackage;	/*!< Function to display iterator matches. */
/*@null@*/
    QSpecF_t qva_specQuery;	/*!< Function to query spec file. */
/*@unused@*/
    int qva_verbose;		/*!< (unused) */
/*@only@*/ /*@null@*/
    const char * qva_queryFormat;/*!< Format for headerSprintf(). */
    int sign;			/*!< Is a passphrase needed? */
/*@observer@*/
    const char * passPhrase;	/*!< Pass phrase. */
/*@observer@*/ /*@null@*/
    const char * qva_prefix;	/*!< Path to top of install tree. */
    char	qva_mode;
		/*!<
		- 'q'	from --query, -q
		- 'Q'	from --querytags
		- 'V'	from --verify, -V
		- 'A'	from --addsign
		- 'I'	from --import
		- 'K'	from --checksig, -K
		- 'R'	from --resign
		*/
    char	qva_char;	/*!< (unused) always ' ' */
};

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct rpmQVKArguments_s rpmQVKArgs;

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct poptOption rpmQVSourcePoptTable[];

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern int specedit;

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct poptOption rpmQueryPoptTable[];

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct poptOption rpmVerifyPoptTable[];

/** \ingroup rpmcli
 * Display query/verify information for each header in iterator.
 *
 * This routine uses:
 *	- qva->qva_mi		rpm database iterator
 *	- qva->qva_showPackage	query/verify display routine
 *
 * @param qva		parsed query/verify options
 * @param ts		transaction set
 * @return		result of last non-zero showPackage() return
 */
int rpmcliShowMatches(QVA_t qva, rpmts ts)
	/*@globals rpmGlobalMacroContext, h_errno, fileSystem, internalState @*/
	/*@modifies qva, rpmGlobalMacroContext, fileSystem, internalState @*/;

/** \ingroup rpmcli
 * Display list of tags that can be used in --queryformat.
 * @param fp	file handle to use for display
 */
void rpmDisplayQueryTags(FILE * fp)
	/*@globals fileSystem @*/
	/*@modifies *fp, fileSystem @*/;

/** \ingroup rpmcli
 * Common query/verify source interface, called once for each CLI arg.
 *
 * This routine uses:
 *	- qva->qva_mi		rpm database iterator
 *	- qva->qva_showPackage	query/verify display routine
 *
 * @param qva		parsed query/verify options
 * @param ts		transaction set
 * @param arg		name of source to query/verify
 * @return		showPackage() result, 1 if rpmdbInitIterator() is NULL
 */
int rpmQueryVerify(QVA_t qva, rpmts ts, const char * arg)
	/*@globals rpmGlobalMacroContext, h_errno,
		fileSystem, internalState @*/
	/*@modifies qva, ts, rpmGlobalMacroContext,
		fileSystem, internalState @*/;

/** \ingroup rpmcli
 * Display results of package query.
 * @todo Devise a meaningful return code.
 * @param qva		parsed query/verify options
 * @param ts		transaction set
 * @param h		header to use for query
 * @return		0 always
 */
int showQueryPackage(QVA_t qva, rpmts ts, Header h)
	/*@globals rpmGlobalMacroContext, h_errno, fileSystem, internalState @*/
	/*@modifies ts, h, rpmGlobalMacroContext, fileSystem, internalState @*/;

/** \ingroup rpmcli
 * Display package information.
 * @todo hack: RPMQV_ALL can pass char ** arglist = NULL, not char * arg. Union?
 * @param ts		transaction set
 * @param qva		parsed query/verify options
 * @param argv		query argument(s) (or NULL)
 * @return		0 on success, else no. of failures
 */
int rpmcliQuery(rpmts ts, QVA_t qva, /*@null@*/ const char ** argv)
	/*@globals rpmGlobalMacroContext, h_errno,
		fileSystem, internalState @*/
	/*@modifies ts, qva, rpmGlobalMacroContext,
		fileSystem, internalState @*/;

/** \ingroup rpmcli
 * Verify file attributes (including MD5 sum).
 * @todo gnorpm and python bindings prevent this from being static.
 * @param ts		transaction set
 * @param fi		file info (with linked header and current file index)
 * @retval *res		bit(s) returned to indicate failure
 * @param omitMask	bit(s) to disable verify checks
 * @return		0 on success (or not installed), 1 on error
 */
/*@-incondefs@*/
int rpmVerifyFile(const rpmts ts, rpmfi fi,
		/*@out@*/ rpmVerifyAttrs * res, rpmVerifyAttrs omitMask)
	/*@globals h_errno, fileSystem, internalState @*/
	/*@modifies ts, fi, *res, fileSystem, internalState @*/
	/*@requires maxSet(res) >= 0 @*/;
/*@=incondefs@*/

/** \ingroup rpmcli
 * Display results of package verify.
 * @param qva		parsed query/verify options
 * @param ts		transaction set
 * @param h		header to use for verify
 * @return		result of last non-zero verify return
 */
int showVerifyPackage(QVA_t qva, rpmts ts, Header h)
	/*@globals rpmGlobalMacroContext, h_errno, fileSystem, internalState @*/
	/*@modifies ts, h, rpmGlobalMacroContext, fileSystem, internalState @*/;

/**
 * Check package and header signatures.
 * @param qva		parsed query/verify options
 * @param ts		transaction set
 * @param fd		package file handle
 * @param fn		package file name
 * @return		0 on success, 1 on failure
 */
int rpmVerifySignatures(QVA_t qva, rpmts ts, FD_t fd, const char * fn)
	/*@globals rpmGlobalMacroContext, h_errno, fileSystem, internalState @*/
	/*@modifies qva, ts, fd, rpmGlobalMacroContext, h_errno,
		fileSystem, internalState @*/;

/** \ingroup rpmcli
 * Verify package install.
 * @todo hack: RPMQV_ALL can pass char ** arglist = NULL, not char * arg. Union?
 * @param ts		transaction set
 * @param qva		parsed query/verify options
 * @param argv		verify argument(s) (or NULL)
 * @return		0 on success, else no. of failures
 */
int rpmcliVerify(rpmts ts, QVA_t qva, /*@null@*/ const char ** argv)
	/*@globals rpmGlobalMacroContext, h_errno,
		fileSystem, internalState @*/
	/*@modifies ts, qva, rpmGlobalMacroContext,
		fileSystem, internalState @*/;

/*@}*/
/* ==================================================================== */
/** \name RPMBT */
/*@{*/

/** \ingroup rpmcli
 * Describe build command line request.
 */
struct rpmBuildArguments_s {
    rpmQueryFlags qva_flags;	/*!< Bit(s) to control verification. */
    int buildAmount;		/*!< Bit(s) to control operation. */
/*@null@*/
    const char * buildRootOverride; /*!< from --buildroot */
/*@null@*/
    char * targets;		/*!< Target platform(s), comma separated. */
/*@observer@*/
    const char * passPhrase;	/*!< Pass phrase. */
/*@only@*/ /*@null@*/
    const char * cookie;	/*!< NULL for binary, ??? for source, rpm's */
    int force;			/*!< from --force */
    int noBuild;		/*!< from --nobuild */
    int noDeps;			/*!< from --nodeps */
    int noLang;			/*!< from --nolang */
    int shortCircuit;		/*!< from --short-circuit */
    int sign;			/*!< from --sign */
    int useCatalog;		/*!< from --usecatalog */
    char buildMode;		/*!< Build mode (one of "btBC") */
    char buildChar;		/*!< Build stage (one of "abcilps ") */
/*@observer@*/ /*@null@*/
    const char * rootdir;
};

/** \ingroup rpmcli
 */
typedef	struct rpmBuildArguments_s *	BTA_t;

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct rpmBuildArguments_s	rpmBTArgs;

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct poptOption		rpmBuildPoptTable[];

/*@}*/
/* ==================================================================== */
/** \name RPMEIU */
/*@{*/
/* --- install/upgrade/erase modes */

/*@unchecked@*/
extern int rpmcliPackagesTotal;
/*@unchecked@*/
extern int rpmcliHashesCurrent;
/*@unchecked@*/
extern int rpmcliHashesTotal;
/*@unchecked@*/
extern int rpmcliProgressCurrent;
/*@unchecked@*/
extern int rpmcliProgressTotal;

/** \ingroup rpmcli
 * The rpm CLI generic transaction callback handler.
 * @todo Remove headerSprintf() from the progress callback.
 * @deprecated Transaction callback arguments need to change, so don't rely on
 * this routine in the rpmcli API.
 *
 * @param arg		per-callback private data (e.g. an rpm header)
 * @param what		callback identifier
 * @param amount	per-callback progress info
 * @param total		per-callback progress info
 * @param key		opaque header key (e.g. file name or PyObject)
 * @param data		private data (e.g. rpmInstallInterfaceFlags)
 * @return		per-callback data (e.g. an opened FD_t)
 */
/*@null@*/
void * rpmShowProgress(/*@null@*/ const void * arg,
		const rpmCallbackType what,
		const unsigned long amount,
		const unsigned long total,
		/*@null@*/ fnpyKey key,
		/*@null@*/ void * data)
	/*@globals rpmcliHashesCurrent,
		rpmcliProgressCurrent, rpmcliProgressTotal,
		h_errno, fileSystem, internalState @*/
	/*@modifies rpmcliHashesCurrent,
		rpmcliProgressCurrent, rpmcliProgressTotal,
		fileSystem, internalState @*/;

/** \ingroup rpmcli
 * Install source rpm package.
 * @param ts		transaction set
 * @param arg		source rpm file name
 * @retval *specFilePtr	(installed) spec file name
 * @retval *cookie
 * @return		0 on success
 */
int rpmInstallSource(rpmts ts, const char * arg,
		/*@null@*/ /*@out@*/ const char ** specFilePtr,
		/*@null@*/ /*@out@*/ const char ** cookie)
	/*@globals rpmGlobalMacroContext, h_errno,
		fileSystem, internalState@*/
	/*@modifies ts, *specFilePtr, *cookie, rpmGlobalMacroContext,
		fileSystem, internalState @*/;

/** \ingroup rpmcli
 * Describe database command line requests.
 */
struct rpmInstallArguments_s {
    rpmtransFlags transFlags;
    rpmprobFilterFlags probFilter;
    rpmInstallInterfaceFlags installInterfaceFlags;
    rpmEraseInterfaceFlags eraseInterfaceFlags;
    rpmQueryFlags qva_flags;	/*!< from --nodigest/--nosignature */
    uint_32 rbtid;		/*!< from --rollback */
    int numRelocations;
    int noDeps;
    int incldocs;
/*@owned@*/ /*@null@*/
    rpmRelocation * relocations;
/*@null@*/
    const char * prefix;
/*@observer@*/ /*@null@*/
    const char * rootdir;
};

/** \ingroup rpmcli
 * Install/upgrade/freshen binary rpm package.
 * @param ts		transaction set
 * @param ia		mode flags and parameters
 * @param fileArgv	array of package file names (NULL terminated)
 * @return		0 on success
 */
int rpmInstall(rpmts ts, struct rpmInstallArguments_s * ia,
		/*@null@*/ const char ** fileArgv)
	/*@globals rpmcliPackagesTotal, rpmGlobalMacroContext, h_errno,
		fileSystem, internalState@*/
	/*@modifies ts, ia, rpmcliPackagesTotal, rpmGlobalMacroContext,
		fileSystem, internalState @*/;

/** \ingroup rpmcli
 * Erase binary rpm package.
 * @param ts		transaction set
 * @param ia		control args/bits
 * @param argv		array of package file names (NULL terminated)
 * @return		0 on success
 */
int rpmErase(rpmts ts, struct rpmInstallArguments_s * ia,
		/*@null@*/ const char ** argv)
	/*@globals rpmGlobalMacroContext, h_errno, fileSystem, internalState @*/
	/*@modifies ts, ia, rpmGlobalMacroContext,
		fileSystem, internalState @*/;

/**
 * A rollback transaction id element.
 */
/*@-fielduse@*/
typedef /*@abstract@*/ struct IDT_s {
    unsigned int instance;	/*!< installed package transaction id. */
/*@owned@*/ /*@null@*/
    const char * key;		/*! removed package file name. */
    Header h;			/*!< removed package header. */
    union {
	uint_32 u32;		/*!< install/remove transaction id */
    } val;
} * IDT;
/*@=fielduse@*/

/**
 * A rollback transaction id index.
 */
typedef /*@abstract@*/ struct IDTindex_s {
    int delta;			/*!< no. elements to realloc as a chunk. */
    int size;			/*!< size of id index element. */
    int alloced;		/*!< current number of elements allocated. */
    int nidt;			/*!< current number of elements initialized. */
/*@only@*/ /*@null@*/
    IDT idt;			/*!< id index elements. */
} * IDTX;

/**
 * Destroy id index.
 * @param idtx		id index
 * @return		NULL always
 */
/*@null@*/
IDTX IDTXfree(/*@only@*/ /*@null@*/ IDTX idtx)
	/*@modifies idtx @*/;

/**
 * Create id index.
 * @return		new id index
 */
/*@only@*/
IDTX IDTXnew(void)
	/*@*/;

/**
 * Insure that index has room for "need" elements.
 * @param idtx		id index
 * @param need		additional no. of elements needed
 * @return 		id index (with room for "need" elements)
 */
/*@only@*/ /*@null@*/
IDTX IDTXgrow(/*@only@*/ /*@null@*/ IDTX idtx, int need)
	/*@modifies idtx @*/;

/**
 * Sort tag (instance,value) pairs.
 * @param idtx		id index
 * @return 		id index
 */
/*@only@*/ /*@null@*/
IDTX IDTXsort(/*@only@*/ /*@null@*/ IDTX idtx)
	/*@modifies idtx @*/;

/**
 * Load tag (instance,value) pairs from rpm databse, and return sorted id index.
 * @param ts		transaction set
 * @param tag		rpm tag
 * @return 		id index
 */
/*@only@*/ /*@null@*/
IDTX IDTXload(rpmts ts, rpmTag tag)
	/*@globals rpmGlobalMacroContext, h_errno, fileSystem, internalState @*/
	/*@modifies ts, rpmGlobalMacroContext, fileSystem, internalState  @*/;

/**
 * Load tag (instance,value) pairs from packages, and return sorted id index.
 * @param ts		transaction set
 * @param globstr	glob expression
 * @param tag		rpm tag
 * @return 		id index
 */
/*@only@*/ /*@null@*/
IDTX IDTXglob(rpmts ts, const char * globstr, rpmTag tag)
	/*@globals rpmGlobalMacroContext, h_errno, fileSystem, internalState @*/
	/*@modifies ts, rpmGlobalMacroContext, fileSystem, internalState @*/;

/** \ingroup rpmcli
 * Rollback transactions, erasing new, reinstalling old, package(s).
 * @param ts		transaction set
 * @param ia		mode flags and parameters
 * @param argv		array of arguments (NULL terminated)
 * @return		0 on success
 */
int rpmRollback(rpmts ts, struct rpmInstallArguments_s * ia,
		/*@null@*/ const char ** argv)
	/*@globals rpmcliPackagesTotal, rpmGlobalMacroContext, h_errno,
		fileSystem, internalState @*/
	/*@modifies ts, ia, rpmcliPackagesTotal, rpmGlobalMacroContext,
		fileSystem, internalState @*/;

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct rpmInstallArguments_s rpmIArgs;

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct poptOption rpmInstallPoptTable[];

/*@}*/
/* ==================================================================== */
/** \name RPMDB */
/*@{*/
/* --- database modes */

/** \ingroup rpmcli
 * Describe database command line requests.
 */
struct rpmDatabaseArguments_s {
    int init;			/*!< from --initdb */
    int rebuild;		/*!< from --rebuilddb */
    int verify;			/*!< from --verifydb */
};

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct rpmDatabaseArguments_s rpmDBArgs;

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct poptOption rpmDatabasePoptTable[];

/*@}*/
/* ==================================================================== */
/** \name RPMK */
/*@{*/

/** \ingroup rpmcli
 * Import public key packet(s).
 * @todo Implicit --update policy for gpg-pubkey headers.
 * @param ts		transaction set
 * @param pkt		pgp pubkey packet(s)
 * @param pktlen	pgp pubkey length
 * @return		RPMRC_OK/RPMRC_FAIL
 */
rpmRC rpmcliImportPubkey(const rpmts ts,
		const unsigned char * pkt, ssize_t pktlen)
	/*@globals RPMVERSION, rpmGlobalMacroContext, h_errno,
		fileSystem, internalState @*/
	/*@modifies ts, rpmGlobalMacroContext,
		fileSystem, internalState @*/;

/** \ingroup rpmcli
 * Bit(s) to control rpmReSign() operation.
 */
/*@-typeuse@*/
typedef enum rpmSignFlags_e {
    RPMSIGN_NONE		= 0,
    RPMSIGN_CHK_SIGNATURE	= 'K',	/*!< from --checksig */
    RPMSIGN_NEW_SIGNATURE	= 'R',	/*!< from --resign */
    RPMSIGN_ADD_SIGNATURE	= 'A',	/*!< from --addsign */
    RPMSIGN_IMPORT_PUBKEY	= 'I',	/*!< from --import */
} rpmSignFlags;
/*@=typeuse@*/

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct poptOption rpmSignPoptTable[];

/** \ingroup rpmcli
 * Create/Modify/Check elements from signature header.
 * @param ts		transaction set
 * @param qva		mode flags and parameters
 * @param argv		array of arguments (NULL terminated)
 * @return		0 on success
 */
int rpmcliSign(rpmts ts, QVA_t qva, /*@null@*/ const char ** argv)
	/*@globals RPMVERSION, rpmGlobalMacroContext, h_errno,
		fileSystem, internalState @*/
	/*@modifies ts, qva, rpmGlobalMacroContext,
		fileSystem, internalState @*/;

/*@}*/

#ifdef __cplusplus
}
#endif

#endif	/* H_RPMCLI */
