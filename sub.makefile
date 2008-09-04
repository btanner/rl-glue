.PHONY: all clean

all:
	make -f rl-glue.makefile CC=$(CC)
	make -f rl-glue.makefile CC=$(CXX)
	
clean:
	make -f rl-glue.makefile clean
	
