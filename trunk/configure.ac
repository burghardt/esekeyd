AC_INIT([ESE Key Deamon], [1.2.7], [krzysztof@burghardt.pl], [esekeyd])
AC_CONFIG_AUX_DIR(build)
AC_CONFIG_SRCDIR(src/esekeyd.c)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE
AM_CONDITIONAL([DOT_SVN], [test -f .svn/entries])

if test "$CFLAGS"; then
 CFLAGS_CUSTOM=yes
fi

dnl Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB

if test x"$cross_compiling" = xyes; then
    AC_CHECK_PROGS(CC_FOR_BUILD, gcc cc)
else
    CC_FOR_BUILD="$CC"
fi

dnl Checks for header files.
AC_CHECK_HEADERS(stdio.h stdlib.h unistd.h fcntl.h syslog.h string.h \
getopt.h sys/types.h sys/stat.h linux/input.h)

dnl Checks for library functions.
AC_CHECK_FUNCS(fprintf exit system fork \
syslog openlog closelog \
fopen feof fread fclose \
open read close getopt_long)

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
 src/Makefile
 doc/Makefile
 examples/Makefile
 ])
AC_OUTPUT
