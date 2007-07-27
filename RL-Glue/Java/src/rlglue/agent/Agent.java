package rlglue.agent;

import rlglue.types.Action;
import rlglue.types.Observation;

public interface Agent
{
    public void agent_init(final String taskSpecification);
    public Action agent_start(Observation observation);
    public Action agent_step(double reward, Observation observation);
    public void agent_end(double reward);
    public void agent_cleanup();
    public void agent_freeze();
    public String agent_message(final String message);
}
