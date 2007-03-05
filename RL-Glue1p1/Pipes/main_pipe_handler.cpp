/*
 *  main_pipe_handler.cpp
 *  
 *
 *  Created by Adam White on 29/11/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */
#include <sys/types.h> 
#include <sys/wait.h> 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>

#include <sys/stat.h>

#include <errno.h>
#include <libgen.h>
#include <fcntl.h>
#include <ctype.h>

using namespace std;

#include "RL_Interface.h"

Observation_action oa0;
Reward_observation_action_terminal roat0;

State_key sk;
Random_seed_key rsk;

FILE* pipe_in_stream;
FILE* pipe_out_stream;

char pipe_main_buf[100000];
char pipe_main_buf2[100000]; char* tmp_main_buf;

int steps; double stepsd;
double reward;

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

task_spec_struct2 tss;

void parse_task_spec2(char* ts, task_spec_struct2* ps);

void pipe_main_init0();
void pipe_main_init1();
void  pipe_main_start();
void pipe_main_step();
void pipe_main_return();
void pipe_main_average();
void pipe_main_a_num_steps();
void pipe_main_num_steps();
void pipe_main_num_episodes();
void pipe_main_episode0();
void pipe_main_episode1();
void pipe_main_set_state();
void pipe_main_set_random();
void pipe_main_get_state();
void pipe_main_get_random_seed();


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

template<class T>
void pipe_write_act(FILE* f, T val)
{
    fprintf(f," %s\n",val);
    fflush(f);
}

template<>
void pipe_write_act(FILE* f, int val)
{
    fprintf(f," %d\n",val);
    fflush(f);
}
template<>
void pipe_write_act(FILE* f, double val)
{
    fprintf(f," %lf\n",val);
    fflush(f);
}

template<>
void pipe_write_act(FILE* f, double* val)
{
    for (int i = 0; i < tss.action_dim; i++)
    {
	fprintf(f," %lf",val[i]);
    }
    fprintf(f,"\n");
    fflush(f);
}

template<>
void pipe_write_act(FILE* f, int* val)
{
    for (int i = 0; i < tss.action_dim; i++)
    {
    	fprintf(f," %d",val[i]);
    }
    fprintf(f,"\n");
    fflush(f);
}

template<class T>
void pipe_write_obs(FILE* f, T val)
{
    fprintf(f," %s\n",val);
    fflush(f);
}

template<>
void pipe_write_obs(FILE* f, int val)
{
    fprintf(f," %d\n",val);
    fflush(f);
}
template<>
void pipe_write_obs(FILE* f, double val)
{
    fprintf(f," %lf\n",val);
    fflush(f);
}
template<>
void pipe_write_obs(FILE* f, double* val)
{
    for (int i = 0; i < tss.obs_dim; i++)
    {
	fprintf(f," %lf",val[i]);
    }
    fprintf(f,"\n");
    fflush(f);
}

template<>
void pipe_write_obs(FILE* f, int* val)
{
    for (int i = 0; i < tss.obs_dim; i++)
    {
    	fprintf(f," %d",val[i]);
    }
    fprintf(f,"\n");
    fflush(f);
}

template<class T>
void pipe_write_key(FILE* f, T val)
{
    fprintf(f," %s\n",val);
    fflush(f);
}

template<>
void pipe_write_key(FILE* f, int val)
{
    fprintf(f," %d\n",val);
    fflush(f);
}
template<>
void pipe_write_key(FILE* f, double val)
{
    fprintf(f," %lf\n",val);
    fflush(f);
}

template<>
void pipe_write_key(FILE* f, vector <double> val)
{
    for (int i = 0; i < val.size(); i++)
    {
	fprintf(f," %lf",val[i]);
    }
    fprintf(f,"\n");
    fflush(f);
}

