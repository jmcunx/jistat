/*
 * Copyright (c) 2014 ... 2021 2022
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

#ifndef _MSDOS
#include <sys/param.h>
#endif
#include <stdio.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>

#ifdef OpenBSD
#include <err.h>
#endif
#ifdef __FreeBSD_version
#include <err.h>
#endif
#ifdef __NetBSD_Version__
#include <err.h>
#endif

#include <j_lib2.h>
#include <j_lib2m.h>

#define USG_MSG_JISTAT_1 "\tShow detail file information.\n"

#define ID_GROUP gid_t
#define ID_USER  uid_t

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
#  define LIT_STAT_DEVICE   "      Device:             %Lux\n"
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

/*
 * split_rec()
 */
int split_rec(int fpos, char **gname, ID_GROUP gid, char *buf)

{
  int i, fcount;
  ID_GROUP id_test;

  (*gname) = (char *) NULL;

  fcount = 0;
  i = 0;
  while (buf[i] != JLIB2_CHAR_NULL)
    {
      if (buf[i] == ':')
        {
	  buf[i] = JLIB2_CHAR_NULL;
	  fcount++;
	}
      i++;
      if ((fcount == fpos) && (buf[i] != ':'))
	{
	  id_test = atol(&(buf[i]));
	  if (id_test == gid)
	    {
	      (*gname) = buf;
	      break;
	    }
	}
    }

  if ((*gname) == (char *) NULL)
    return((int) FALSE);
  return((int) TRUE);

} /* split_rec() */

/*
 * get_file_desc() -- Get File Type
 */
char *get_file_desc(file_stat)

	struct stat *file_stat;

{

  int file_type;

  file_type = (int) (file_stat->st_mode & S_IFMT);

  switch (file_type)
    {
    case (int) S_IFREG:
      return(FILE_DESC_REGULAR);
      break;
    case (int) S_IFDIR:
      return(FILE_DESC_DIRECTORY);
      break;
#   ifdef S_IFCHR
    case (int) S_IFCHR:
      return(FILE_DESC_CHAR_SPECIAL);
      break;
#   endif
#   ifdef S_IFBLK
    case (int) S_IFBLK:
      return(FILE_DESC_BLOCK_SPECIAL);
      break;
#   endif
#   ifdef S_IFLNK
    case (int) S_IFLNK:
      return(FILE_DESC_SYMB_LINK);
      break;
#   endif
#   ifdef S_IFPIP
    case (int) S_IFPIP:
      return(FILE_DESC_PIPE);
      break;
#   endif
#   ifdef S_IFSOCK
#   if S_IFSOCK != S_IFPIP
    case (int) S_IFSOCK:
      return(FILE_DESC_SOCKET);
      break;
#   endif
#   endif
#   ifdef S_IFIFO
#   if S_IFIFO != S_IFPIP
    case (int) S_IFIFO:
      return(FILE_DESC_FIFO);
      break;
#   endif
#   endif
    default:
      return(FILE_DESC_UNKNOWN);
    } /*  switch() */

} /* get_file_desc() */

/*
 * get_username()
 */
char *get_username(ID_USER uid)

{
  FILE *fp;
  static char buf[2048];
  char *uname;

  uname = (char *) NULL;

  fp = fopen(FILE_PASSWD, "r");
  if (fp == (FILE *) NULL)
    return((char *) NULL);

  /* read and get name */
  while ( fgets(buf, 2048, fp) != (char *) NULL)
    {
      j2_bye_nl(buf);
      if ( split_rec(2, &uname, uid, buf) )
        break;
    }

  fclose(fp);

  return(uname);

} /* get_username() */

/*
 * get_groupname()
 */
char *get_groupname(ID_GROUP gid)

{
  FILE *fp;
  static char buf[2048];
  char *gname;

  gname = (char *) NULL;

  fp = fopen(FILE_GROUP, "r");
  if (fp == (FILE *) NULL)
    return((char *) NULL);

  /* read and get name */
  while ( fgets(buf, 2048, fp) != (char *) NULL)
    {
      j2_bye_nl(buf);
      if ( split_rec(2, &gname, gid, buf) )
        break;
    }


  fclose(fp);

  return(gname);

} /* get_groupname() */

/*
 * show_stat()
 */
void show_stat(FILE *fp, char *fname, struct stat *s)

