check_PROGRAMS=test1 message_test rl_episode_test_experiment
TESTS = $(check_Programs)

INCLUDES = -I$(top_srcdir)/src
LDADD = ../src/librlglue.la

test1_SOURCES = test1_environment.c	test1_agent.c test1_experiment.c useful_functions.c useful_functions.h
test1_CFLAGS = $(AM_CFLAGS) -Wall -pedantic -std=c99

message_test_SOURCES = message_test_agent.c	message_test_environment.c message_test_experiment.c useful_functions.c useful_functions.h
message_test_CFLAGS = $(AM_CFLAGS) -Wall -pedantic -std=c99

rl_episode_test_experiment_SOURCES = test1_environment.c	test1_agent.c rl_episode_test_experiment.c useful_functions.c useful_functions.h
rl_episode_test_experiment_CFLAGS = $(AM_CFLAGS) -Wall -pedantic -std=c99

noinst_PROGRAMS = $(TESTS)
