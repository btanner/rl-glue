import java.lang.*;
import java.io.*;
import java.util.*;

public class pipes_main
{

	private static String pipe_in = "/tmp/RL_pipe_main_in";
	private static String pipe_out = "/tmp/RL_pipe_main_out";
	
    private static FileInputStream fis;
    private static InputStreamReader fr; 
    private static BufferedReader br; 
    private static FileWriter file;
	
	private static int obs_case;
	private static int act_case;

	    
	static Vector oa = new Vector();
	static Vector roat = new Vector();
	private static int [] sia;
	private static double [] sfa;
	private static int [] aia;
	private static double [] afa;
	
	private static String buf;
	
    private static StringTokenizer st;
    private static String [] token = new String[1000];


	
	public static void  RL_init() throws IOException
	{
	
		boolean errorr = true;
		while(errorr)
		{
			try
			{
				fis = new FileInputStream("/tmp/RL_pipe_main_in");
				fr = new InputStreamReader(fis);
				br = new BufferedReader(fr); 
				file = new FileWriter("/tmp/RL_pipe_main_out");

				errorr = false;
			}
			catch(Exception e)
			{
            e.printStackTrace();

				errorr = true;
			}
		}
	
		file.write("RLinit0\n");
		file.flush();
			
		oa.setSize(2);
		roat.setSize(4);
		
		buf = br.readLine();
		while(buf == null)
			buf = br.readLine(); 

		if(!(buf.trim()).equals("done"))
			RL_cleanup();


	}

	public static void  RL_init(String inpipe, String outpipe)throws IOException
	{
		boolean errorr = true;
		while(errorr)
		{
			try
			{
				fis = new FileInputStream(pipe_in);
				fr = new InputStreamReader(fis);
				br = new BufferedReader(fr); 
				file = new FileWriter(pipe_out);
			
				errorr = false;
			}
			catch(FileNotFoundException e)
			{
				errorr = true;
			}
		}
	
		file.write("RLinit1\n");
		file.flush();
		
		file.write(inpipe+"\n");
		file.flush();
		file.write(outpipe+"\n");
		file.flush();
		
		oa.setSize(2);
		roat.setSize(4);
		
		buf = br.readLine();
		while(buf == null)
			buf = br.readLine(); 

		if(!(buf.trim()).equals("done"))
			RL_cleanup();
	}

	public static Vector RL_start() throws IOException
	{
		int count = 0;
	
		try{
		
		file.write("RLstart\n");
		file.flush();		
		
		buf = br.readLine();
		while(buf == null)
			buf = br.readLine(); 
	
		st = new StringTokenizer(buf);
		
		while (st.hasMoreTokens()) 
		{
			token[count] = st.nextToken().trim();
			if(!token[count].equals("\n"))
				count++;
		}
		
		if(count == 1)
		{
			obs_case = 1;
			for(int i=0;i<token[0].length();i++)
				if(token[0].charAt(i) == '.')
					obs_case = 2;
		}
		else
		{
			obs_case = 3;
			for(int i=0; i< count;i++)
				for(int j=0; j< token[i].length(); j++)
					if(token[i].charAt(j) == '.')
						obs_case = 4;
		}
			
		if(obs_case == 1)
		{
			oa.set(0,new Integer(Integer.parseInt(token[0])));
		}
		else if(obs_case == 2)
		{
			oa.set(0,new Double(Double.parseDouble(token[0])));
		}
		else if(obs_case == 3)
		{
			sia = new int[count];
			for(int i=0;i<count;i++)
				sia[i] = Integer.parseInt(token[i]);
			oa.set(0,sia);
		}
		else
		{
			sfa = new double[count];
			for(int i=0;i<count;i++)
				sfa[i] = Double.parseDouble(token[i]);
			oa.set(0,sfa);
		}		
		
		}
        catch (Throwable e) {
            System.err.println("ERROR: Observation types not valid. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor Observation type when using pipe communication. \nExiting...\n");
            e.printStackTrace();
        }
		
		try{
		
		count = 0;
		buf = br.readLine();
		while(buf == null)
			buf = br.readLine(); 
	
		st = new StringTokenizer(buf);
		
		while (st.hasMoreTokens()) 
		{
			token[count] = st.nextToken().trim();
			if(!token[count].equals("\n"))
				count++;
		}
		
		if(count == 1)
		{
			act_case = 1;
			for(int i=0;i<token[0].length();i++)
				if(token[0].charAt(i) == '.')
					act_case = 2;
		}
		else
		{
			act_case = 3;
			for(int i=0; i< count;i++)
				for(int j=0; j< token[i].length(); j++)
					if(token[i].charAt(j) == '.')
						act_case = 4;
		}
			
		if(act_case == 1)
		{
			oa.set(1,new Integer(Integer.parseInt(token[0])));
		}
		else if(act_case == 2)
		{
			oa.set(1,new Double(Double.parseDouble(token[0])));
		}
		else if(act_case == 3)
		{
			aia = new int[count];
			for(int i=0;i<count;i++)
				aia[i] = Integer.parseInt(token[i]);
			oa.set(1,aia);
		}
		else
		{
			afa = new double[count];
			for(int i=0;i<count;i++)
				afa[i] = Double.parseDouble(token[i]);
			oa.set(1,afa);
		}	
		
		}
        catch (Throwable e) {
            System.err.println("ERROR: Action types not valid. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor Action type when using pipe communication. \nExiting...\n");
            e.printStackTrace();
        }
		
		return oa;
	}

