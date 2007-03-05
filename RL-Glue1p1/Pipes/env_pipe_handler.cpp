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

} task_spec_struct;

void parse_task_spec(char* ts, task_spec_struct* ps);


//-------------------------
Task_specification env_init();
Observation env_start();
Reward_observation env_step(Action a);
void env_cleanup();
#ifdef ENV_STATE
State_key env_get_state();
void env_set_state(State_key sk);
#endif
#ifdef ENV_RANDOM
Random_seed_key env_get_random_seed();
void env_set_random_seed(Random_seed_key rsk);
#endif

//-------------------------
void pipe_env_init();
void pipe_env_start();
void pipe_env_step();
void pipe_env_get_state();
void pipe_env_set_state();
void pipe_env_get_random_seed();
void pipe_env_set_random_seed();

Reward_observation hand_ro;
Action agent_action;
Observation agent_state;
State_key sk;
Random_seed_key rsk;

FILE* pipe_in_stream;
FILE* pipe_out_stream;

char pipe_env_buf[100000];

task_spec_struct tss;

template<class T>
void my_alloc(T& val)
{

}

template<>
void my_alloc(int*& val)
{
	val = new int[tss.action_dim];
}

template<>
void my_alloc(double*& val)
{
	val = new double[tss.action_dim];
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
	for (int i = 0; i < tss.obs_dim; i++)
    {
    	fprintf(f," %lf",val[i]);
    }
    fprintf(f,"\n");
    fflush(f);
}

template<>
void pipe_write(FILE* f, int* val)
{
    for (int i = 0; i < tss.obs_dim; i++)
    {
        fprintf(f," %d",val[i]);
    }
    fprintf(f,"\n");
    fflush(f);
}

template <class T>
void pipe_read(FILE* f, T& val)
{
    fscanf(f," %s",val);
}

template <>
void pipe_read(FILE* f, int& val)
{
    fscanf(f," %d",&val);
}

template <>
void pipe_read(FILE* f, double& val)
{
    fscanf(f," %lf",&val);
}

template <>
void pipe_read (FILE* f, int*& val )
{
    for (int i = 0; i < tss.action_dim; i++)
    {
    	fscanf(f," %d",&(val[i]));
    }
}

template <>
void pipe_read (FILE* f, double*& val )
{
    for (int i = 0; i < tss.action_dim; i++)
    {
    	fscanf(f," %lf",&(val[i]));
    }
}

int main(int argc, char** argv)
{
	char* pipe_in;
	char* pipe_out;

	if (argc == 3)
	{
		printf("\nUsing user pipe names:\n");
		printf("\t...%s\n\t...%s\n\n",argv[1],argv[2]);
		pipe_in = argv[1];
		pipe_out = argv[2];
	}
	else if(argc == 1)
	{
		printf("\nUsing default pipe names:\n");
		printf("\t.../tmp/RL_pipe_env_in\n\t.../tmp/RL_pipe_env_out\n\n");
		pipe_in = "/tmp/RL_pipe_env_in";
		pipe_out = "/tmp/RL_pipe_env_out";
	}
	else
	{
		printf("\nError: incorrect input arguments!\n\nExample of valid invokations:\n \t./RLenv /tmp/RL_pipe_env_in /tmp/RL_pipe_env_out\n...or...\n\t./RLenv\n\nExiting...\n\n");
		exit(0);
	}
	pipe_in_stream = fopen(pipe_in,"r");
	pipe_out_stream = fopen(pipe_out,"w");
	while(pipe_in_stream == NULL || pipe_out_stream == NULL)
	{
		pipe_in_stream = fopen(pipe_in,"r");
		pipe_out_stream = fopen(pipe_out,"w");
	}


	pipe_read(pipe_in_stream,pipe_env_buf);
	

    while(true)
    {   
        if(strcmp(pipe_env_buf,"init") == 0)
        {   
            pipe_env_init();
        }
        if(strcmp(pipe_env_buf,"start") == 0)
        {   
            pipe_env_start();
        }        
        else if(strcmp(pipe_env_buf,"step") == 0)   
        {
            pipe_env_step();
        }
        else if(strcmp(pipe_env_buf,"getstate") == 0)   
        {
            pipe_env_get_state();
        }
        else if(strcmp(pipe_env_buf,"setstate") == 0)   
        {
            pipe_env_set_state();
        }
        else if(strcmp(pipe_env_buf,"getrandom") == 0)   
        {
            pipe_env_get_random_seed();
        }
        else if(strcmp(pipe_env_buf,"setrandom") == 0)   
        {
            pipe_env_set_random_seed();
        }
        else if(strcmp(pipe_env_buf,"cleanup") == 0)
        {
            env_cleanup();
			fclose(pipe_in_stream);
			fclose(pipe_out_stream);
            exit(1);
        }        
		pipe_read(pipe_in_stream,pipe_env_buf);
    }

    return 0;
}

  
void pipe_env_init()
{
    char* ts = env_init();
    parse_task_spec(ts,&tss);
	my_alloc(agent_action);
    pipe_write(pipe_out_stream, ts);
}

void pipe_env_start()
{
    pipe_write(pipe_out_stream, env_start());
}
        
void pipe_env_step()
{
//get action off pipe
	pipe_read(pipe_in_stream,agent_action);
    hand_ro = env_step(agent_action);

	pipe_write(pipe_out_stream, hand_ro.o);
	pipe_write(pipe_out_stream, hand_ro.r);
	pipe_write(pipe_out_stream, (int)hand_ro.terminal);
}

void pipe_env_get_state()
{
#ifdef ENV_STATE
	sk = env_get_state();
	pipe_write(pipe_out_stream, sk);
#else
	pipe_write(pipe_out_stream,"NOTIMPLEMENTED");
#endif
}
void pipe_env_set_state()
{
#ifdef ENV_STATE
	pipe_read(pipe_in_stream,sk);
	env_set_state(sk);
	pipe_write(pipe_out_stream,"IMPLEMENTED");
#else
	pipe_write(pipe_out_stream,"NOTIMPLEMENTED");
#endif
}

void pipe_env_get_random_seed()
{
#ifdef ENV_RANDOM
	rsk = env_get_random_seed();
	pipe_write(pipe_out_stream, rsk);
#else
	pipe_write(pipe_out_stream,"NOTIMPLEMENTED");
#endif
}
void pipe_env_set_random_seed()
{
#ifdef ENV_RANDOM
	pipe_read(pipe_in_stream,rsk);
	env_set_random_seed(rsk);
	pipe_write(pipe_out_stream,"IMPLEMENTED");
#else
	pipe_write(pipe_out_stream,"NOTIMPLEMENTED");
#endif
}

void parse_type(char*& ts, int& dim, char*& types, float*& mins, float*& maxs)
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

void parse_task_spec(char* ts, task_spec_struct* ps)
{
	int i,j;
	
	int n = sscanf(ts," %f : %c : %n",&(ps->version),&(ps->episodic),&j);
	if(n != 2)
	{
		printf("\nError1: Incorrect task spec format!!! Exiting....\n\n"); 
		exit(0);
	}
	ts += j;
	parse_type(ts,ps->obs_dim,ps->obs_types,ps->obs_mins,ps->obs_maxs);
	parse_type(ts,ps->action_dim,ps->action_types,ps->action_mins,ps->action_maxs);
	
}

