GLUE_RELEASE=3:0:0
GLUE_VERSION=3:0:0

BUILT_SOURCES= svn_dist_version svn_version.c 
EXTRA_DIST= svn_dist_version

lib_LTLIBRARIES = librlglue.la librlgluenetdev.la
 
librlglue_la_CFLAGS = -Wall -pedantic -std=c99
librlglue_la_SOURCES = RL_direct_experiment.c RL_direct_environment.c RL_direct_agent.c RL_glue.c rlglue/RL_common.h rlglue/Agent_common.h rlglue/Environment_common.h rlglue/Experiment_common.h svn_version.c
librlglue_la_LDFLAGS= -version-info $(GLUE_VERSION) -release $(GLUE_RELEASE)
librlglue_la_includedir=$(includedir)/rlglue
librlglue_la_include_HEADERS = rlglue/RL_common.h rlglue/Agent_common.h rlglue/Environment_common.h rlglue/legacy_types.h  rlglue/Experiment_common.h 

NETDEV_RELEASE=1:0:0
NETDEV_VERSION=1:0:0

librlgluenetdev_la_CFLAGS = -Wall -pedantic -std=c99

librlgluenetdev_la_SOURCES = RL_network.c rlglue/RL_common.h rlglue/network/RL_network.h svn_version.c

librlgluenetdev_la_LDFLAGS= -version-info $(NETDEV_RELEASE) -release $(NETDEV_VERSION)
librlgluenetdev_la_includedir=$(includedir)/rlglue/network
librlgluenetdev_la_include_HEADERS = rlglue/network/RL_network.h

bin_PROGRAMS = rl_glue
rl_glue_SOURCES = RL_network_experiment.c RL_server_environment.c RL_network_agent.c RL_network_glue.c RL_server_experiment.c RL_glue.c RL_network_environment.c RL_server_agent.c rlglue/Agent_common.h svn_version.c rlglue/Environment_common.h rlglue/RL_common.h rlglue/Experiment_common.h
#This is basically a no-op suggested by: http://www.gnu.org/software/libtool/manual/automake/Libtool-Issues.html to solve a problem where because both targets build rlglue.c
rl_glue_CFLAGS = $(AM_CFLAGS) -Wall -pedantic -std=c99
rl_glue_LDADD   = librlgluenetdev.la

if HAVESVNWC
svn_dist_version: FORCE
	MYSVNVERSION=`@mysvnversion@`
	@mysvnversion@ > svn_dist_version
else
	MYSVNVERSION < svn_dist_version
endif

svn_version.c: svn_dist_version
	echo 'const char* svn_version(void) { const char* SVN_Version = "\c' > svn_version.c
	$(MYSVNVERSION)                  >> svn_version.c
	echo '"; return SVN_Version; }\c'   >> svn_version.c




FORCE:
