import java.lang.*;
import java.io.*;
import java.util.*;

public class RL_util
{
    public static int [] numberSteps;
    public static int cummulativeReward;
    public static int utilCount;
    public static FileWriter f1;
    public static FileWriter f2;
    public static FileWriter f3;
    public static double endAverageReward, endCumReward;

     
    public static void initStats(int num_episodes) throws IOException
    {
        f1 =  new FileWriter("Data/averageRewardChallenge_"+num_episodes+".dat");
        f2 =  new FileWriter("Data/numberstepsChallenge_"+num_episodes+".dat");
        f3 =  new FileWriter("Data/cummulativeRewardChallenge_"+num_episodes+".dat");
        
        numberSteps = new int[num_episodes];
        cummulativeReward = 0;
        utilCount = 0;
    }
    
    public static void collectStats() throws IOException   
    {
        f1.write(pipes_main.RL_average_reward()+"\n");
        f2.write(pipes_main.RL_num_steps()+"\n");
        cummulativeReward += pipes_main.RL_return();

        f3.write(cummulativeReward+"\n");
        
        endAverageReward = pipes_main.RL_average_reward();
        endCumReward = cummulativeReward;
        numberSteps[utilCount] = pipes_main.RL_num_steps();
        utilCount++;
    }
    public static void endStats(int num_episodes)throws IOException
    {
        double num = 0, sum = 0;
        double meanSteps, varOfSteps;
        
        
        for(int x=0; x< num_episodes; x++)
        {
                sum += numberSteps[x];
        }	
        meanSteps = sum/num_episodes;
        for(int x=0; x< num_episodes; x++)
        {
                num = num + (numberSteps[x] - meanSteps)*(numberSteps[x] - meanSteps);
        }
        varOfSteps = num/(num_episodes - 1);
        
        System.out.println("\nDATA STATS::");        
        System.out.println("-------------------------------------------------------");
        System.out.println("The mean number of steps is = " + meanSteps);
        System.out.println("The variance in the number of steps is = "+varOfSteps);
            System.out.println("--");
            System.out.println("The final average reward is = "+endAverageReward);
            System.out.println("The final cummulative reward is = "+endCumReward);        
        System.out.println("-------------------------------------------------------");
        
        f1.close();
        f2.close();
        f3.close();
    }
}