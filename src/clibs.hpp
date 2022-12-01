#ifndef __stew_clibs_hpp__
#define __stew_clibs_hpp__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include <unistd.h>

namespace stew::c
{
  /////////////
  /// IOLIB ///
  /////////////

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
