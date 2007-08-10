all:
	cd src && javac rlglue/RLGlue.java -d ../bin
	cd src/rlglue/ && javac -cp ../../bin agent/ClientAgent.java -d ../../bin 
	cd src/rlglue/ && javac -cp ../../bin environment/ClientEnvironment.java -d ../../bin/	
	cd bin && jar cf RL-Glue.jar * && mv RL-Glue.jar ../
