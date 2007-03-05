#ifndef MINE_COMMON_H
#define MINE_COMMON_H

#define ENV_INIT
#define AGENT_INIT
#define AGENT_CLEANUP

typedef int Observation;    //Defining what type a Observation is
typedef int Action;	  //Defining what type a action is




// ------- Don't change below this line ------------
typedef char* Task_specification;
typedef double Reward;
typedef int Random_seed_key;
typedef int State_key;

typedef struct {
	Reward r;
	Observation o;
	bool terminal;
} Reward_observation;

#endif
