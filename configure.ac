AC_INIT(src/RL_glue.c)
AC_CONFIG_AUX_DIR(config)
AM_INIT_AUTOMAKE(rlglue,3.0-beta)
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_LIBTOOL






dnl see whether subversion is installed
AC_PATH_PROG(svnversioncommand, svnversion)

dnl use svnversion to record the current repository revision only if
dnl subversion is installed and we are in a working copy
if test "X$svnversioncommand" = "X" || test `$svnversioncommand -n '.'` = "exported"; then
mysvnversion="exported"
else
mysvnversion="svnversion -n '.'"
fi

dnl this is used to make sure we create the file svn_dist_version only if
dnl we have svnversion and if we are in a svn working copy. In all other
dnl cases the distributed copy of svn_dist_version should be used
AM_CONDITIONAL(HAVESVNWC, test "X$svnversioncommand" != "X" && test `$svnversioncommand -n '.'` != "exported")


AC_SUBST(mysvnversion)

AC_OUTPUT(Makefile src/Makefile tests/Makefile)
