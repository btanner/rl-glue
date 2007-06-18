#ifndef RLcommon_h
#define RLcommon_h

typedef char* Task_specification;
typedef double Reward;

typedef struct RL_abstract_type_t
{
  unsigned int numInts;
  unsigned int numDoubles;
  int* intArray;
  double* doubleArray;
} RL_abstract_type;

typedef RL_abstract_type Observation;
typedef RL_abstract_type Action;
typedef RL_abstract_type Random_seed_key;
typedef RL_abstract_type State_key;
typedef char* Message;

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