{
  int  size;
  char *gname;
  char *uname;
  char *ftype;
  char *mode;
  char fmt_mode[1024];

  ftype = get_file_desc(s);
  gname = get_groupname(s->st_gid);
  uname = get_username(s->st_uid);

  snprintf(fmt_mode, 1023, FMT_01, s->st_mode);

  size = strlen(fmt_mode);
  if (size > 4)
    mode = &(fmt_mode[(size - 4)]);
  else
    mode = fmt_mode;

  fprintf(fp, LIT_STAT_NAME,   fname);
  fprintf(fp, LIT_STAT_TYPE,   ftype);
  fprintf(fp, LIT_STAT_INODE,  (long int) s->st_ino);
  fprintf(fp, LIT_STAT_LINK,   s->st_nlink);
  fprintf(fp, LIT_STAT_DEVICE, s->st_dev);
  fprintf(fp, LIT_STAT_PERMIS, mode);
  fprintf(fp, LIT_STAT_OWNER,  s->st_uid,
              (uname == (char *) NULL ? " " : uname));
  fprintf(fp, LIT_STAT_GROUP,  s->st_gid,
              (gname == (char *) NULL ? " " : gname));
  fprintf(fp, LIT_STAT_SIZE,   (long int) s->st_size);

  fprintf(fp, LIT_STAT_ACCESS, ctime(&(s->st_atime)));
  fprintf(fp, LIT_STAT_MODIFY, ctime(&(s->st_mtime)));
  fprintf(fp, LIT_STAT_STATUS, ctime(&(s->st_ctime)));

} /* show_stat() */

/*
 * show_rev()
 */
int show_rev(FILE *fp, char *pname)
{

  fprintf(fp,"%s %s:\n", pname, LIT_REV);
  fprintf(fp,"\t%s %s\n", LIT_INFO_02, j2_get_build());

#ifdef OSTYPE
  fprintf(fp,"\t%s\n", OSTYPE);
#endif
#ifdef PRETTY_NAME
  fprintf(fp,"\t%s\n",PRETTY_NAME);
#endif  /* PRETTY_NAME  */

  fprintf(fp, LIT_INFO_01, __DATE__, __TIME__);

  exit(EXIT_FAILURE);

} /* show_rev() */

/*
 * show_help()
 */
int show_help(FILE *fp, char *pname)

{

  fprintf(fp, USG_MSG_USAGE, pname);
  fprintf(fp, USG_MSG_JISTAT_1);
  fprintf(fp, USG_MSG_OPTIONS);
  fprintf(fp, USG_MSG_ARG_ERR,       SWITCH_CHAR, ARG_ERR);
  fprintf(fp, USG_MSG_ARG_FORCE,     SWITCH_CHAR, ARG_FORCE);
  fprintf(fp, USG_MSG_ARG_HELP,      SWITCH_CHAR, ARG_HELP);
  fprintf(fp, USG_MSG_ARG_OUT,       SWITCH_CHAR, ARG_OUT);
  fprintf(fp, USG_MSG_ARG_VERBOSE_1, SWITCH_CHAR, ARG_VERBOSE);
  fprintf(fp, USG_MSG_ARG_VERSION,   SWITCH_CHAR, ARG_VERSION);

  return(EXIT_FAILURE);

} /* show_help() */

/*
 * init_finfo() -- initialize out file structure
 */
void init_finfo(struct s_file_info *f)

{

  f->fp    = (FILE *) NULL;
  f->fname = (char *) NULL;

} /* init_finfo() */

/*
 * open_out() -- save the file anem and check status
 */
int open_out(FILE *wfp, struct s_file_info *f, char *fname, int force)

{
  int errsave;

  if (fname == (char *) NULL)
    return(EXIT_SUCCESS);

  if (force == (int) FALSE)
    {
      if ( j2_f_exist(fname) )
	{
	  fprintf(wfp, MSG_ERR_E025, fname);
	  return(EXIT_FAILURE);
	}
    }


  f->fp = fopen(fname, "w");
  errsave = errno;
  if (f->fp == (FILE *) NULL)
    {
      f->fp = stderr;  /* needs to be something */
      fprintf(wfp, MSG_ERR_E002, fname);
      fprintf(wfp, "\t%s\n", strerror(errsave));
      return(EXIT_FAILURE);
    }

  /*** success, save file name ***/
  f->fname = strdup(fname);
  return(EXIT_SUCCESS);

} /* open_out() */

/*
 * close_out() -- close output
 */
