/*
 * Copyright (c) 1994 1995 1996 ... 2023 2024
 *     John McCue <jmccue@jmcunx.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * jistat_j.c -- routines from lib j_lib2
 */

#ifndef _MSDOS
#include <sys/param.h>
#endif

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "jistat.h"

#ifdef _AIX
#define HAS_GETLINE 1
#endif
#ifdef __NetBSD_Version__
#define HAS_GETLINE 1
#endif
#ifdef OpenBSD
#define HAS_GETLINE 1
#endif
#ifdef __FreeBSD__
#define HAS_GETLINE 1
#endif
#ifdef linux
#define HAS_GETLINE 1
#endif

#define SIZE_GETLINE_BUF 256

/*
 * j2_f_exist() -- determines if a file exists
 */
int j2_f_exist(char *file_name)

{
  if (file_name == (char *) NULL)
    return((int) FALSE);

#ifdef _MSDOS
  if (access(file_name, 00) == -1)
    return (FALSE);
  else
    return (TRUE);
#else
  struct stat file_info;
  if (stat(file_name, &file_info) == 0)
    return (TRUE);
  else
    return (FALSE);
#endif

} /* j2_f_exist() */

/*
 * j2_get_prgname() -- return filename or default name.
 *
 *                     basename() is not used.  Some OS Systems do not
 *                     have basename() so this can be a basename()
 *                     replacement
 */
char *j2_get_prgname(char *argv_0, char *default_name)

{
  int i = 0, j = 0;

  if (argv_0 == (char *) NULL)
    {
      if (default_name == (char *) NULL)
	return((char *) NULL);
      else
	return(strdup(default_name));
    }

  /* basename(3) not in Coherent or MS-DOS */

  for (i = 0, j = 0; argv_0[i] != JLIB2_CHAR_NULL; i++)
    {
      if (argv_0[i] == '/')
	j = i + 1;
      else
	{
	  if (argv_0[i] == '\\')
	    j = i + 1;
	}
    }

  if (argv_0[j] == JLIB2_CHAR_NULL)
    if (default_name == (char *) NULL)
      return((char *) NULL);
    else
      return(strdup(default_name));
  else
    return(strdup(&(argv_0[j])));

} /* j2_get_prgname() */

/*
 * j2_bye_last() -- When the last byte = x, replaces it with NULL 
 *                  and returns string length
 */
long int j2_bye_last(char *x, char lastc)

{
  int i;
  
  if ( x == (char *) NULL )
    return(0L);
  if (strlen(x) < 1)
    return(0L);
  
  i = strlen(x) - (size_t) 1;
  
  if (x[i] == lastc)
    x[i] = JLIB2_CHAR_NULL;
  
  return((long int) strlen(x));
  
} /* j2_bye_last() */
