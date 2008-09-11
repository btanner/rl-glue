# AM_CFLAGS = -I$(top_srcdir)/src
# 
# bin_PROGRAMS = test1
# test1_SOURCES = constant_environment.c	echo_agent.c		experiment.c useful_functions.c useful_functions.h
# 
# #test1_LDADD = $../src/librlglue.la
# #INCLUDES = -I$(srcdir)/../src
# 
# #This is maybe supposed to be a program?
# #TESTS_ENVIRONMENT = $(top_srcdir)/tests
# #test1_LDADD = $(builddir)../src/librlglue.la
# #test1_LDADD = $(top_builddir)/lib/src/librlglue.la
# #test1_LDADD=$(top_builddir)/src/librlglue.la
# #test1_LDFLAGS   = $(AM_LDFLAGS) $(LIBADD_DL) -lrlglue -L$(top_builddir)/src
# LDADD=../src/librlglue.la
# 
# 
# TESTS = test1
# 
# EXTRA_DIST              = $(TESTS)

check_Programs=test1
TESTS = test1

INCLUDES = -I$(top_srcdir)/src
LDADD = ../src/librlglue.la

test1_SOURCES = constant_environment.c	echo_agent.c experiment.c useful_functions.c useful_functions.h
test1_CFLAGS = $(AM_CFLAGS) -Wall -pedantic -std=c99

noinst_PROGRAMS = $(TESTS)
