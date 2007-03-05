#include <sys/types.h> 
#include <sys/wait.h> 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

#include "RLcommon.h"

typedef struct 
{
        float version;			
        char episodic;			
        int obs_dim;			
        char *obs_types;	    
        float *obs_mins;           
        float *obs_maxs;			
        int action_dim;
        char *action_types;		
        float *action_mins;		
        float *action_maxs;	

} task_spec_struct2;

void parse_task_spec2(char* ts, task_spec_struct2* ps);

void agent_init(Task_specification ts);
Action agent_start(Observation s);
Action agent_step(Reward r, Observation o);
void agent_end(Reward r);
void agent_cleanup();

void pipe_agent_init();
void pipe_agent_start();
void pipe_agent_step();
void pipe_agent_end();

Action agent_action;
Observation agent_state;

FILE* pipe_in_stream;
FILE* pipe_out_stream;
ifstream pipe_in_file;
ofstream pipe_out_file;

char pipe_agent_buf[100000];

task_spec_struct2 tss;

template<class T>
void my_alloc(T& val)
{

}

template<>
void my_alloc(int*& val)
{
	val = new int[tss.obs_dim];
}

template<>
void my_alloc(double*& val)
{
	val = new double[tss.obs_dim];
}


template<class T>
void pipe_write(FILE* f, T val)
{
    fprintf(f," %s\n",val);
    fflush(f);
}

template<>
void pipe_write(FILE* f, int val)
{
    fprintf(f," %d\n",val);
    fflush(f);
}

template<>
void pipe_write(FILE* f, double val)
{
    fprintf(f," %lf\n",val);
    fflush(f);
}

template<>
void pipe_write(FILE* f, double* val)
{
    for (int i = 0; i < tss.action_dim; i++)
    {
	fprintf(f," %lf",val[i]);
    }
    fprintf(f,"\n");
    fflush(f);
}

template<>
void pipe_write(FILE* f, int* val)
{
    for (int i = 0; i < tss.action_dim; i++)
    {
    	fprintf(f," %d",val[i]);
    }
    fprintf(f,"\n");
    fflush(f);
}

template <class T>
void pipe_read(FILE* f, T& val)
{
	//is >> val;
	//return is;
	fscanf(f," %s",val);
}

//--------------
template <>
void pipe_read(FILE* f, int& val )
{
fscanf(f," %d",&val);
}
//-------------

template <>
void pipe_read(FILE* f, double& val )
{
	fscanf(f," %lf",&val);
}

template <>
void pipe_read (FILE* f, int*& val )
{
	//val = new int[tss.obs_dim];
    for (int i = 0; i < tss.obs_dim; i++)
    {
        //is >> val[i];
	fscanf(f," %d",&(val[i]));
    }
	//return is;
}

template <>
void pipe_read (FILE* f, double*& val )
{
	//val = new double[tss.obs_dim];
    for (int i = 0; i < tss.obs_dim; i++)
    {
        //is >> val[i];
	fscanf(f," %lf",&(val[i]));
	//fread(&(val[i]),sizeof(double),1,f);
    }
	//return is;
}


