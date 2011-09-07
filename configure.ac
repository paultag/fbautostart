AC_INIT([fbautostart], [2.718281])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_HEADERS([config.h])     # not even really needed
AC_PROG_CXX

AC_CHECK_PROGS(regex_cmd, sed)
if test x$regex_cmd = "x"; then
  AC_MSG_ERROR([error. sed is required to build the data files.])
fi

AC_CONFIG_FILES([Makefile
doc/Makefile
])
AC_OUTPUT
