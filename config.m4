dnl $Id$
dnl config.m4 for extension setproctitle

PHP_ARG_ENABLE(setproctitle, whether to enable setproctitle support,
[  --enable-setproctitle   Enable setproctitle support])

if test "$PHP_SETPROCTITLE" != "no"; then
    AC_CACHE_VAL(php_cv_system_provides_setproctitle_call,[
    AC_CHECK_FUNCS(setproctitle, [
        php_cv_system_provides_setproctitle_call=yes
        break
    ],[
        php_cv_system_provides_setproctitle_call=no
    ])])

    AC_MSG_CHECKING([for prctl])

    AC_TRY_COMPILE([ #include <sys/prctl.h> ], [prctl(0, 0, 0, 0, 0);], [
        AC_DEFINE([HAVE_PRCTL], 1, [do we have prctl?])
        AC_MSG_RESULT([yes])
    ], [
        AC_MSG_RESULT([no])
    ])

    AC_MSG_CHECKING([if your OS provides a native way to change a process title])
    if test "$php_cv_system_provides_setproctitle_call" = "yes"; then
        AC_MSG_RESULT(yes)
        AC_DEFINE(HAVE_SETPROCTITLE,1, [Define if your system has setproctitle])
    else
        AC_MSG_RESULT(no)
    fi
  PHP_NEW_EXTENSION(setproctitle, setproctitle.c, $ext_shared)
fi
