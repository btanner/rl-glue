#include "pipes_env.h"



void RL_init()
{
		printf("\n\ninside interface rl\n\n");
		
		main_file_in = open("/tmp/RL_pipe_main_in",O_WRONLY);
		main_file_out = open("/tmp/RL_pipe_main_out",O_RDONLY);
		while(env_file_in == -1 || env_file_out == -1)
		{
			env_file_in = open("/tmp/RL_pipe_main_in",O_WRONLY);
			env_file_out = open("/tmp/RL_pipe_main_out",O_RDONLY);
		}
        if (write(env_file_in, "RLinit0\n", strlen("RLinit0\n")) == -1)
		perror("pipe writing:");		
		
		Observation_action oa;
		Reward_observation_action_terminal roat;		
}

void RL_init(char* inpipe, char* outpipe)
{
		main_file_in = open("/tmp/RL_pipe_main_in",O_WRONLY);
		main_file_out = open("/tmp/RL_pipe_main_out",O_RDONLY);
		while(env_file_in == -1 || env_file_out == -1)
		{
			env_file_in = open("/tmp/RL_pipe_main_in",O_WRONLY);
			env_file_out = open("/tmp/RL_pipe_main_out",O_RDONLY);
		}
        if (write(env_file_in, "RLinit1\n", strlen("RLinit1\n")) == -1)
		perror("pipe writing:");
		write_data(inpipe);
		write_data(outpipe);
		
		Observation_action oa;
		Reward_observation_action_terminal roat;
}

Observation_action RL_start()
{
    write(env_file_in, "RLstart\n", strlen("RLstart\n"));
	read_data(pipe_main_buf);
	
	return oa;
}

Reward_observation_action_terminal RL_step()
{
    write(env_file_in, "RLstep\n", strlen("RLstep\n"));
	read_data(pipe_main_buf);
	
	return roat;
}

Reward RL_return()
{
    write(env_file_in, "RLreturn\n", strlen("RLreturn\n"));
	read_data(pipe_main_buf);
	
	return r;
}

Reward RL_average_reward()
{
    write(env_file_in, "RLaveragereward\n", strlen("RLaveragereward\n"));
	read_data(pipe_main_buf);
	
	return r;
}

double RL_average_num_steps()
{
    write(env_file_in, "RLaveragenumsteps\n", strlen("RLaveragenumsteps\n"));

	read_data(pipe_main_buf);
	sscanf(pipe_env_buf,"%lf",&(r));
	
	return r;
}

int RL_num_steps()
{
	int ret; 
	
    write(env_file_in, "RLnumsteps\n", strlen("RLnumsteps\n"));
	read_data(pipe_main_buf);
	sscanf(pipe_env_buf,"%d",&(ret));
	
	return ret;
}

int RL_num_episodes()
{
	int ret; 
	
    write(env_file_in, "RLnumepisodes\n", strlen("RLnumepisodes\n"));
	read_data(pipe_main_buf);
	sscanf(pipe_env_buf,"%d",&(ret));
	
	return ret;
}

void RL_episode()
{
    write(env_file_in, "RLepisode0\n", strlen("RLepisode0\n"));
	read_data(pipe_main_buf);
	if(strcmp(pipe_main_buf,"done") != 0)
		RL_cleanup();
}

void RL_episode(int num_Steps)
{
    write(env_file_in, "RLepisode1\n", strlen("RLepisode1\n"));
	pipe_main_buf[0] = '\0';
	sprintf(pipe_main_buf, "%d", &num_Steps);
	write_data(pipe_main_buf);
	
	if(strcmp(pipe_main_buf,"done") != 0)
		RL_cleanup();
}

void RL_set_state(int sk)
{
    write(env_file_in, "RLsetstate\n", strlen("RLsetstate\n"));
	pipe_main_buf[0] = '\0';
	sprintf(pipe_main_buf, "%d", &sk);
	write_data(pipe_main_buf);
	
	read_data(pipe_main_buf);

	if(strcmp(pipe_main_buf,"done") != 0)
		RL_cleanup();
}

void RL_set_state(float sk)
{
    write(env_file_in, "RLsetstate\n", strlen("RLsetstate\n"));
	pipe_main_buf[0] = '\0';
	sprintf(pipe_main_buf, "%lf", &sk);
	write_data(pipe_main_buf);
	
	read_data(pipe_main_buf);

	if(strcmp(pipe_main_buf,"done") != 0)
		RL_cleanup();
}

void RL_set_state(std::vector <int>& sk)
{
    write(env_file_in, "RLsetstate\n", strlen("RLsetstate\n"));
	pipe_main_buf[0] = '\0';
	for(int i=0; i< sk.size(); i++)
	{
		sprintf(tmpbuf,"%d",&(sk[i]));
		strcat(pipe_main_buf,tmpbuf);
	}
	write_data(pipe_main_buf);
	
	read_data(pipe_main_buf);

	if(strcmp(pipe_main_buf,"done") != 0)
		RL_cleanup();
}

void RL_set_state(std::vector <float>& sk);
void RL_set_random_seed(int rsk);
void RL_set_random_seed(float rsk);
void RL_set_random_seed(std::vector <int>& rsk);
void RL_set_random_seed(std::vector <float>& rsk);


