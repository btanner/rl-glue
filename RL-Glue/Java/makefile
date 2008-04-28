all:
	mkdir -p bin
	javac -d bin src/rlglue/*.java src/rlglue/**/*.java
	cd bin && jar cmf ../MANIFEST RL-Glue.jar * && mv RL-Glue.jar ../
