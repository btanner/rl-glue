//Must include all the following files
import java.lang.*;
import java.io.*;
import java.util.*;

public class main
{

//MAIN PROGRAM
public static void main(String[] args) throws IOException
{
		int num_episodes = 1000;
                
		pipes_main.RL_init();					//initialize interface
		RL_util.initStats(num_episodes);
		System.out.println("Beggining experiment!!!");		
		for(int x=0; x < num_episodes; x++)
        {
            pipes_main.RL_episode();	//run a single episode
            System.out.print(".");
			RL_util.collectStats();
        }
		System.out.println();

		RL_util.endStats(num_episodes);
		pipes_main.RL_cleanup();
}
}
