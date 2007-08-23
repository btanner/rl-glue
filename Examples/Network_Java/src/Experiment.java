import rlglue.RLGlue;
import rlglue.types.*;
import java.io.IOException;

public class Experiment
{
	protected static final int kNumEpisodes = 100;
	protected static int rlNumSteps[];
	protected static double rlReturn[];

	protected static void run(int numEpisodes) throws IOException
	{
		/*run for num_episode number of episodes and store the number of steps and return from each episode*/        	
		for(int x = 0; x < numEpisodes; ++x) {
			RLGlue.RL_episode(0);
			System.out.print(".");
			rlNumSteps[x] = RLGlue.RL_num_steps();
			rlReturn[x] = RLGlue.RL_return();
		}
	}

	public static void main(String [] args) throws IOException {
		double avgSteps = 0.0;
		double avgReturn = 0.0;

		rlNumSteps = new int[Experiment.kNumEpisodes];
		rlReturn = new double[Experiment.kNumEpisodes];

		RLGlue.RL_init();

		String agentMessage = RLGlue.RL_agent_message("poke");
		System.out.println(agentMessage);
		
		String msg = "poke";
		
		while (msg.length() < 65536) {
		    msg += "0123456789" + msg;
		}
				
		String envMessage = RLGlue.RL_env_message(msg);
		System.err.println(msg);
		
		run(kNumEpisodes);
		RLGlue.RL_cleanup();
		
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
}
