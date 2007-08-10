all:
	mkdir -p bin
	cd src && javac rlglue/*.java -d ../bin
	cd src/rlglue/ && javac -cp ../../bin agent/*.java -d ../../bin 
	cd src/rlglue/ && javac -cp ../../bin environment/*.java -d ../../bin/	
	cd bin && jar cf RL-Glue.jar * && mv RL-Glue.jar ../
