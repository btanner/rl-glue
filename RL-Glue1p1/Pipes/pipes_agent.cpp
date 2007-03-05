#include "pipes_agent.h"
#include <stdlib.h>
void env_cleanup();
void agent_cleanup();

void agent_init(char* ts, char* PIPE_AGENT_IN, char* PIPE_AGENT_OUT)
{	

	
	agent_file_in = open(PIPE_AGENT_IN,O_WRONLY);
	agent_file_out = open(PIPE_AGENT_OUT,O_RDONLY);
	while(agent_file_in == -1 || agent_file_out == -1)
	{
		agent_file_in = open(PIPE_AGENT_IN,O_WRONLY);
		agent_file_out = open(PIPE_AGENT_OUT,O_RDONLY);
	}

	write(agent_file_in, "init\n", strlen("init\n"));
	write(agent_file_in, ts, strlen(ts));
	write(agent_file_in, "\n", strlen("\n"));
    
}

void agent_init(char* ts)
{		
	agent_file_in = open("/tmp/RL_pipe_agent_in",O_WRONLY);
	agent_file_out = open("/tmp/RL_pipe_agent_out",O_RDONLY);
	while(agent_file_in == -1 || agent_file_out == -1)
	{
		agent_file_in = open("/tmp/RL_pipe_agent_in",O_WRONLY);
		agent_file_out = open("/tmp/RL_pipe_agent_out",O_RDONLY);
	}
	
	write(agent_file_in, "init\n", strlen("init\n"));
	write(agent_file_in, ts, strlen(ts));
	write(agent_file_in, "\n", strlen("\n"));
    
}

Action agent_start(Observation o)
{ 
    write(agent_file_in, "start\n", strlen("start\n"));
    write_data(o);

    read_data(pipe_data);
    return pipe_data;
}

Action agent_step(Reward r, Observation o)
{ 
    write(agent_file_in, "step\n", strlen("step\n"));

    write_data(o);
    sprintf(pipe_data,"%lf",r);

    write_data(pipe_data);
    read_data(pipe_data);
    return pipe_data;
}

void agent_end(Reward r)
{
    write(agent_file_in, "end\n", strlen("end\n"));
    sprintf(pipe_data,"%lf",r);
    write_data(pipe_data);
} 
void agent_cleanup()
{
    write(agent_file_in, "cleanup\n", strlen("cleanup\n"));
	close(agent_file_in);
	close(agent_file_out);
}    

void write_data(char* buf)
{    
    write(agent_file_in, buf, strlen(buf));
    write(agent_file_in, "\n", strlen("\n"));
}

void read_data(char* buf)          
{
    int i = 0;
    buf[0] = '\0';


    read(agent_file_out, &(buf[i]), 1);
    while(buf[i] != '\n')
    {
        read(agent_file_out, &(buf[++i]), 1);
    }

}
