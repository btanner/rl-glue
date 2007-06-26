package rlglue;

public interface AgentInterface 
{
    void agent_init(final String taskSpecification);
    Action agent_start(Observation observation);
    Action agent_step(double reward, Observation observation);
    void agent_end(double reward);
    void agent_cleanup();
    void agent_freeze();
    String agent_message(final String message);
}
