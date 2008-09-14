AC_INIT(src/RL_glue.c)
AC_CONFIG_AUX_DIR(config)
AM_INIT_AUTOMAKE(rlglue,3.0-beta)
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_LIBTOOL

AC_CONFIG_HEADERS(config/config.h)


AC_PATH_PROG(svnversioncommand, svnversion)
if test "X$svnversioncommand" = "X" || test `$svnversioncommand -n '.'` = "exported"; then
AC_DEFINE_UNQUOTED(SVN_VERSION, ["UNKNOWN"], [repository svn version])
else
AC_DEFINE_UNQUOTED(SVN_VERSION, ["`svnversion -n`"], [repository svn version])
fi

AC_OUTPUT(Makefile src/Makefile tests/Makefile)
