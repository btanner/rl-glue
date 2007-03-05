#include "pipes_env.h"

void env_cleanup();
void agent_cleanup();

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