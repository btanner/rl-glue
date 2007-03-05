#! /bin/bash

var0=" "
var1=" "

FILE=config.txt
declare -a VAR 
declare -a AGENT 
declare -a ENV 
declare -a COMM 
declare -a MAIN 
declare -a n
declare -a piles
declare -a osname
declare -a oss

let k=0
piller=`gcc --version | head -n 1`

if [ "${piller}" = "" ]; then
    piller=`cc -v 2>&1 | head -n 1`
    if [ "${piller}" = "" ]; then
        echo
        echo ERROR1: GCC NOT FOUND. RL-Glue requires at least GNU GCC 3.3.0
        echo Exiting ....
        echo
        exit 0
        
fi;fi;

i=0
  for ELEMENT in $piller ; do
    piles[$k]=$ELEMENT

    let i=$k-1
    if [ $k == 0 ]; then 
        let i=0
    fi
    
    if [ ${piles[$i]} = "not" ]; then
    if [ ${piles[$k]} = "found" ]; then
        echo
        echo ERROR2: GCC NOT FOUND. RL-Glue requires at least GNU GCC 3.3.0
        echo Exiting ....
        echo
        exit 0
    fi;fi;
    
    let k=$k+1
    
  done

let jj=0
jj=$k
echo
k=0
yy=0
while [  $k -lt $jj ] ; do
    if [ ${piles[$k]} = "(GCC)" ] ; then
        let yy=1
        let k=$k+1
        echo You are using GCC version ${piles[$k]}
        if [ `expr "${piles[$k]}" : '5\.*'` = 0 ]; then
        if [ `expr "${piles[$k]}" : '4\.*'` = 0 ]; then
        if [ `expr "${piles[$k]}" : '3\.9\.*'` = 0 ]; then
        if [ `expr "${piles[$k]}" : '3\.8\.*'` = 0 ]; then
        if [ `expr "${piles[$k]}" : '3\.7\.*'` = 0 ]; then
        if [ `expr "${piles[$k]}" : '3\.6\.*'` = 0 ]; then
        if [ `expr "${piles[$k]}" : '3\.5\.*'` = 0 ]; then
        if [ `expr "${piles[$k]}" : '3\.4\.*'` = 0 ]; then
        if [ `expr "${piles[$k]}" : '3\.3\.*'` = 0 ]; then
            echoc
            echo ERROR3: RL-Glue requires at least GNU GCC 3.3.0
            echo Exiting ....
            echo
            exit 0
        fi
        fi;fi;fi;fi;fi;fi;fi;fi;
    else
        let k=$k+1
    fi
done

if [ $yy = 0 ]; then
            echo
            echo WARNING: Your complier is not regonized 
            echo RL-Glue requires at least GNU GCC 3.3.0
            echo RL-Glue may not compile or run correctly
            echo Continuing ....
fi
        
osname=`uname -a | head -n 1`

k=0
  for ELEMENT in $osname ; do
    oss[$k]=$ELEMENT
    let k=$k+1
  done

k=0
echo
echo You are running ${oss[$k]}
if [ ${oss[$k]} == "SunOS" ]; then
            echo
            echo WARNING: You are using SUN OS 
            echo RL-Glue has not been extensively tested on SUN OS
            echo RL-Glue may not compile or run correctly
            echo Continuing ....
else
if [ ${oss[$k]} != "Darwin" ]; then
if [ ${oss[$k]} != "Linux" ]; then
if [ ${oss[$k]} != "Unix" ]; then
if [ ${oss[$k]} != "AIX" ]; then
            echo
            echo ERROR5: RL-Glue only supports Mac OS X, Linux, Unix and AIX
            echo Exiting ....
            echo
            exit 0   
fi;fi;fi;fi;fi;



