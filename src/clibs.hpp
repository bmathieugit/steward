#ifndef __stew_clibs_hpp__
#define __stew_clibs_hpp__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <threads.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>

namespace stew::c
{
  /////////////
  /// IOLIB ///
  /////////////
  using ::access;
  using ::chmod;
  using ::chown;
  using ::closedir;
  using ::DIR;
  using ::fclose;
  using ::fflush;
  using ::FILE;
  using ::fopen;
  using ::fputc;
  using ::fwrite;
  using ::getgid;
  using ::getgrgid;
  using ::getgrnam;
  using ::getpwnam;
  using ::getpwuid;
  using ::getuid;
  using ::mkdir;
  using ::opendir;
  using ::putc;
  using ::readdir;
  using ::remove;
  using ::rename;

  int touch(const char *filename, int mode)
  {    
    int fd = open(filename, O_CREAT, mode);

    if (fd == 0)
    {
      return errno;
    }

    close(fd);
    return 0;
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
