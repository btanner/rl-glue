AC_INIT(src/RL_glue.c)
AC_CONFIG_AUX_DIR(config)
AM_INIT_AUTOMAKE(rlglue,3.0-RC2)
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_LIBTOOL


AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_FUNCS([gettimeofday])

AC_CONFIG_HEADERS(config/config.h)

AC_OUTPUT(Makefile src/Makefile tests/Makefile)