template<>
void pipe_write_key(FILE* f, vector <int> val)
{
    for (int i = 0; i < val.size(); i++)
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

template <class T>
void pipe_read_key(FILE* f, T& val)
{
	//is >> val;
	//return is;
	fscanf(f," %s",val);
}

//--------------
template <>
void pipe_read_key(FILE* f, int& val )
{
fscanf(f," %d",&val);
}
//-------------

template <>
void pipe_read_key(FILE* f, double& val )
{
	fscanf(f," %lf",&val);
}
void pipe_read_key (FILE* f, vector<int>& val )
{
    for (int i = 0; i < val.size(); i++)
    {
	fscanf(f," %d",&(val[i]));
    }
}

template <>
void pipe_read_key (FILE* f, vector<double>& val )
{
    for (int i = 0; i < val.size(); i++)
    {
	fscanf(f," %lf",&(val[i]));
    }
}

int main(int argc, char** argv)
{
	char pipe_in[100];
	char pipe_out[100];

	strcpy(pipe_in,"/tmp/RL_pipe_main_in");
	strcpy(pipe_out,"/tmp/RL_pipe_main_out");

	pipe_in_stream = fopen(pipe_out,"r+");
	pipe_out_stream = fopen(pipe_in,"w+");
	while(pipe_in_stream == NULL || pipe_out_stream == NULL)
	{
		pipe_in_stream = fopen(pipe_out,"r+");
		pipe_out_stream = fopen(pipe_in,"w+");
	}

	pipe_main_buf[0] = '\0';
	pipe_read(pipe_in_stream,pipe_main_buf);
    while(true)
    {    
        if(strcmp(pipe_main_buf,"RLinit0") == 0)
        {   
            pipe_main_init0();
        }
        else if(strcmp(pipe_main_buf,"RLinit1") == 0)
        {   
            pipe_main_init1();
        }
        else if(strcmp(pipe_main_buf,"RLstart") == 0)   
        {
            pipe_main_start();
        }
        else if(strcmp(pipe_main_buf,"RLstep") == 0)   
        {
            pipe_main_step();
        }
        else if(strcmp(pipe_main_buf,"RLreturn") == 0){   
            pipe_main_return();
        }
        else if(strcmp(pipe_main_buf,"RLaveragereward") == 0){   
            pipe_main_average();
        }
        else if(strcmp(pipe_main_buf,"RLaveragenumsteps") == 0){   
            pipe_main_a_num_steps();
        }
        else if(strcmp(pipe_main_buf,"RLnumsteps") == 0){   
            pipe_main_num_steps();
        }
        else if(strcmp(pipe_main_buf,"RLnumepisodes") == 0){   
            pipe_main_num_episodes();
        }
        else if(strcmp(pipe_main_buf,"RLepisode0") == 0){   
            pipe_main_episode0();
        }
        else if(strcmp(pipe_main_buf,"RLepisode1") == 0){   
            pipe_main_episode1();
        }
        else if(strcmp(pipe_main_buf,"RLsetstate") == 0){   
            pipe_main_set_state();
        }
        else if(strcmp(pipe_main_buf,"RLsetrandom") == 0){   
            pipe_main_set_random();
        }
        else if(strcmp(pipe_main_buf,"RLgetstate") == 0){   
            pipe_main_get_state();
        }
        else if(strcmp(pipe_main_buf,"RLgetrandomseed") == 0){   
            pipe_main_get_random_seed();
        }
        else if(strcmp(pipe_main_buf,"RLcleanup") == 0)
        {
            RL_cleanup();
			fclose(pipe_in_stream);
			fclose(pipe_out_stream);
            exit(1);
        }        
		pipe_main_buf[0] = '\0';
    	pipe_read(pipe_in_stream,pipe_main_buf);
		while (strlen(pipe_main_buf) <= 0)
			pipe_read(pipe_in_stream,pipe_main_buf);
	}

    return 0;
}

void pipe_main_init0()
{
	RL_init();
    parse_task_spec2(RL_get_spec(),&tss);
	pipe_write(pipe_out_stream,"done");
}
void pipe_main_init1()
{
	pipe_read(pipe_in_stream,pipe_main_buf);
	pipe_read(pipe_in_stream,pipe_main_buf2);
#ifdef PIPES
	RL_init(pipe_main_buf,pipe_main_buf2);
#else
	RL_init();
#endif
	pipe_write(pipe_out_stream,"done");
}

void pipe_main_start()
{
	printf("in start\n\n");
	oa0 = RL_start();
	pipe_write_obs(pipe_out_stream,oa0.o);
	pipe_write_act(pipe_out_stream,oa0.a);
}
void pipe_main_step()
{
	roat0 = RL_step();
	pipe_write(pipe_out_stream,roat0.r);
	pipe_write_obs(pipe_out_stream,roat0.o);
	pipe_write_act(pipe_out_stream,roat0.a);
	pipe_write(pipe_out_stream,roat0.terminal);
}

void pipe_main_return()
{
	reward = RL_return();
	pipe_write(pipe_out_stream,reward);
}
void pipe_main_average()
{
	reward = RL_average_reward();
	pipe_write(pipe_out_stream,reward);
}	
void pipe_main_a_num_steps()
{
	stepsd = RL_average_num_steps();
	pipe_write(pipe_out_stream,stepsd);
}	
void pipe_main_num_steps()
{
	steps = RL_num_steps();
	pipe_write(pipe_out_stream,steps);
}
void pipe_main_num_episodes()
{
	steps = RL_num_episodes();
	pipe_write(pipe_out_stream,steps);
}	
void pipe_main_episode0()
{
	RL_episode();
	pipe_write(pipe_out_stream,"done");
}
void pipe_main_episode1()
{
	pipe_read(pipe_in_stream,steps);
	RL_episode(steps);
	pipe_write(pipe_out_stream,"done");
}	
void pipe_main_set_state()
{
	pipe_read_key(pipe_in_stream,sk);
	RL_set_state(sk);
	pipe_write(pipe_out_stream,"done");
}
void pipe_main_set_random()
{
	pipe_read_key(pipe_in_stream,rsk);
	RL_set_random_seed(rsk);
	pipe_write(pipe_out_stream,"done");
}
void pipe_main_get_random_seed()
{
	rsk = RL_get_random_seed();
	pipe_write_key(pipe_out_stream,rsk);
}
void pipe_main_get_state()
{
	sk = RL_get_state();
	pipe_write_key(pipe_out_stream,sk);
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
