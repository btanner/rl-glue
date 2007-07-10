import rlglue.*;
import java.io.IOException;

public class Experiment
{
    protected static final int kNumEpisodes = 100;
    protected static int rlNumSteps[];
    protected static double rlReturn[];

    protected static void runStepwise() throws IOException
    {
	double avgSteps = 0.0;
	double avgReturn = 0.0;
	
	rlNumSteps = new int[Experiment.kNumEpisodes];
	rlReturn = new double[Experiment.kNumEpisodes];
	for (int x = 0; x < Experiment.kNumEpisodes; ++x)
	{
	    RLGlue.RL_start();
	    while(RLGlue.RL_step().terminal != 1);
	}

	/*add up all the steps and all the returns*/
	for (int i = 0; i < Experiment.kNumEpisodes; ++i) {
	    avgSteps += rlNumSteps[i];
	    avgReturn += rlReturn[i];
	}
	
	/*average steps and returns*/
	avgSteps /= (double)Experiment.kNumEpisodes;
	avgReturn /= (double)Experiment.kNumEpisodes;
	
	/*print out results*/
	System.out.println("\n-----------------------------------------------\n");
	System.out.println("Number of episodes: " + Experiment.kNumEpisodes);
	System.out.println("Average number of steps per episode: " +  avgSteps);
	System.out.println("Average return per episode: " + avgReturn);
	System.out.println("-----------------------------------------------\n");
    }

    protected static void runEpisodic() throws IOException
    {
	double avgSteps = 0.0;
	double avgReturn = 0.0;
	
	rlNumSteps = new int[Experiment.kNumEpisodes];
	rlReturn = new double[Experiment.kNumEpisodes];

	/*basic main loop*/
	/*run for num_episode number of episodes and store the number of steps and return from each episode*/        
	for(int x = 0; x < Experiment.kNumEpisodes; ++x) {
	    RLGlue.RL_episode(0);
	    System.out.print(".");
	    System.out.println( "RL_agent_message:" + RLGlue.RL_agent_message("RL_agent_message") );
	    System.out.println( "RL_env_message:" + RLGlue.RL_env_message("RL_env_message") );
	    rlNumSteps[x] = RLGlue.RL_num_steps();
	    rlReturn[x] = RLGlue.RL_return();
	}
	
	/*add up all the steps and all the returns*/
	for (int i = 0; i < Experiment.kNumEpisodes; ++i) {
	    avgSteps += rlNumSteps[i];
	    avgReturn += rlReturn[i];
	}
	
	System.out.println( "RL_agent_message:" + RLGlue.RL_agent_message("RL_agent_message") );
	System.out.println( "RL_env_message:" + RLGlue.RL_env_message("RL_env_message") );

	/*average steps and returns*/
	avgSteps /= (double)Experiment.kNumEpisodes;
	avgReturn /= (double)Experiment.kNumEpisodes;
	
	/*print out results*/
	System.out.println("\n-----------------------------------------------\n");
	System.out.println("Number of episodes: " + Experiment.kNumEpisodes);
	System.out.println("Average number of steps per episode: " +  avgSteps);
	System.out.println("Average return per episode: " + avgReturn);
	System.out.println("-----------------------------------------------\n");
    }
    
    public static void main(String [] args) throws IOException {
	RLGlue.RL_init();
	System.out.println("Experiment.runEpisodic");
	Experiment.runEpisodic();
	Experiment.runStepwise();
	RLGlue.RL_cleanup();
    }   
}
