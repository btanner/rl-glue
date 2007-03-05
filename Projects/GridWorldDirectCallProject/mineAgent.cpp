#include "mineAgent.h"

void agent_init(Task_specification task_spec)
{

    if(task_spec != NULL)
    {
        //Parse task_spec...General case where I dont know in adavance number of state and actions dimensions
        //if you know use one call to sscanf
		int j = 0;	
		try 
		{
			task_spec_struct ps;
			parse_task_spec(task_spec, &ps);
			version = ps.version;
			state_dim = (int)ps.obs_maxs[0]+1;
			action_dim = (int)ps.action_maxs[0]+1;
			
		} //done parsing ...not so bad!!
        catch(int e) 
        {
            fprintf(stderr,"Error: \n");
            fprintf(stderr,"I did not understand your task spec. I need 'numStates numActions'\n");
            exit(-1);
		}
        
		n = state_dim; //WE KNOW ONLY OE STATE DIMENSION
		m = action_dim;    //again we know number of action dim       
		fprintf(stderr,"task_spec: %s, num_states: %d, num_actions: %d\n",task_spec,n,m);
		nm = n*m;   

		actions = new int[m];//array of possible actions
		for(int i = 0; i < m; i++)
			actions[i] = i;

		Q = new double*[n];
		for( int i = 0; i < n; i++)
			Q[i] = new double[m];

		for( int i = 0; i < n; i++)
			for(int j =0; j < m; j++)
				Q[i][j] = 0;                

		alpha = 0.1;
		epsilon = 0.01;
		srand(999);
    }

}

Action agent_start(Observation o)
{
	//Choose and return the agent's first action
	A = egreedy(o);        
	return actions[A];
}

Action agent_step(Reward r, Observation o)
{
	//Update one step, return agent's action
	double delta;
	int iNew, iOld, a;
        
	a = egreedy(o);

	delta = r + Q[o][a] - Q[S][A];
	Q[S][A] = Q[S][A] + alpha*delta;
	A = a;
	S = o;

	return actions[a];    
}

void agent_end(Reward r)
{
	//Update last step of current episode
	int i;
	double error;
	error = r - Q[S][A];
	Q[S][A] = Q[S][A] + alpha*error;
}

void agent_cleanup()
{
	for( int i = 0; i < n; i++)
		delete [] Q[i];
		
	delete [] Q;
	delete [] actions;
}

int egreedy(int s)
{
	//chooses action epsilon-greedily
	double ran = rand()/static_cast<double>(RAND_MAX);
	if( ran > epsilon ) 
		return argmax(s);
	else
		return (int)(rand() % m);
}

int argmax(int s)
{
	//find the index of the maximal action, break ties randomly
	int bestIndex;
	double bestValue, value;
	int numBests = 1;   
      
	bestIndex = 0;
	bestValue = Q[s][0];	

	for(int i = 1; i < m; i++)
	{
            value = Q[s][i];
            if( value > bestValue )
            {
                bestValue = value;
                bestIndex = i;
                numBests = 1;
            } 
            else if (value == bestValue) 
            {
                numBests = numBests + 1;
                if( (int)(rand() % numBests) == 0 )
                    bestIndex = i;
            }
	}
	return bestIndex;        
}
