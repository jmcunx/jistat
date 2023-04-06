/*
 * Copyright (c) 2014 ... 2023 2024
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

#ifndef JISTAT_H

#define JISTAT_H "INCLUDED"

#define USG_MSG_JISTAT_1 "\tShow detail file information.\n"

#define ID_GROUP gid_t
#define ID_USER  uid_t

#ifndef JLIB2_CHAR_NULL
#define NO_JLIB 1
#define JLIB2_INT_NULL   ((int) '\0')
#define JLIB2_CHAR_NULL  ((char) '\0')
#define JLIB2_UCHAR_NULL ((unsigned char) '\0' )
#endif

#ifndef NULL
#define NULL '\0'
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef SSIZE_T
#define SSIZE_T ssize_t
#endif
#ifndef TAB_CHAR
#define TAB_CHAR 0x09
#endif

/*** values returned by file_type() ***/
#define SIZE_OPT 80
#define FILE_DESC_UNKNOWN       "Type Unknown"
#define FILE_DESC_DIRECTORY     "Directory"
#define FILE_DESC_REGULAR       "Regular"
#define FILE_DESC_CHAR_SPECIAL  "Character Special"
#define FILE_DESC_BLOCK_SPECIAL "Block Special"
#define FILE_DESC_PIPE          "Pipe"
#define FILE_DESC_SOCKET        "Socket"
#define FILE_DESC_SYMB_LINK     "Symbolic Link"
#define FILE_DESC_FIFO          "FIFO"

#define LIT_STAT_NAME   "File: %s\n"
#define LIT_STAT_TYPE   "      File Type:          %s\n"

#ifndef LIT_STAT_INODE
#ifdef __FreeBSD__
#  define LIT_STAT_INODE    "      Inode:              %d\n"
#  define LIT_STAT_DEVICE   "      Device:             %x\n"
#  define LIT_STAT_LINK     "      Link Count:         %u\n"
#endif
#endif

#ifndef LIT_STAT_INODE
#ifdef OpenBSD
#  define LIT_STAT_INODE    "      Inode:              %ld\n"
#  define LIT_STAT_DEVICE   "      Device:             %x\n"
#  define LIT_STAT_LINK     "      Link Count:         %u\n"
#endif
#endif

#ifndef LIT_STAT_INODE
#ifdef __NetBSD_Version__
#  define LIT_STAT_INODE    "      Inode:              %ld\n"
#  define LIT_STAT_DEVICE   "      Device:             %lux\n"
#  define LIT_STAT_LINK     "      Link Count:         %u\n"
#endif
#endif

#ifndef LIT_STAT_INODE
#  define LIT_STAT_INODE    "      Inode:              %ld\n"
#  define LIT_STAT_LINK     "      Link Count:         %lud\n"
#  define LIT_STAT_DEVICE   "      Device:             %lu\n"
#endif

#define FMT_01          "%o"
#define LIT_STAT_SIZE   "      Size:               %ld\n"
#define LIT_STAT_PERMIS "      Permissions:        %s\n"
#define LIT_STAT_OWNER  "      Owner:              %-7d %s\n"
#define LIT_STAT_GROUP  "      Group:              %-7d %s\n"
#define LIT_STAT_ACCESS "      Last Access:        %s"
#define LIT_STAT_MODIFY "      Last Modify:        %s"
#define LIT_STAT_STATUS "      Last Status Change: %s"

/*** File Names ***/
#define FILE_GROUP  "/etc/group"
#define FILE_PASSWD "/etc/passwd"

/*** Literals ***/
#define LIT_MIS_FIL "Cannot locate: %s\n"

#ifdef NO_JLIB
#define ARG_ERR           'e'  /* Output Error File                  */
#define ARG_FORCE         'f'  /* force create files                 */
#define ARG_HELP          'h'  /* Show Help                          */
#define ARG_OUT           'o'  /* Output File                        */
#define ARG_VERBOSE       'v'  /* Verbose                            */
#define ARG_VERSION       'V'  /* Show Version Information           */
#define LIT_C80           "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"
#define LIT_INFO_01       "\tBuild: %s %s\n"
#define LIT_INFO_02       "j_lib2 built:"
#define LIT_REV           "Revision"
#define MSG_ERR_E000      "Try '%s %c%c' for more information\n"
#define MSG_ERR_E002      "ERROR E002: Cannot open '%s' for write, processing aborted\n"
#define MSG_ERR_E005      "ERROR E005: need one or more files to process\n"
#define MSG_ERR_E025      "ERROR E025: File %s cannot be created, already exists\n"
#define SWITCH_CHAR       '-'
#define USG_MSG_ARG_ERR        "\t%c%c file\t\t: Write errors to file 'file', default stderr\n"
#define USG_MSG_ARG_FORCE      "\t%c%c\t\t: force create of files when found\n"
#define USG_MSG_ARG_HELP       "\t%c%c\t\t: Show brief help and exit\n"
#define USG_MSG_ARG_OUT        "\t%c%c file\t\t: Write output to file 'file', default stdout\n"
#define USG_MSG_ARG_VERBOSE_1  "\t%c%c\t\t: Show headings for file being processed\n"
#define USG_MSG_ARG_VERSION    "\t%c%c\t\t: Show revision information and exit\n"
#define USG_MSG_OPTIONS        "Options\n"
#define USG_MSG_USAGE          "usage:\t%s [OPTIONS] [FILES ...]\n"
#endif /* NO_JLIB */

#ifndef EXIT_LOGIC
#define EXIT_LOGIC  2 /* prog ran fine BUT need to pass warning code     */
                      /* for a logical reason (Example: files different, */
                      /* or file open errors)                            */
#endif /* EXIT_LOGIC */

/*** structures ***/
struct s_file_info
{
  FILE *fp;
  char *fname;
} ;

typedef struct
{
  char *prog_name;
  int warn_code;
  int verbose;        /* TRUE or FALSE */
  int force;          /* TRUE or FALSE */
  struct s_file_info out;
  struct s_file_info err;
} work_area;

#ifdef NO_JLIB
char *j2_get_prgname(char *argv_0, char *default_name);
int j2_f_exist(char *file_name);
long int j2_bye_last(char *x, char lastc);
#define j2_bye_nl(x)    j2_bye_last((x), '\n')
#define j2_bye_ctlm(x)  j2_bye_last((x), '\r')
#endif /* NO_JLIB */

#endif /* JISTAT_H */
