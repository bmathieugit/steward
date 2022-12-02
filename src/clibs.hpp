#ifndef __stew_clibs_hpp__
#define __stew_clibs_hpp__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

namespace stew::c
{
  /////////////
  /// IOLIB ///
  /////////////
  using ::access;
  using ::chmod;
  using ::closedir;
  using ::fclose;
  using ::fflush;
  using ::FILE;
  using ::fopen;
  using ::fputc;
  using ::fwrite;
  using ::opendir;
  using ::putc;
  using ::readdir;
  using ::rename;
  using ::remove;
  using ::mkdir;

  int touch(const char *filename, int mode) 
  {
    int fd = open(filename, mode);

    if (fd == -1) 
    {
      return 0;
    }

    close(fd);
    return errno;
  }

  /////////////////
  /// STRINGLIB ///
  /////////////////
  using ::isalnum;
  using ::isalpha;
  using ::isblank;
  using ::iscntrl;
  using ::isdigit;
  using ::isgraph;
  using ::islower;
  using ::isprint;
  using ::ispunct;
  using ::isspace;
  using ::isupper;
  using ::isxdigit;
  using ::strlen;
  using ::strtoul;

  using ::strcpy;
  using ::strncpy;



}

#endif
