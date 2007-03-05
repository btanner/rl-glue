#include <sys/types.h> 
#include <sys/stat.h>
#include <sys/wait.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include <errno.h>
#include <libgen.h>
#include <fcntl.h>
#include <ctype.h>

#include "Pipecommon.h"

     
#ifndef PIPE_AGENT_H
#define PIPE_AGENT_H

int agent_file_in;
int agent_file_out;

void write_data(char* _pipe);
void read_data(char* pipe_);

char pipe_data[10000];

#endif