LASTLINE=''
# Putting elements into an array of Variables
let x=0 ;   let y=0
while read LINE ; do  
  LASTLINE=$LINE
  for ELEMENT in $LINE ; do
    VAR[$x]=$ELEMENT
    let x=$x+1
  done
  # Do something with the Variables
  y=0
  n=`echo ${VAR[$y]} | tr A-Z a-z`
  
  if [ "$n" = "agent" ]; then
  y=2
  AGENT=`echo ${VAR[$y]}`
  fi
  y=0
  if [ "$n" = "env" ]; then
  y=2
  ENV=`echo ${VAR[$y]}`
  fi  
  y=0
  if [ "$n" = "main" ]; then
  y=2
  MAIN=`echo ${VAR[$y]}`
  fi
  y=0
  if [ "$n" = "common" ]; then
  y=2
  COMM=`echo ${VAR[$y]}`
  fi   
  unset VAR
  let x=0 
done < $FILE

x=0
A=`tail -n 1 $FILE`
if [ ! "$LASTLINE" = "$A" ]; then
    for ELEMENT in $A ; do
    VAR[$x]=$ELEMENT
    let x=$x+1
  done
  # Do something with the Variables
  y=0
  n=`echo ${VAR[$y]} | tr A-Z a-z`
  
  if [ "$n" = "agent" ]; then
  y=2
  AGENT=`echo ${VAR[$y]}`
  fi
  y=0
  if [ "$n" = "env" ]; then
  y=2
  ENV=`echo ${VAR[$y]}`
  fi  
  y=0
  if [ "$n" = "main" ]; then
  y=2
  MAIN=`echo ${VAR[$y]}`
  fi
  y=0
  if [ "$n" = "common" ]; then
  y=2
  COMM=`echo ${VAR[$y]}`
  fi  
  y=0
  if [ "$n" = "agent_language" ]; then
  y=2
  AGENTLANG=`echo ${VAR[$y]}`
  fi  
  y=0
  if [ "$n" = "env_language" ]; then
  y=2
  ENVLANG=`echo ${VAR[$y]}`
  fi
fi

echo
echo $AGENT
echo $ENV
echo

if [ -n "`echo "$AGENT" | grep -x '.*\.[^/]*'`" ]; then	  
	if [ -z "`echo "$ENV" | grep -x '.*\.[^/]*'`" ]; then
		printf "\nERROR1: RL-Glue 1.0 only supports 2-way pipe communication. Both agent and environment must be execuatable or source files.\n\n"
		exit 0
	fi
fi

if [ -z "`echo "$AGENT" | grep -x '.*\.[^/]*'`" ]; then
	if [ -n "`echo "$ENV" | grep -x '.*\.[^/]*'`" ]; then
		printf "\nERROR2: RL-Glue 1.0 only supports 2-way pipe communication. Both agent and environment must be execuatable or source files.\n\n"
		exit 0
	fi
fi	

if [ -n "`echo "$AGENT" | grep -x '.*\.[^/]*'`" ]; then
if [ -z "`echo "$AGENT" | grep -x '.*c'`" ]; then
	if [ -z "`echo "$AGENT" | grep -x '.*cc'`" ]; then
		if [ -z "`echo "$AGENT" | grep -x '.*cpp'`" ]; then
		printf "\nERROR3: RL-Glue 1.0 only supports direct language calls for c and c++. \n\n" 
		exit 0
fi; fi; fi; 
fi

if [ -n "`echo "$ENV" | grep -x '.*\.[^/]*'`" ]; then
if [ -z "`echo "$ENV" | grep -x '.*c'`" ]; then
	if [ -z "`echo "$ENV" | grep -x '.*cc'`" ]; then
		if [ -z "`echo "$ENV" | grep -x '.*cpp'`" ]; then
		printf "\nERROR33: RL-Glue 1.0 only supports direct language calls for c and c++. \n\n" 
		exit 0
fi; fi; fi; 
fi

if [ -z "`echo "$ENV" | grep -x '.*\.[^/]*'`" ]; then
	if [ -z "`echo "$AGENT" | grep -x '.*\.[^/]*'`" ]; then
		var0=PIPES
	fi
fi

