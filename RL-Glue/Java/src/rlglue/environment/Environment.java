package rlglue.environment;

import rlglue.types.Action;
import rlglue.types.Observation;
import rlglue.types.Random_seed_key;
import rlglue.types.Reward_observation;
import rlglue.types.State_key;

public interface Environment
{
    String env_init();
    Observation env_start();
    Reward_observation env_step(Action action);
    void env_cleanup();
    void env_set_state(State_key key);
    void env_set_random_seed(Random_seed_key key);
    State_key env_get_state();
    Random_seed_key env_get_random_seed();
    String env_message(final String message);
}
