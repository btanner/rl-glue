RL-Glue Distribution
---------------------
Copyright Adam White 2005

Documentation
---------------
For the full RL-Glue Documentation, please visit RL-Glue's public home:
http://rlai.cs.ualberta.ca/RLBB/top.html

Getting Started
---------------
There are several make targets: 
      - rl-glue  <-- this will make the RL-Glue executable: ./RL-Glue/bin/RL_glue
      - examples <-- this will make C, Java, and Python examples in the ./Examples directory and subdirectories
      - all	 <-- rl-glue and examples

So, to get started, just type:
make all

Running your First Experiment
----------------------------
Open four terminal windows and run the following commands (after you have run 'make all' in the base rl-glue directory):

Terminal 1:
cd Examples/Glue_only
./RL_glue

Terminal 2:
cd Examples/C/Agent
./RL_agent

Terminal 3:
cd Examples/C/Environment
./RL_environment

Terminal 4:
cd Examples/C/Experiment
./RL_experiment

This will start an experiment of a random agent on a mines environment. All of the components are networked and written in C. To have some components not networked (compiled in directly with the glue), check out the Examples/Glue_plus_* directories.

There are also examples for Java and Python. To run a Java agent:

cd Examples/Java/Agent
./RL_agent

And likewise for Environments and Experiments. To use Python, just replace Java with Python above.


Related Information and Documentation
-------------------------------------
You may also want to check out the developer website:
http://rl-glue.googlecode.com/

You may also be interested in the related (and possibly outdated) RL-Library site, which hosts a variety of agents and environments:
http://rlai.cs.ualberta.ca/RLR/index.html

Or the disjoint developer website:
http://rl-library.googlecode.com/


Credits
--------
Adam White
Mark Lee
Andrew Butcher
Brian Tanner
Leah Hackman
Richard Sutton