if [ -z "`echo "$MAIN" | grep -x '.*\.[^/]*'`" ]; then
		var1=PIPES
fi
if [ -n "`echo "$MAIN" | grep -x '.*java'`" ]; then
		var1=PIPES
fi

makestr0=".PHONY: all clean clean-irix\n
MAINV = $MAIN 
AGENTV = $AGENT 
ENVV = $ENV 
COMMON = $COMM \n
all:  ../../RL-Glue/RLcommon.h Build/RL_Interface.o Build/agent.o Build/env.o Build/main.o Build/gnu.o Build/spec.o  
\t\${CXX}  Build/env.o Build/gnu.o Build/RL_Interface.o Build/agent.o Build/main.o Build/spec.o  -o RL_main \n
../../RL-Glue/RLcommon.h: \${COMMON}
\tcp \${COMMON} ../../RL-Glue/RLcommon.h\n 
Build/RL_Interface.o: ../../RL-Glue/RL_Interface.c 
\t\${CXX} -g -o Build/RL_Interface.o -c ../../RL-Glue/RL_Interface.c\n
Build/main.o: \$(MAINV)
\t\${CXX} -I ../../RL-Glue/ -I Stats/ -g -o Build/main.o -c \$(MAINV) \n
Build/agent.o: \$(AGENTV) 
\t\${CXX} -I ../../RL-Glue/ -I Utils/ -g -o Build/agent.o -c \$(AGENTV) \n
Build/env.o: \$(ENVV) 
\t\${CXX} -I ../../RL-Glue/ -g -o Build/env.o -c \$(ENVV) \n
Build/gnu.o: Stats/gnu.cpp 
\t\${CXX} -g -c -o Build/gnu.o Stats/gnu.cpp \n
Build/spec.o: Utils/C_agent_parser.cpp
\t\${CXX} -g -c -o Build/spec.o Utils/C_agent_parser.cpp\n

