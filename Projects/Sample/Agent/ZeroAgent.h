#ifndef ZeroAgent_h
#define ZeroAgent_h

#include <RLcommon.h>

void agent_init(int argc, char** argv, Task_specification task_spec);
Action agent_start(Observation o);
Action agent_step(Reward r, Observation o);
void agent_end(Reward r);
void agent_cleanup();

#endif
