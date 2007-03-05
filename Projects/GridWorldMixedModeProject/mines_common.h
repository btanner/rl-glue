
#ifndef MINE_COMMON_H
#define MINE_COMMON_H

#define ENV_INIT
#define AGENT_INIT
#define AGENT_CLEANUP
#define ENV_CLEANUP


typedef int Observation;    //Defining what type a Observation is
typedef int Action;	  //Defining what type a action is

typedef char* Task_specification;
typedef double Reward;

typedef struct {
	Reward r;
	Observation o;
	bool terminal;
} Reward_observation;

typedef int State_key;
typedef int Random_seed_key;


#endif