import java.util.Random;

import rlglue.*;

public class Mines implements Environment
{
    protected Random random;
    public DefaultEnvironment()
    {
    }

    public String env_init()
    {
	random = new Random();
	return "123456789ABCDEF";
    }

    public Observation env_start()
    {
	Observation obs = new Observation(1,0);
	obs.intArray[0] = random.nextInt();
	return obs;
    }

    public Reward_observation env_step(Action action)
    {
	Reward_observation ro = new Reward_observation();

	ro.o = new Observation(1,0);
	ro.r = -1;

	if (action.intArray[0] == 1)
	    ro.terminal = 1;
	else 
	    ro.terminal = 0;

	return ro;
    }

    public void env_cleanup()
    {
    }

    public String env_message(final String message)
    {
	return "";
    }

    public Random_seed_key env_get_random_seed()
    {
	return new Random_seed_key(1,0);
    }

    public State_key env_get_state()
    {
	return new State_key(1,0);
    }

    public void env_set_random_seed(Random_seed_key rsk)
    {
    }

    public void env_set_state(State_key rsk)
    {
    }
}