Task_specification env_init(char* PIPE_ENV_IN, char* PIPE_ENV_OUT)
{	
		env_file_in = open(PIPE_ENV_IN,O_WRONLY);
		env_file_out = open(PIPE_ENV_OUT,O_RDONLY);
		while(env_file_in == -1 || env_file_out == -1)
		{
			env_file_in = open(PIPE_ENV_IN,O_WRONLY);
			env_file_out = open(PIPE_ENV_OUT,O_RDONLY);
		}
        if (write(env_file_in, "init\n", strlen("init\n")) == -1)
		perror("pipe writing:");
//		fsync(env_file_in);
		
//		fprintf(env_file_in,"init\n");

//		read_data_env{pipe_env_buf);
		ro.o = new char[100000];

		
        return pipe_env_buf; 
}

Task_specification env_init()
{


//does this code assume the files were already created????

		env_file_in = open("/tmp/RL_pipe_env_in",O_WRONLY);
		env_file_out = open("/tmp/RL_pipe_env_out",O_RDONLY);
		while(env_file_in == -1 || env_file_out == -1)
		{
			env_file_in = open("/tmp/RL_pipe_env_in",O_WRONLY);
			env_file_out = open("/tmp/RL_pipe_env_out",O_RDONLY);
		}
		
        if (write(env_file_in, "init\n", strlen("init\n")) == -1)
		perror("pipe writing:");
		//fsync(env_file_in);
		//printf("before\n");
		//fprintf(env_file_in,"%s","init");
		//fflush(env_file_in);
		read_data_env(pipe_env_buf);
		ro.o = new char[100000];

        return pipe_env_buf; 
}

Observation env_start()
{ 
    write(env_file_in, "start\n", strlen("start\n"));
	//fprintf(env_file_in,"start\n");

    read_data_env(pipe_env_buf);

    return pipe_env_buf;
}

Reward_observation env_step(Action act)
{   
    write(env_file_in, "step\n", strlen("step\n"));
	//fprintf(env_file_in,"step\n");

    write_data_env(act);    
    read_data_env(ro.o);
	read_data_env(pipe_env_buf);
	sscanf(pipe_env_buf,"%lf",&(ro.r));
	read_data_env(pipe_env_buf);
    sscanf(pipe_env_buf,"%d",&(ro.terminal));

    return ro;
}


State_key env_get_state()
{
    write(env_file_in, "getstate\n", strlen("getstate\n"));
	//fprintf(env_file_in,"getstate\n");

	read_data_env(pipe_env_buf);
	if (strcmp(pipe_env_buf,"NOTIMPLEMENTED\n") == 0)
	{
	
		printf("\n\nERROR: Get and Set state methods are either not defined or flags are not set in the common types file to allow you to call them!! \nExiting....\n\n");

		env_cleanup();
		agent_cleanup();
		exit(0);
	}
	return pipe_env_buf;
	
}
Random_seed_key env_get_random_seed()
{
    write(env_file_in, "getrandom\n", strlen("getrandom\n"));
	//fprintf(env_file_in,"getrandom\n");

	read_data_env(pipe_env_buf);
	if (strcmp(pipe_env_buf,"NOTIMPLEMENTED\n") == 0)
	{
		printf("\n\nERROR: Get and Set state methods are either not defined or flags are not set in the common types file to allow you to call them!! \nExiting....\n\n");

		env_cleanup();
		agent_cleanup();
		exit(0);
	}
	return pipe_env_buf;
}
void env_set_state(State_key sk0)
{
    write(env_file_in, "setstate\n", strlen("setstate\n"));
	//fprintf(env_file_in,"setstate\n");

	write_data_env(sk0);
	read_data_env(pipe_env_buf);
	if (strcmp(pipe_env_buf,"NOTIMPLEMENTED\n") == 0)
	{

		printf("\n\nERROR: Get and Set random methods are either not defined or flags are not set in the common types file to allow you to call them!! \nExiting....\n\n");

		env_cleanup();
		agent_cleanup();
		exit(0);
	}
}
void env_set_random_seed(Random_seed_key rsk0)
{
    write(env_file_in, "setrandom\n", strlen("setrandom\n"));
	//fprintf(env_file_in,"setrandom\n");

	write_data_env(rsk0);
	read_data_env(pipe_env_buf);
	if (strcmp(pipe_env_buf,"NOTIMPLEMENTED\n") == 0)
	{
	
		printf("\n\nERROR: Get and Set random methods are either not defined or flags are not set in the common types file to allow you to call them!! \nExiting....\n\n");

		env_cleanup();
		agent_cleanup();
		exit(0);
	}
}

void env_cleanup()
{
    write(env_file_in, "cleanup\n", strlen("cleanup\n"));
	//fprintf(env_file_in,"cleanup\n");

	close(env_file_in);
	close(env_file_out);
}    

void write_data_env(char* buf)
{
	sprintf(pipe_env_buf,"%s\n",(const char*)buf);
	///fprintf(env_file_in,pipe_env_buf);

	write(env_file_in, pipe_env_buf, strlen(pipe_env_buf));
}
  
void read_data_env(char* buf)          
{
    int i = 0;

    buf[0] = '\0';

    //fgets(&(buf[i]), 1, env_file_out);
	read(env_file_out, &(buf[i]), 1);

    while(buf[i] != '\n')
    {
	//	    fgets(&(buf[++i]), 1, env_file_out);

        read(env_file_out, &(buf[++i]), 1);
    }
	buf[++i] = '\0';
}