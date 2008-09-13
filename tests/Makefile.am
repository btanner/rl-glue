TESTS = test_sanity test_1 test_message test_rl_episode

INCLUDES = -I$(top_srcdir)/src
LDADD = ../src/librlglue.la

test_sanity_SOURCES = test_1_environment.c	test_1_agent.c test_sanity_experiment.c useful_functions.c useful_functions.h
test_sanity_CFLAGS = $(AM_CFLAGS) -Wall -pedantic -std=c99

test_1_SOURCES = test_1_environment.c	test_1_agent.c test_1_experiment.c useful_functions.c useful_functions.h
test_1_CFLAGS = $(AM_CFLAGS) -Wall -pedantic -std=c99

test_message_SOURCES = test_message_agent.c	test_message_environment.c test_message_experiment.c useful_functions.c useful_functions.h
test_message_CFLAGS = $(AM_CFLAGS) -Wall -pedantic -std=c99

test_rl_episode_SOURCES = test_1_environment.c	test_1_agent.c test_rl_episode_experiment.c useful_functions.c useful_functions.h
test_rl_episode_experiment_CFLAGS = $(AM_CFLAGS) -Wall -pedantic -std=c99

noinst_PROGRAMS = $(TESTS)
