javac Agent/AgentLoader.java
javac Environment/EnvironmentLoader.java 
javac rlglue/*.java
javac ../../Utils/*.java
rm -Rf classes
mkdir classes
mkdir classes/rlglue
mv Agent/*.class classes
mv Environment/*.class classes
mv rlglue/*.class classes/rlglue
mv ../../Utils/*.class classes
cd classes
jar -cvf RL-Glue.jar ./rlglue/*.class *.class
mv RL-Glue.jar ../
cd ../
