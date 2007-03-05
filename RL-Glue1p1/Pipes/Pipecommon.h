
#ifndef RLCOMMON_H
#define RLCOMMON_H

// specify which functions are defined
#define ENV_INIT
#define ENV_CLEANUP
#define AGENT_INIT
#define AGENT_CLEANUP
#define AGENT_NAME
#define ENV_NAME


#define	ENV_STATE
#define ENV_RANDOM


#define PIPES

typedef char*   Observation;          // observation is array of pos targets 
typedef char*   Action;	             // action are different agent actions

typedef char*  Task_specification;   // define task specification
typedef double Reward;               // define reward

typedef struct {                     // define structure with 
  Reward      r;                     //  - reward
  Observation o;                     //  - observation
  bool        terminal;              //  - terminal state
} Reward_observation;

typedef char* State_key;
typedef char* Random_seed_key;

#endif
