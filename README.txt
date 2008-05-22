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
Getting Started - Cygwin/Windows
----------------------------
You will need to have Java installed in Windows (not within Cygwin).
Within Cygwin you will need gcc, make, and bash.

To get the Java compiler to work within Cygwin, you will need to include
it in your path. Edit your ~/.bash_profile:

$ vim ~/.bash_profile  

You will need to install the vim package (or some other editor).

And then add the following line:

export PATH=/cygdrive/c/Program\ Files/Java/jdk1.6.0_03/bin:$PATH

This obviously depends on where Java is installed on your computer and the version of Java you have installed. 

----------------------------
Running your First Experiment
----------------------------

The following instructions will start an experiment of a random agent on the "mines" environment. All of the components are networked and written in C. Open the "Run_networked_example.bash" script in Examples/Run_scripts. There are three sections in this script: agent, environment and experiment program. Make sure that only one each agent, environment and experiment is not commented out. So if we wanted all three components to be C programs, the script would look like the following:
----------- Verbatim --------------------------------
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
cd ../Environment/Python/
./RL_environment &
cd $CurrentDir



# ----------------------------------------------
# Choose an Experiment
# (uncomment only ONE of the following Experiment)
# ----------------------------------------------
#
# ---------- Java -----------------
cd ../Experiment/Java/
./RL_experiment &
cd $CurrentDir

# ----------  C -----------------
../Experiment/C/RL_experiment &

# ---------- Python -----------------
#cd ../Experiment/Python/
#./RL_experiment &
#cd $CurrentDir


# Run the glue ------------------------------------
../../RL-Glue/bin/RL_glue 

----------- End Verbatim --------------------------------

This will run a C agent, C environment and C experiment program is three separate processes, communicating over sockets. Feel free to mix and match the different languages. Remember you can only run a one component of the same type at a time; you can't start a Java agent and a Python agent.

To run using no networked components (compiled in directly with the glue), run the "Glue_single_C_exe.bash" script.


-----------------------------------------
	Source Code and Build Instructions
-----------------------------------------
The source for all of these example agents, environments, and experiments are located in:
./Examples/Agents/*/src
./Examples/Environments/*/src
./Examples/Experiments/*/src

To look at how each of these components is built, look at the makefiles in each directory


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





