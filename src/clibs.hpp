#ifndef __stew_clibs_hpp__
#define __stew_clibs_hpp__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <threads.h>
#include <assert.h>


namespace stew::c
{
  /////////////
  /// IOLIB ///
  /////////////

  using ::fclose;
  using ::fflush;
  using ::FILE;
  using ::fopen;
  using ::fputc;
  using ::fwrite;
  using ::putc;
  using ::remove;
  using ::rename;

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
