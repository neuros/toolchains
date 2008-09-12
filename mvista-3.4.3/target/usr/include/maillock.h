/*
 *	Copyright (C) 1999 Miquel van Smoorenburg
 *
 *	This library is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU Library General Public License as
 *	published by the Free Software Foundation; either version 2 of the
 *	License, or (at your option) any later version.
 *
 *	On Debian GNU/Linux systems, the complete text of the GNU Library
 *	General Public License can be found in `/usr/doc/copyright/LGPL'.
 *	You can also find a copy on the GNU website at http://www.gnu.org/
 */
#ifndef _MAILLOCK_H
#define _MAILLOCK_H

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef _PATH_MAILDIR
#include <paths.h>
#endif

#define MAILDIR		(_PATH_MAILDIR "/")

/*
 *	Prototypes.
 */
int	maillock(const char *name, int retries);
void	touchlock();
void	mailunlock();

#ifdef  __cplusplus
}
#endif

#include <lockfile.h>

#endif /* _MAILLOCK_H */
