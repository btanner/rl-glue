LDFLAGS := $(LDFLAGS)
CFLAGS := $(CFLAGS) -Wall -pedantic

all: SkeletonExperiment

SkeletonExperiment: SkeletonExperiment.o SkeletonAgent.o SkeletonEnvironment.o
	$(CC)  $(LDFLAGS) SkeletonExperiment.o SkeletonAgent.o SkeletonEnvironment.o -lrlglue -lrlutils -o SkeletonExperiment      

SkeletonExperiment.o: SkeletonExperiment.c
	$(CC)  $(CFLAGS) -c SkeletonExperiment.c -o SkeletonExperiment.o 

SkeletonAgent.o: SkeletonAgent.c
	$(CC)  $(CFLAGS) -c SkeletonAgent.c -o SkeletonAgent.o 

SkeletonEnvironment.o: SkeletonEnvironment.c
	$(CC)  $(CFLAGS) -c SkeletonEnvironment.c -o SkeletonEnvironment.o 

clean:
	rm -rf SkeletonExperiment SkeletonExperiment.exe *.o









