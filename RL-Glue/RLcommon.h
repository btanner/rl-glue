#ifndef RLcommon_h
#define RLcommon_h

typedef int Random_seed_key;
typedef int State_key;
typedef char* Task_specification;
typedef double Reward;

typedef struct RL_abstract_type_t
{
  int numInts;
  int numDoubles;
  int* intArray;
  double* doubleArray;
} RL_abstract_type;

typedef RL_abstract_type Observation;
typedef RL_abstract_type Action;

typedef struct {
  Observation o;
  Action a;
} Observation_action;

typedef struct Reward_observation_t
{
  Reward r;
  Observation o;
  int terminal;
} Reward_observation;

typedef struct {
  Reward r;
  Observation o;
  Action a;
  int terminal;
} Reward_observation_action_terminal;

#endif
