#!/bin/bash

CurrentDir=$(pwd)
echo
echo 'Edit "Run_network_demo.bash" to change which agent, environment or experiment program to execute'
echo

# Run the glue ------------------------------------
../../RL-Glue/bin/RL_glue & 

# ----------------------------------------------
# Choose an Agent
# (uncomment only ONE of the following agents)
# ----------------------------------------------
#
# ---------- Java -----------------
#cd ../Agent/Java/
#./RL_agent &
#cd $CurrentDir

# ----------  C -----------------
../Agent/C/RL_agent &

# ---------- Python -----------------
#cd ../Agent/Python
#./RL_agent &
#cd $CurrentDir



# ----------------------------------------------
# Choose an Environment
# (uncomment only ONE of the following environments)
# ----------------------------------------------
#
# ---------- Java -----------------
#cd ../Environment/Java/
#./RL_environment &
#cd $CurrentDir

# ----------  C -----------------
../Environment/C/RL_environment &

# ---------- Python -----------------
#cd ../Environment/Python/
#./RL_environment &
#cd $CurrentDir



# ----------------------------------------------
# Choose an Experiment
# (uncomment only ONE of the following Experiment)
# ----------------------------------------------
#
# ---------- Java -----------------
#cd ../Experiment/Java/
#./RL_experiment &
#cd $CurrentDir

# ----------  C -----------------
../Experiment/C/RL_experiment 

# ---------- Python -----------------
#cd ../Experiment/Python/
#./RL_experiment &
#cd $CurrentDir