clean: 
\t-rm -rf Build/*.o RL_main core* ../../RL-Glue/RLcommon.h 
clean-irix:
\trm -rf Build/*.o RL_main core* ../../RL-Glue/RLcommon.h"

makestr1=".PHONY: all clean clean-irix\n
MAINV = ../../RL-Glue/Pipes/main_pipe_handler.cpp
AGENTV = $AGENT 
ENVV = $ENV 
COMMON = $COMM \n
all:  ../../RL-Glue/RLcommon.h Build/RL_Interface.o Build/agent.o Build/env.o Build/main.o Build/spec.o  
\t\${CXX}  Build/env.o Build/RL_Interface.o Build/agent.o Build/main.o Build/spec.o -o RL_main \n
../../RL-Glue/RLcommon.h: \${COMMON}
\tcp \${COMMON} ../../RL-Glue/RLcommon.h\n 
Build/RL_Interface.o: ../../RL-Glue/RL_Interface.c 
\t\${CXX} -g -o Build/RL_Interface.o -c ../../RL-Glue/RL_Interface.c\n
Build/main.o: \$(MAINV)
\t\${CXX} -I ../../RL-Glue/  -g -o Build/main.o -c \$(MAINV) \n
Build/agent.o: \$(AGENTV) 
\t\${CXX} -I ../../RL-Glue/ -I Utils/ -g -o Build/agent.o -c \$(AGENTV) \n
Build/env.o: \$(ENVV) 
\t\${CXX} -I ../../RL-Glue/ -g -o Build/env.o -c \$(ENVV) \n
Build/spec.o: Utils/C_agent_parser.cpp
\t\${CXX} -g -c -o Build/spec.o Utils/C_agent_parser.cpp\n

clean: 
\t-rm -rf Build/*.o RL_main core* ../../RL-Glue/RLcommon.h 
clean-irix:
\trm -rf Build/*.o RL_main core* ../../RL-Glue/RLcommon.h"

makestr2=".PHONY: all clean clean-irix\n
MAINV = $MAIN 
AGENTV = ../../RL-Glue/Pipes/pipes_agent.cpp 
ENVV = ../../RL-Glue/Pipes/pipes_env.cpp 
COMMON = ../../RL-Glue/Pipes/Pipecommon.h \n
all: ../../RL-Glue/RLcommon.h Build/RL_Interface.o Build/agent.o Build/env.o Build/main.o Build/gnu.o 
\t\${CXX}  Build/env.o Build/gnu.o Build/RL_Interface.o Build/agent.o Build/main.o -o RL_main\n
../../RL-Glue/RLcommon.h: \${COMMON}
\tcp \${COMMON} ../../RL-Glue/RLcommon.h\n
Build/RL_Interface.o: ../../RL-Glue/RL_Interface.c
\t\${CXX} -g -o Build/RL_Interface.o -c ../../RL-Glue/RL_Interface.c\n
Build/main.o: \$(MAINV)
\t\${CXX} -I ../../RL-Glue/ -I stats/ -g -o Build/main.o -c \$(MAINV)\n
Build/agent.o: \$(AGENTV)
\t\${CXX} -I ../../RL-Glue/ -I -g -o Build/agent.o -c \$(AGENTV)\n
Build/env.o: \$(ENVV)
\t\${CXX} -I ../../RL-Glue/ -g -o Build/env.o -c \$(ENVV)\n
Build/gnu.o: Stats/gnu.cpp
\t\${CXX} -g -c -o Build/gnu.o Stats/gnu.cpp\n
clean:
\t-rm -rf Build/*.o RL_main  core*  ../../RL-Glue/RLcommon.h
clean-irix:
\trm -rf Build/*.o RL_main  core*  ../../RL-Glue/RLcommon.h"

makestr3=".PHONY: all clean clean-irix\n
MAINV = ../../RL-Glue/Pipes/main_pipe_handler.cpp 
AGENTV = ../../RL-Glue/Pipes/pipes_agent.cpp 
ENVV = ../../RL-Glue/Pipes/pipes_env.cpp 
COMMON = ../../RL-Glue/Pipes/Pipecommon.h \n
all: ../../RL-Glue/RLcommon.h Build/RL_Interface.o Build/agent.o Build/env.o Build/main.o  
\t\${CXX}  Build/env.o Build/RL_Interface.o Build/agent.o Build/main.o -o RL_main\n
../../RL-Glue/RLcommon.h: \${COMMON}
\tcp \${COMMON} ../../RL-Glue/RLcommon.h\n
Build/RL_Interface.o: ../../RL-Glue/RL_Interface.c
\t\${CXX} -g -o Build/RL_Interface.o -c ../../RL-Glue/RL_Interface.c\n
Build/main.o: \$(MAINV)
\t\${CXX} -I ../../RL-Glue/ -g -o Build/main.o -c \$(MAINV)\n
Build/agent.o: \$(AGENTV)
\t\${CXX} -I ../../RL-Glue/ -I -g -o Build/agent.o -c \$(AGENTV)\n
Build/env.o: \$(ENVV)
\t\${CXX} -I ../../RL-Glue/ -g -o Build/env.o -c \$(ENVV)\n
clean:
\t-rm -rf Build/*.o RL_main  core*  ../../RL-Glue/RLcommon.h
clean-irix:
\trm -rf Build/*.o RL_main  core*  ../../RL-Glue/RLcommon.h"


rm makefile
if [ "$var0" = "PIPES" ]; then
    if [ "$var1" = "PIPES" ]; then
        printf "\nAgent-Env and Main Interface will use Pipe communication \n\n"
        printf "$makestr3" > makefile
    else
        printf "\nAgent-Env Interface will use Pipe communication \n\n"
        printf "$makestr2" > makefile   
    fi;     
else
    if [ "$var1" = "PIPES" ]; then
        printf "\nMain Interface will use Pipe communication \n\n"
        printf "$makestr1" > makefile    
    else
	   printf "\nInterface will NOT use Pipe communication \n\n"
	   printf "$makestr0" > makefile
    fi
fi

make clean
make