int main(int argc, char** argv)
{
	char* pipe_in;
	char* pipe_out;

	if(argc == 3)
	{
		printf("\nUsing user pipe names:\n");
		printf("\t...%s\n\t...%s\n\n",argv[1],argv[2]);
		pipe_in = argv[1];
		pipe_out = argv[2];
	}
	else if(argc == 1)
	{
		printf("\nUsing default pipe names:\n");
		printf("\t.../tmp/RL_pipe_agent_in\n\t.../tmp/RL_pipe_agent_out\n\n");
		pipe_in = "/tmp/RL_pipe_agent_in";
		pipe_out= "/tmp/RL_pipe_agent_out";
	}
	else
	{
		printf("\nError: incorrect input arguments!\n\nExample of valid invokations:\n\t./RLagent /tmp/RL_pipe_agent_in /tmp/RL_pipe_agent_out\n...or...\n\t./RLagent\n\nExiting...\n\n");
		exit(0);
	}
	
	pipe_in_stream = fopen(pipe_in,"r");
	pipe_out_stream = fopen(pipe_out,"w");
	while(pipe_in_stream == NULL || pipe_out_stream == NULL)
	{
		pipe_in_stream = fopen(pipe_in,"r");
		pipe_out_stream = fopen(pipe_out,"w");
	}

	pipe_agent_buf[0] = '\0';
	pipe_read(pipe_in_stream,pipe_agent_buf);
    while(true)
    {    
        if(strcmp(pipe_agent_buf,"init") == 0)
        {   
            pipe_agent_init();
        }
        else if(strcmp(pipe_agent_buf,"start") == 0)   
        {
            pipe_agent_start();
        }
        else if(strcmp(pipe_agent_buf,"step") == 0)   
        {
            pipe_agent_step();
        }
        else if(strcmp(pipe_agent_buf,"end") == 0){   
            pipe_agent_end();
        }
        else if(strcmp(pipe_agent_buf,"cleanup") == 0)
        {
            agent_cleanup();
			fclose(pipe_in_stream);
			fclose(pipe_out_stream);
            exit(1);
        }        
		pipe_agent_buf[0] = '\0';
    	pipe_read(pipe_in_stream,pipe_agent_buf);
		while (strlen(pipe_agent_buf) <= 0)
			pipe_read(pipe_in_stream,pipe_agent_buf);
	}

    return 0;
}

  
void pipe_agent_init()
{
	pipe_read(pipe_in_stream,pipe_agent_buf);
	parse_task_spec2(pipe_agent_buf,&tss);
	my_alloc(agent_state);
    agent_init(pipe_agent_buf);    
}

void pipe_agent_start()
{
	pipe_read(pipe_in_stream,agent_state);
    agent_action = agent_start(agent_state);
	pipe_write(pipe_out_stream,agent_action); 
}

void pipe_agent_step()
{
    Reward reward;
    pipe_read(pipe_in_stream,agent_state);
    pipe_read(pipe_in_stream,reward);
    agent_action = agent_step(reward,agent_state);
    pipe_write(pipe_out_stream,agent_action);
}

void pipe_agent_end()
{
    double reward;
    pipe_read(pipe_in_stream,reward);
	agent_end(reward);  
}

void parse_type2(char*& ts, int& dim, char*& types, float*& mins, float*& maxs)
{
	int j,n,i;
	n = sscanf(ts," %d _ [%n",&dim,&j);
	if(n != 1)
	{
		printf("\nError6: Incorrect task spec format!!! Exiting....\n\n"); 
		exit(0);
	}
	ts += j;
	types  =new char[dim+1];
	mins = new float[dim];
	maxs = new float[dim];
	for (i = 0; i < dim-1; i++)
	{
		n = sscanf(ts," %c ,%n",&(types[i]),&j);
		if (n != 1)
		{
			printf("\nError7: Incorrect task spec format!!! Exiting....\n\n"); 
			exit(0);
		}
		ts += j;
	} 
	n = sscanf(ts," %c ] _ [%n",&(types[i++]),&j);
	if (n != 1)
	{
		printf("\nError8: Incorrect task spec format!!! Exiting....\n\n"); 
		exit(0);
	}
	ts += j;
	types[i] = '\0';
	
	for (i = 0; i < dim-1; i++)
	{
		n = sscanf(ts," %f , %f ] _ [%n",&(mins[i]),&(maxs[i]),&j);
		if (n != 2)
		{
			printf("\nError9: Incorrect task spec format!!! Exiting....\n\n"); 

			exit(0);
		}
		ts += j;
	}
	n = sscanf(ts," %f , %f ] :%n",&(mins[i]),&(maxs[i]),&j);
	if (n != 2)
	{
		printf("\nError: Incorrect task spec format!!! Exiting....\n\n"); 
		exit(0);
	}
	ts += j;
}

void parse_task_spec2(char* ts, task_spec_struct2* ps)
{
	int i,j;
	
	int n = sscanf(ts," %f : %c : %n",&(ps->version),&(ps->episodic),&j);
	if(n != 2)
	{
		printf("\nError1: Incorrect task spec format!!! Exiting....\n\n"); 
		exit(0);
	}
	ts += j;
	parse_type2(ts,ps->obs_dim,ps->obs_types,ps->obs_mins,ps->obs_maxs);
	parse_type2(ts,ps->action_dim,ps->action_types,ps->action_mins,ps->action_maxs);

}
