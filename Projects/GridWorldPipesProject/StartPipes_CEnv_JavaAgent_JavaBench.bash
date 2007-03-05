#! /bin/bash

rm /tmp/RL*
mkfifo "/tmp/RL_pipe_agent_in" "/tmp/RL_pipe_agent_out" "/tmp/RL_pipe_env_out" "/tmp/RL_pipe_env_in" "/tmp/RL_pipe_main_in" "/tmp/RL_pipe_main_out"

java  -classpath ".:Build/" agent_pipe_handler SarsaAgent &
./Build/RLenv &
java  -classpath ".:Build/" main &

./RL_main 