	public static Vector RL_step()throws IOException
	{
		int count = 0;

		file.write("RLstep\n");
		file.flush();
		
		try{
		buf = br.readLine();
		while(buf == null)
			buf = br.readLine(); 

		roat.set(0,new Double(Double.parseDouble(buf.trim())));
		}
        catch (Throwable e) {
            System.err.println("ERROR: Reading reward in RL_step! \nExiting...\n");
            e.printStackTrace();
        }
		
		try {
		buf = br.readLine();
		while(buf == null)
			buf = br.readLine(); 
	
		st = new StringTokenizer(buf);
		
		while (st.hasMoreTokens()) 
		{
			token[count] = st.nextToken().trim();
			if(!token[count].equals("\n"))
				count++;
		}

		if(obs_case == 1)
		{
			roat.set(1,new Integer(Integer.parseInt(token[0])));
		}
		else if(obs_case == 2)
		{
			roat.set(1,new Double(Double.parseDouble(token[0])));
		}
		else if(obs_case == 3)
		{
			sia = new int[count];
			for(int i=0;i<count;i++)
				sia[i] = Integer.parseInt(token[i]);
			roat.set(1,sia);
		}
		else
		{
			sfa = new double[count];
			for(int i=0;i<count;i++)
				sfa[i] = Double.parseDouble(token[i]);
			roat.set(1,sfa);
		}	
		}
        catch (Throwable e) {
            System.err.println("ERROR: Reading Observation in RL_step!");
            System.err.println("Observation types may be invalid. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor Observation type when using pipe communication. \nExiting...\n");
            e.printStackTrace();
        }
		
		try{
		count = 0;
		buf = br.readLine();
		while(buf == null)
			buf = br.readLine(); 
	
		st = new StringTokenizer(buf);
		
		while (st.hasMoreTokens()) 
		{
			token[count] = st.nextToken().trim();
			if(!token[count].equals("\n"))
				count++;
		}
		
		if(act_case == 1)
		{
			roat.set(2,new Integer(Integer.parseInt(token[0])));
		}
		else if(act_case == 2)
		{
			roat.set(2,new Double(Double.parseDouble(token[0])));
		}
		else if(act_case == 3)
		{
			aia = new int[count];
			for(int i=0;i<count;i++)
				aia[i] = Integer.parseInt(token[i]);
			roat.set(2,aia);
		}
		else
		{
			afa = new double[count];
			for(int i=0;i<count;i++)
				afa[i] = Double.parseDouble(token[i]);
			roat.set(2,afa);
		}	
		}
        catch (Throwable e) {
            System.err.println("ERROR: Reading Action in RL_step!");
            System.err.println("Action types may be invalid. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor Action type when using pipe communication. \nExiting...\n");
            e.printStackTrace();
        }
			
		try{
		buf = br.readLine(); 
		while(buf == null)
			buf = br.readLine(); 
			
		roat.set(3,new Boolean(Boolean.getBoolean(buf.trim())));
		}
        catch (Throwable e) {
            System.err.println("ERROR: Reading terminal flag in RL_step! \nExiting...\n");
            e.printStackTrace();
        }
		return roat;
	}

	public static double RL_return()throws IOException
	{	
		try{
		
		file.write("RLreturn\n");
		file.flush();
		
		buf = br.readLine(); 
		while(buf == null)
			buf = br.readLine(); 
		}
        catch (Throwable e) {
            System.err.println("ERROR: Reading reward in RL_return! \nExiting...\n");
            e.printStackTrace();
        }
		
		return Double.parseDouble(buf.trim());
	}

