LDFLAGS := $(LDFLAGS)
CFLAGS := $(CFLAGS) -Wall -pedantic -g

all: SampleExperiment

SampleExperiment: SampleExperiment.o SampleSarsaAgent.o SampleMinesEnvironment.o
	$(CC)  $(LDFLAGS) SampleExperiment.o SampleSarsaAgent.o SampleMinesEnvironment.o -lm -lrlglue -lrlutils -o SampleExperiment      

SampleExperiment.o: SampleExperiment.c
	$(CC)  $(CFLAGS) -c SampleExperiment.c -o SampleExperiment.o 

SampleSarsaAgent.o: SampleSarsaAgent.c
	$(CC)  $(CFLAGS) -c SampleSarsaAgent.c -o SampleSarsaAgent.o 

SampleMinesEnvironment.o: SampleMinesEnvironment.c
	$(CC)  $(CFLAGS) -c SampleMinesEnvironment.c -o SampleMinesEnvironment.o 

clean:
	rm -rf SampleExperiment SampleExperiment.exe *.o









