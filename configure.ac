AC_INIT([ESE Key Deamon], [1.2.3], [krzysztof@burghardt.pl], [esekeyd])
AC_CONFIG_AUX_DIR(build)
AC_CONFIG_SRCDIR(src/esekeyd.c)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE

if test "$CFLAGS"; then
 CFLAGS_CUSTOM=yes
fi

dnl Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB

dnl Checks for header files.
AC_CHECK_HEADERS(stdio.h stdlib.h unistd.h fcntl.h syslog.h string.h \
sys/types.h sys/stat.h linux/input.h)

dnl Check for getopt_long in standard library
AC_CHECK_FUNCS(getopt_long,, [AC_LIBOBJ(getopt)
                              AC_LIBOBJ(getopt1)])

dnl Checks for library functions.
AC_CHECK_FUNCS(fprintf exit system fork \
syslog openlog closelog \
fopen feof fread fclose \
open read close)
dnl AC_CHECK_LIB(socket, socket)

dnl Use option --enable-gcc-debug to enable GCC debug code. 
AC_ARG_ENABLE(gcc-debug,
AC_HELP_STRING([--enable-gcc-debug],
        [enable GCC DEBUG code]),
        [enable_gcc_debug=yes],
        [enable_gcc_debug=no])
if test "$enable_gcc_debug" = "yes" && (test "$GCC" = "yes"); then
 if test -z "$CFLAGS_CUSTOM"; then
  CFLAGS="-Wall -O0 -ggdb -DDEBUGGER"
 else
  CFLAGS="$CFLAGS -O0 -ggdb -DDEBUGGER"
 fi
 AC_MSG_RESULT([Enabling GCC DEBUG support...])
else
 if test -z "$CFLAGS_CUSTOM" && (test "$GCC" = "yes"); then
  CFLAGS="-Wall -O2"
 fi
fi

AC_CONFIG_FILES([Makefile
 lib/Makefile
 examples/Makefile
 old/Makefile
 src/Makefile
 doc/Makefile
 debian/Makefile
 ])
AC_OUTPUT
