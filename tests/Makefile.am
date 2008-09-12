check_Programs=test1 message_test
TESTS = $(check_Programs)

INCLUDES = -I$(top_srcdir)/src
LDADD = ../src/librlglue.la

test1_SOURCES = constant_environment.c	echo_agent.c experiment.c useful_functions.c useful_functions.h
test1_CFLAGS = $(AM_CFLAGS) -Wall -pedantic -std=c99

message_test_SOURCES = message_test_agent.c	message_test_environment.c message_test_experiment.c useful_functions.c useful_functions.h
message_test_CFLAGS = $(AM_CFLAGS) -Wall -pedantic -std=c99

noinst_PROGRAMS = $(TESTS)
