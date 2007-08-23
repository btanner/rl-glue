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

		System.err.println("+ RL_agent_message: query_agent");
		String agent_response = RLGlue.RL_agent_message("query_agent");
		System.err.println("- RL_agent_message: " + agent_response);
		
		System.err.println("+ RL_env_message: query_env");
		String env_response = RLGlue.RL_env_message("query_env");
		System.err.println("- RL_env_message: " + env_response);

		/*basic main loop*/

		for (int run = 0; run < 2; ++run)
		{
			RLGlue.RL_init();

			Random_seed_key key = new Random_seed_key(2, 2);
			for (int i = 0; i < 2; ++i)
			{
			    key.intArray[i] = i;
			    key.doubleArray[i] = (double) i;
			}
			RLGlue.RL_set_random_seed(key);

			String agentMessage = RLGlue.RL_agent_message("poke");
			System.out.println(agentMessage);

			String envMessage = RLGlue.RL_env_message("prod");
			System.out.println(envMessage);

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
}
