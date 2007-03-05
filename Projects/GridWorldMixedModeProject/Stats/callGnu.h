#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
//------------------------------------------------------------------
//This routine takes a string, which contains plotting
//commands for gnuplot. It starts gnuplot plots according
//to the input string.
//
//returns -1 if it could not open gnuplot, 0 otherwise
//Should not have to change this code
//------------------------------------------------------------------
int gnuplot(const char *gnucommand)
{
	pid_t pid;
	int RLpipe2[2];
	int status;	
	
	if (pipe(RLpipe2) < 0)
	{
		perror("pipe");
		exit(1);
	}
    pid = fork();	
    if (pid == 0) 
	{
		dup2(RLpipe2[0], 0);
		close(RLpipe2[1]);
		
		execlp("gnuplot","gnuplot","-persist",NULL);
		exit(1);  
	}
	close(RLpipe2[0]);
	FILE* fpg_in = fdopen(RLpipe2[1],"w");
	fprintf(fpg_in,"%s\n",gnucommand);
	fflush(fpg_in);
    fclose(fpg_in);
	
    waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) != 0)
			return -1;

	return 0;
}