void close_out(struct s_file_info *f, int use_stdout)
{

  if (f->fname != (char *) NULL)
    {
      fclose(f->fp);
      free(f->fname);
      init_finfo(f);
      if (use_stdout == (int) TRUE)
	f->fp = stdout;
      else
	f->fp = stderr;
    }

} /* close_out() */

/*
 * get_args() -- load arguments
 */
void get_args(int argc, char **argv, work_area *w)

{
  char ckarg[SIZE_OPT];
  int opt;
  char *fout = (char *) NULL; 
  char *ferr = (char *) NULL; 
  int  display_help      = (int) FALSE;
  int  display_rev       = (int) FALSE;
  int ok;

  w->warn_code = EXIT_SUCCESS;
  w->force     = (int) FALSE;
  w->verbose   = (int) FALSE;
  init_finfo(&(w->out));
  init_finfo(&(w->err));
  w->out.fp    = stdout;
  w->err.fp    = stderr;
  w->prog_name = j2_get_prgname(argv[0], "jistat");

  snprintf(ckarg, SIZE_OPT, "%c:%c:%c%c%c%c", 
          ARG_ERR, ARG_OUT, 
          ARG_HELP, ARG_VERSION, ARG_VERBOSE, ARG_FORCE);

  while ((opt = getopt(argc, argv, ckarg)) != -1)
    {
      switch (opt)
	{
	  case ARG_FORCE:
	    w->force = (int) TRUE;
	    break;
	  case ARG_HELP:
	    display_help = (int) TRUE;
	    break;
	  case ARG_VERSION:
	    display_rev  = (int) TRUE;
	    break;
	  case ARG_VERBOSE:
	    w->verbose = (int) TRUE;
	    break;
	  case ARG_OUT:
	    fout = optarg;
	    break;
	  case ARG_ERR:
	    ferr = optarg;
	    break;
	  default:
	    fprintf(stderr, MSG_ERR_E000, w->prog_name, SWITCH_CHAR, ARG_HELP);
	    exit(EXIT_FAILURE);
	    break;
	}
    }

  /*** if necessary - save stdout/err files and open ***/
  ok = EXIT_SUCCESS;
  ok = open_out(stderr, &(w->err), ferr, w->force);
  if (ok == EXIT_SUCCESS)
    ok = open_out(w->err.fp, &(w->out), fout, w->force);
  if (ok != EXIT_SUCCESS)
    {
      fprintf(w->err.fp, MSG_ERR_E000, w->prog_name, SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }

  /*** show help/rev and exit ? ***/
  ok = EXIT_SUCCESS;
  if (display_help == (int) TRUE)
    ok = show_help(w->out.fp, w->prog_name);
  if (display_rev == (int) TRUE)
    ok = show_rev(w->out.fp, w->prog_name);
  if (ok != EXIT_SUCCESS)
    {
      close_out(&(w->out), (int) TRUE);
      close_out(&(w->err), (int) FALSE);
      exit(ok);
    }

  /*** count files to default verbose ***/
  if (w->verbose != (int) TRUE)
    {
      for (ok = optind; ok < argc; ok++)
	;
      ok -= optind;
      if (ok > 1)
	w->verbose = (int) TRUE;
    }

} /* get_args() */

/*
 * process_file() -- process a specific file
 */
void process_file(char *fname, work_area *w)

{
  struct stat s;

  if (w->verbose)
    fprintf(w->out.fp, "%s\n%s\n%s\n", LIT_C80, (fname == (char *) NULL ? "stdin" : fname), LIT_C80);

  if (stat(fname, &s)  == -1)
    {
      fprintf(w->out.fp, LIT_MIS_FIL, fname);
      w->warn_code = EXIT_LOGIC;
    }
  else
    show_stat(w->out.fp, fname, &s);

} /* process_file() */

/*
 * main()
 */
int main(int argc, char **argv)

{
  int i;
  work_area w;

#ifdef OpenBSD
  if(pledge("stdio rpath wpath cpath",NULL) == -1)
    err(1,"pledge\n");
#endif

  get_args(argc, argv, &w);

  /*** process files ***/
  for (i = optind; i < argc; i++)
    process_file(argv[i], &w);

  if (i == optind)
    fprintf(w.err.fp, MSG_ERR_E005);

  /*** done ***/
  close_out(&(w.out), (int) TRUE);
  close_out(&(w.err), (int) FALSE);
  if (w.prog_name != (char *) NULL)
    free(w.prog_name);
  exit(w.warn_code);

} /* main() */
