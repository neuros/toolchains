#ifndef _CRACK_H
#define _CRACK_H 1

/*
  source:
    /var/cvs/projects/debian/cracklib/debian/dpkg.src/cracklib2-dev.crack.h.in,v

  revision:
    @(#) cracklib2-dev.crack.h.in,v 1.3 1999/03/29 15:30:21 jplejacq Exp

  copyright:
    Copyright (C) 1998, 1999 Jean Pierre LeJacq <jplejacq@quoininc.com>

    Distributed under the GNU GENERAL PUBLIC LICENSE.

  description:
    cracklib - a pro-active password sanity library.
*/


static char const CRACKLIB_DICTPATH[] = "/usr/lib";
/*
  Directory and prefix of the password dictionary database used by
  several of the utilities provided with the cracklib package.  None
  of the functions in libcrack.a actually use this.  If you want to
  use the same database as these utilities, use this constant as the
  second argument to FascistCheck().
*/


extern char const * FascistCheck(char const password[], char const dictpath[]);
/*
  inputs:
    password is the user chosen potential password.

    dictpath is the full path name + filename prefix of the cracklib
    dictionary database, which is hardcoded to CRACKLIB_DICTPATH in
    several of the utility programs that come with cracklib.
  return:
    The NULL pointer for a good password, or a pointer to a diagnostic
    string if it is a bad password.
*/


#endif