	public static double RL_average_reward()throws IOException
	{	
		try{
		
		file.write("RLaveragereward\n");
		file.flush();
		
		buf = br.readLine();
		while(buf == null)
			buf = br.readLine(); 
		}
        catch (Throwable e) {
            System.err.println("ERROR: Reading reward in RL_average_reward! \nExiting...\n");
            e.printStackTrace();
        }
		return Double.parseDouble(buf.trim());
	}

	public static double RL_average_num_steps()throws IOException
	{
		try {
		
		file.write("RLaveragenumsteps\n");
		file.flush();
		
		buf = br.readLine(); 
		while(buf == null)
			buf = br.readLine(); 
        }
		catch (Throwable e) {
            System.err.println("ERROR: Reading num steps in RL_average_num_steps! \nExiting...\n");
            e.printStackTrace();
        }
		return Double.parseDouble(buf.trim());
	}

	public static int RL_num_steps()throws IOException
	{
		try{
		
		file.write("RLnumsteps\n");
		file.flush();
		
		buf = br.readLine(); 
		while(buf == null)
			buf = br.readLine(); 
		
        }
		catch (Throwable e) {
            System.err.println("ERROR: Reading num steps in RL_num_steps! \nExiting...\n");
            e.printStackTrace();
        }
		
		return Integer.parseInt(buf.trim());
	}

	public static int RL_num_episodes()throws IOException
	{
		try {
		
		file.write("RLnumepisodes\n");
		file.flush();
		
		buf = br.readLine(); 
		while(buf == null)
			buf = br.readLine(); 
		}
		catch (Throwable e) {
            System.err.println("ERROR: Reading num episodes in RL_num_episodes! \nExiting...\n");
            e.printStackTrace();
        }
		return Integer.parseInt(buf.trim());
	}

	public static void RL_episode()throws IOException
	{
		try{
		
		file.write("RLepisode0\n");
		file.flush();
		
		buf = br.readLine(); 
		while(buf == null)
			buf = br.readLine(); 
		if(!(buf.trim()).equals("done"))
			RL_cleanup();
			
		}
		catch (Throwable e) {
            System.err.println("ERROR: in RL_episode! \nExiting...\n");
            e.printStackTrace();
        }
	}

	public static void RL_episode(int num_Steps)throws IOException
	{
		try{
		
		file.write("RLepisode1\n");
		file.flush();
		file.write(num_Steps + "\n");
		file.flush();
		
		buf = br.readLine(); 
		while(buf == null)
			buf = br.readLine(); 
		if(!(buf.trim()).equals("done"))
			RL_cleanup();
			
		}
		catch (Throwable e) {
            System.err.println("ERROR: in RL_episode! \nExiting...\n");
            e.printStackTrace();
        }			
	}

	public static void RL_set_state(int sk)throws IOException
	{
		try {
		
		file.write("RLsetstate\n");
		file.flush();
		
		file.write(sk + "\n");
		file.flush();
		
		buf = br.readLine();
		while(buf == null)
			buf = br.readLine(); 

		if(!(buf.trim()).equals("done"))
			RL_cleanup();
		}
		catch (Throwable e) {
            System.err.println("ERROR: in RL_set_state!");
            System.err.println("State key types may be invalid. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor State key type when using pipe communication. \nExiting...\n");
            e.printStackTrace();
        }
	}

	public static void RL_set_state(float sk)throws IOException
	{
		try{
		
		file.write("RLsetstate\n");
		file.flush();
		
		file.write(sk + "\n");
		file.flush();
		
		buf = br.readLine();
		while(buf == null)
			buf = br.readLine(); 

		if(!(buf.trim()).equals("done"))
			RL_cleanup();
			
		}
		catch (Throwable e) {
            System.err.println("ERROR: in RL_set_state!");
            System.err.println("State key types may be invalid. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor State key type when using pipe communication. \nExiting...\n");
            e.printStackTrace();
        }
	}

	public static void RL_set_state(int [] sk)throws IOException
	{
		try {
		file.write("RLsetstate\n");
		file.flush();
		
		buf ="";
		
		for(int i=0;i<sk.length;i++)
			buf = buf + sk[i] +" ";
		
		file.write(buf.trim()+"\n");
		file.flush();
		
		buf = br.readLine();
		while(buf == null)
			buf = br.readLine(); 

		if(!(buf.trim()).equals("done"))
			RL_cleanup();
		}
		catch (Throwable e) {
            System.err.println("ERROR: in RL_set_state!");
            System.err.println("State key types may be invalid. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor State key type when using pipe communication. \nExiting...\n");
            e.printStackTrace();
        }
	}

