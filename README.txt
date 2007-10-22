=========================
RL-Glue Distribution
=========================
Copyright Adam White 2005

----------------------------
Documentation
----------------------------
For the full RL-Glue Documentation, please visit RL-Glue's public home:
http://rlai.cs.ualberta.ca/RLBB/top.html


----------------------------
Super short introduction to RL-Glue
----------------------------
Each run with RL-Glue will use four components: the glue, an environment program, an agent program, and an experiment program.  In the most general case, each of these components is a separate process and they all communicate through the glue using network sockets.

To make this work, you're going to need some sort of C compiler.  If you want to use the Java components, you should have at least Java 1.5.  The Python components should work with any reasonably recent Python distribution.

This code is tested on Linux and Mac OS X.  Your mileage may vary on Windows using Cygwin. 

----------------------------
Getting Started
----------------------------
There are several make targets: 
      - rl-glue  	<-- this will make the RL-Glue executable: ./RL-Glue/bin/RL_glue
      - examples 	<-- this will make C, Java, and Python examples in the ./Examples directory and subdirectories
      - all			<-- rl-glue and examples
	  - clean 		<-- deletes everything

So, to get started, just type:

$>make all

----------------------------
Running your First Experiment
----------------------------

----------------------------
	C/C++ 
----------------------------
The following instructions will start an experiment of a random agent on the "mines" environment. All of the components are networked and written in C. To have some components not networked (compiled in directly with the glue), check out the Examples/Glue_plus_* directories.

Open four terminal windows and run the following commands (after you have run 'make all' in the base rl-glue directory):

Terminal 1:
$>cd Examples/Glue_Only
./RL_glue

Terminal 2:
$>cd Examples/basic_examples/C/Agent
./RL_agent

Terminal 3:
$>cd Examples/basic_examples/C/Environment
./RL_environment

Terminal 4:
$>cd Examples/basic_examples/C/Experiment
./RL_experiment

----------------------------
	Java and Python
----------------------------
There are also examples for Java and Python. To run a Java agent:

$>cd Examples/basic_examples/Java/Agent
$>./RL_agent

And likewise for Environments and Experiments. To use Python, just replace Java with Python above.

These 'executables' are actually bash scripts which invoke either the Python or Java interpreter and loading mechanisms.

-----------------------------------------
	Source Code and Build Instructions
-----------------------------------------
The source for all of these example agents, environments, and experiments are located in:
./Examples/Agents
./Examples/Environments
./Examples/Experiments

To look at how each of these components is built, look at the makefiles in the ./Examples/basic_examples/{C/Java/Python}/{Environments/Agent/Experiment}/ directories!


-------------------------------------
Related Information and Documentation
-------------------------------------
You may also want to check out the developer website:
http://rl-glue.googlecode.com/

You may also be interested in the related (and possibly outdated) RL-Library site, which hosts a variety of agents and environments:
http://rlai.cs.ualberta.ca/RLR/index.html

Or the disjoint developer website for the rl-library:
http://rl-library.googlecode.com/


Credits
--------
Adam White
Mark Lee
Andrew Butcher
Brian Tanner
Leah Hackman
Richard Sutton