	public static void RL_set_state(double [] sk)throws IOException
	{
		try {
		
		file.write("RLsetstate\n");
		file.flush();
		
		buf ="";
		
		for(int i=0;i<sk.length;i++)
			buf = buf + sk[i] +" ";
		
		file.write(buf.trim()+"\n");
		file.flush();
		
		buf = br.readLine();
		while(buf == null)
			buf = br.readLine(); 

		if(!(buf.trim()).equals("done"))
			RL_cleanup();
		}
		catch (Throwable e) {
            System.err.println("ERROR: in RL_set_state!");
            System.err.println("State key types may be invalid. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor State key type when using pipe communication. \nExiting...\n");
            e.printStackTrace();
        }
	}

	public static void RL_set_random_seed(int rsk)throws IOException
	{
		try {
		
		file.write("RLsetrandom\n");
		file.flush();
		
		file.write(rsk + "\n");
		file.flush();
		
		buf = br.readLine();
		while(buf == null)
			buf = br.readLine(); 

		if(!(buf.trim()).equals("done"))
			RL_cleanup();
		}
		catch (Throwable e) {
            System.err.println("ERROR: in RL_set_random_seed!");
            System.err.println("Random key types may be invalid. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor Random key type when using pipe communication. \nExiting...\n");
            e.printStackTrace();
        }
	}

	public static void RL_set_random_seed(double rsk)throws IOException
	{
		try {
		
		file.write("RLsetrandom\n");
		file.flush();
		
		file.write(rsk + "\n");
		file.flush();
		
		buf = br.readLine();
		while(buf == null)
			buf = br.readLine(); 

		if(!(buf.trim()).equals("done"))
			RL_cleanup();
		}
		catch (Throwable e) {
            System.err.println("ERROR: in RL_set_random_seed!");
            System.err.println("Random key types may be invalid. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor Random key type when using pipe communication. \nExiting...\n");
            e.printStackTrace();
        }
	}
	
	public static void RL_set_random_seed(int [] rsk)throws IOException
	{
		try {
		
		file.write("RLsetrandom\n");
		file.flush();
		
		buf ="";
		
		for(int i=0;i<rsk.length;i++)
			buf = buf + rsk[i] +" ";
		
		file.write(buf.trim()+"\n");
		file.flush();
		
		buf = br.readLine();
		while(buf == null)
			buf = br.readLine(); 

		if(!(buf.trim()).equals("done"))
			RL_cleanup();
		}
		catch (Throwable e) {
            System.err.println("ERROR: in RL_set_random_seed!");
            System.err.println("Random key types may be invalid. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor Random key type when using pipe communication. \nExiting...\n");
            e.printStackTrace();
        }
	}
	
	public static void RL_set_random_seed(double [] rsk)throws IOException
	{
		try {
		
		file.write("RLsetrandom\n");
		file.flush();
		
		buf ="";
		
		for(int i=0;i<rsk.length;i++)
			buf = buf + rsk[i] +" ";
		
		file.write(buf.trim()+"\n");
		file.flush();
		
		buf = br.readLine();
		while(buf == null)
			buf = br.readLine(); 

		if(!(buf.trim()).equals("done"))
			RL_cleanup();
		}
		catch (Throwable e) {
            System.err.println("ERROR: in RL_set_random_seed!");
            System.err.println("Random key types may be invalid. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor Random key type when using pipe communication. \nExiting...\n");
            e.printStackTrace();
        }
	}
	
	public static String RL_get_state()throws IOException
	{	
		try {
		file.write("RLgetstate\n");
		file.flush();
		
		buf = br.readLine(); 
		while(buf == null)
			buf = br.readLine(); 
		}
		catch (Throwable e) {
            System.err.println("ERROR: in RL_get_state!\nExiting...\n");
            e.printStackTrace();
        }
		return buf;
	}

	public static String RL_get_random_seed()throws IOException
	{
		try {
		file.write("RLgetrandomseed\n");
		file.flush();
		
		buf = br.readLine(); 
		while(buf == null)
			buf = br.readLine(); 
		}
		catch (Throwable e) {
            System.err.println("ERROR: in RL_get_random_seed!\nExiting...\n");
		}
		return buf;
	}

	public static void RL_cleanup() throws IOException
	{
		try {
		file.write("RLcleanup\n");
		file.flush();
		System.exit(0);		
		}
		catch (Throwable e) {
            System.err.println("ERROR: in RL_cleanup!\nExiting...\n");
		}
	}
}