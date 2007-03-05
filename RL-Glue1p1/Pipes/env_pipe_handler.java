import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;

public class env_pipe_handler {

    private static String pipe_in;
    private static String pipe_out;
    private static String javaName; 
    private static int global_count = 0;
    
    private static Class agent; 
    private static Object retobj;
    
    private static FileInputStream fis;
    private static InputStreamReader fr; 
    private static BufferedReader br; 
    private static FileWriter file;
    
    private static StringTokenizer st;
    private static String [] token = new String[100000];
    
    private static int ai;
    private static double af;
    private static int [] aia;
    private static double [] afa;

    private static int obs_case;
    private static int action_case;
    private static boolean first_step;
    
    public static void main(String[] args) throws IOException{

        if(args.length == 3)
        {
		  System.out.println("\nUsing user pipe names!:");
		  System.out.println("\t..."+args[1]+"\n\t..."+args[2]+"\n");
		  System.out.println("\nExecutable name: "+args[0]);
            pipe_in = args[1];
            pipe_out = args[2];
            javaName = args[0];
        }
        else if(args.length == 1)
        {
            System.out.println("\nUsing default pipe names!");
		    System.out.println("\t.../tmp/RL_pipe_env_in\n\t.../tmp/RL_pipe_env_out");
            System.out.println("\nExecutable name: "+args[0]);
            pipe_in = "/tmp/RL_pipe_env_in";
            pipe_out = "/tmp/RL_pipe_env_out";
            javaName = args[0];
        }
        else
	    {
		  System.out.println("\nError: incorrect input arguments.\n\nExample of valid invokations:\n java Java_env_handler mines /tmp/RL_pipe_env_in /tmp/RL_pipe_env_out\n...or...\njava Java_env_handler mines\n\nExiting...\n\n");
		  System.exit(0);
	    }                

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

        try {
            agent = Class.forName(javaName);
            Class partypes[] = null;
            Constructor ct = agent.getConstructor(partypes);
            Object arglist[] = null;
            retobj = ct.newInstance(arglist);

            String buf = br.readLine();

            while(true)
            {    
                if(buf != null)
                { 
                    if(buf.equals("init"))
                    {   
                        pipe_env_init();
                    }
                    else if(buf.equals("start")) 
                    {   
                        pipe_env_start();
                    }        
                    else if(buf.equals("step"))    
                    {
                        pipe_env_step();
                    }
                    else if(buf.equals("getstate"))   
                    {
                        pipe_env_get_state();
                    }
                    else if(buf.equals("setstate"))   
                    {
                        pipe_env_set_state();
                    }
                    else if(buf.equals("getrandom"))   
                    {
                        pipe_env_get_random_seed();
                    }
                    else if(buf.equals("setrandom"))   
                    {
                        pipe_env_set_random_seed();
                    }
                    else if(buf.equals("cleanup")) 
                    {
                        partypes = null;
                        Method meth = agent.getMethod("env_cleanup",partypes);
                        arglist = null;
                        meth.invoke(retobj, arglist);
                        System.exit(0);
                    }     
                }   
                buf = br.readLine();
            }
        }
        catch (Throwable e) {
            System.err.println("\nError: BAD COMMAND!!   OR  Error in env_cleanup method\n");
            e.printStackTrace();
        }
    }

    private static void pipe_env_init() throws IOException
    {
        first_step = true;
        try{            
            Class partypes[] = null;
            Method meth = agent.getMethod("env_init", partypes);
            Object arglist[] = null;
            Object retobj2 = meth.invoke(retobj, arglist);
            String retval = (String)retobj2;
            
            String buf = String.valueOf(retval);
            
            file.write(buf+"\n");   
            file.flush();
            
        }
        catch (InvocationTargetException e)
        {
            System.err.println("\nError: Error in your env_init method!!\n");
            e.printStackTrace();
        }
        catch (Throwable e) {
            System.err.println("ERROR1: Arguments are not valid. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor Observation type when using pipe communication. \nExiting...\n");
            e.printStackTrace();
        } 
    }
 
    private static void pipe_env_start() throws IOException
    {
        int s, count = 0;
        String buf;
        
        try{        
            Method meth = agent.getMethod("env_start", null);
            Object retobj2 = meth.invoke(retobj, null);
            
            buf = "";
            if( retobj2  instanceof Integer)
            {
                obs_case = 1;
                Integer retval1 = (Integer)retobj2;
                 buf = String.valueOf(retval1.intValue());
            }
            else if(retobj2  instanceof Double)    
            {
                obs_case = 2;

                Double retval2 = (Double)retobj2;
                 buf = String.valueOf(retval2.doubleValue());
            }
            else if ( Class.forName ( "[I" ).isInstance ( retobj2 ) )
            {
                obs_case = 3;

                buf = "";
                for(int i=0;i< Array.getLength(retobj2);i++)
                    buf = buf + String.valueOf(Array.getInt(retobj2,i))+" ";
                    buf = buf.trim();
            }
            else if (Class.forName ( "[D" ).isInstance ( retobj2 )  )
            {
                obs_case = 4;

                buf = "";
                for(int i=0;i< Array.getLength(retobj2);i++)
                    buf = buf + String.valueOf(Array.getDouble(retobj2,i)) + " ";
                    buf = buf.trim();
                    
            }
            else
            {
                System.out.println("ERROR2: Observation is not a valid type. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor Observation type when using pipe communication. \nExiting...\n");
                System.exit(1);
            }
            
            file.write(buf+"\n");
            file.flush();
            
        
        
        }
        catch (InvocationTargetException e)
        {
            System.err.println("\nError: Error in your env_state method!!\n");
            e.printStackTrace();
        }
        catch (Throwable e) {
            System.err.println("ERROR3: Arguments are not valid. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor Observation type when using pipe communication. \nExiting...\n");
            e.printStackTrace();
        }              
    }

    private static void pipe_env_step() throws IOException
    {
        String buf;
        int act, count=0;
        Class partypes[];
        Method meth;
        Object arglist[];
        Object retobj2 = null;
        String buf2;
            double r;
        boolean term;
        
    
        try{ 
        
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
 
            if(first_step)
            {           
                if(count == 1)
                {
                    action_case = 1;
                    for(int i=0;i<token[0].length();i++)
                        if(token[0].charAt(i) == '.')
                            action_case = 2;
                }
                else
                {
                    action_case = 3;
                    for(int i=0; i< count;i++)
                        for(int j=0; j< token[i].length(); j++)
                            if(token[i].charAt(j) == '.')
                                action_case = 4;
                }
            }
            first_step = false;
            if(action_case == 1)
            {
                ai = Integer.parseInt(token[0]);  
               
                partypes = new Class[1];
                partypes[0] = Integer.TYPE;
                meth = agent.getMethod("env_step", partypes);
                arglist = new Object[1];
                arglist[0] = new Integer(ai);
                retobj2 = meth.invoke(retobj, arglist);
            }
            else if(action_case == 2)
            {
                af = Double.parseDouble(token[0]);  
               
                partypes = new Class[1];
                partypes[0] = Double.TYPE;
                meth = agent.getMethod("env_step", partypes);
                arglist = new Object[1];
                arglist[0] = new Double(af);
                retobj2 = meth.invoke(retobj, arglist);
            }
            else if(action_case == 3)
            {
                aia = new int[count];
                for(int i=0;i<count;i++)
                    aia[i] = Integer.parseInt(token[i]);
             
                partypes = new Class[1];
                partypes[0] = Class.forName("[I");
                meth = agent.getMethod("env_step", partypes);
                arglist = new Object[1];
                arglist[0] = aia;
                retobj2 = meth.invoke(retobj, arglist);   
            }
            else if(action_case == 4)
            {
                afa = new double[count];
                for(int i=0;i<count;i++)
                    afa[i] = Double.parseDouble(token[i]);
             
                partypes = new Class[1];
                partypes[0] = Class.forName("[D");
                meth = agent.getMethod("env_step", partypes);
                arglist = new Object[1];
                arglist[0] = afa;
                retobj2 = meth.invoke(retobj, arglist);   
            }

        
            Vector retval = (Vector)retobj2;
            r = ((Double)(retval.get(1))).doubleValue();
            term = ((Boolean)(retval.get(2))).booleanValue();
            
            buf2 = "";
            if(obs_case == 1)
            {
                Integer retval1 = (Integer)retval.get(0);
                 buf2 = String.valueOf(retval1.intValue());
            }
            else if(obs_case == 2)    
            {
                Double retval2 = (Double)retval.get(0);
                 buf2 = String.valueOf(retval2.doubleValue());
            }
            else if (obs_case == 3)
            {
                buf2 = "";
                for(int i=0;i< Array.getLength(retval.get(0));i++)
                    buf2 = buf2 + String.valueOf(Array.getInt(retval.get(0),i)) + " ";
                    buf2 = buf2.trim();
            }
            else if (obs_case == 4 )
            {
                buf2 = "";
                for(int i=0;i< Array.getLength( retval.get(0));i++)
                    buf2 = buf2 + String.valueOf(Array.getDouble( retval.get(0),i)) + " ";
                    buf2 = buf2.trim();
            }

        
            file.write(buf2+"\n");
            file.flush();
        
            buf2 = String.valueOf(r);
            file.write(buf2+"\n");
            file.flush();
        
            if(term){
                file.write("1"+"\n");
            }
            else
                file.write("0"+"\n");
            
            file.flush();    
            
        }
        catch (InvocationTargetException e)
        {
            System.err.println("\nError: Error in your env_step method!!\n");
            e.printStackTrace();
        }
        catch (Throwable e) {
            System.err.println("ERROR5: Arguments are not valid. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor Observation type when using pipe communication. \nExiting...\n");
            e.printStackTrace();
        }    
    }
    
    private static void pipe_env_get_state() throws IOException
    {
        String buf;
    
        try{        
            Method meth = agent.getMethod("env_get_state", null);
            Object retobj2 = meth.invoke(retobj, null);
            
            buf = "";
            if( retobj2  instanceof Integer)
            {
        
                Integer retval1 = (Integer)retobj2;
                 buf = String.valueOf(retval1.intValue());
            }
            else if(retobj2  instanceof Double)    
            {
        
                Double retval2 = (Double)retobj2;
                 buf = String.valueOf(retval2.doubleValue());
            }
            else if ( Class.forName ( "[I" ).isInstance ( retobj2 ) )
            {
        
                buf = "";
                for(int i=0;i< Array.getLength(retobj2);i++)
                    buf = buf + String.valueOf(Array.getInt(retobj2,i))+" ";
                    buf = buf.trim();
            }
            else if (Class.forName ( "[D" ).isInstance ( retobj2 )  )
            {
        
                buf = "";
                for(int i=0;i< Array.getLength(retobj2);i++)
                    buf = buf + String.valueOf(Array.getDouble(retobj2,i)) + " ";
                    buf = buf.trim();
                    
            }
            else
            {
                System.out.println("ERROR6: State_key is not a valid type. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor Observation type when using pipe communication. \nExiting...\n");
                System.exit(1);
            }
            
            file.write(buf+"\n");
            file.flush();
            
        
        
        }
        catch (InvocationTargetException e)
        {
            System.err.println("\nError: Error in your env_get_state method!!\n");
            e.printStackTrace();
        }
        catch (Throwable e) {
            file.write("NOTIMPLEMENTED"+"\n");
            file.flush();
        }              
    }
    
    private static void pipe_env_get_random_seed() throws IOException
    {
        String buf;
    
        try{        
            Method meth = agent.getMethod("env_get_random_seed", null);
            Object retobj2 = meth.invoke(retobj, null);
            
            buf = "";
            if( retobj2  instanceof Integer)
            {
        
                Integer retval1 = (Integer)retobj2;
                 buf = String.valueOf(retval1.intValue());
            }
            else if(retobj2  instanceof Double)    
            {
        
                Double retval2 = (Double)retobj2;
                 buf = String.valueOf(retval2.doubleValue());
            }
            else if ( Class.forName ( "[I" ).isInstance ( retobj2 ) )
            {
        
                buf = "";
                for(int i=0;i< Array.getLength(retobj2);i++)
                    buf = buf + String.valueOf(Array.getInt(retobj2,i))+" ";
                    buf = buf.trim();
            }
            else if (Class.forName ( "[D" ).isInstance ( retobj2 )  )
            {
        
                buf = "";
                for(int i=0;i< Array.getLength(retobj2);i++)
                    buf = buf + String.valueOf(Array.getDouble(retobj2,i)) + " ";
                    buf = buf.trim();
                    
            }
            else
            {
                System.out.println("ERROR7: Random_seed_key is not a valid type. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor Observation type when using pipe communication. \nExiting...\n");
                System.exit(1);
            }
            
            file.write(buf+"\n");
            file.flush();
            
        
        
        }
        catch (InvocationTargetException e)
        {            
            System.err.println("\nError: Error in your env_get_random_seed method!!\n");
            e.printStackTrace();
        }
        catch (Throwable e) {
            file.write("NOTIMPLEMENTED"+"\n");
            file.flush();

        }              
    }

    private static void pipe_env_set_state() throws IOException
    {
        String buf;
        int act, count=0, action_case;
        Class partypes[];
        Method meth;
        Object arglist[];
        Object retobj2 = null;
        String buf2;
            double r;
        boolean term;
        
    
        try{ 
        
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
                action_case = 1;
                for(int i=0;i<token[0].length();i++)
                    if(token[0].charAt(i) == '.')
                        action_case = 2;
            }
            else
            {
                action_case = 3;
                for(int i=0; i< count;i++)
                    for(int j=0; j< token[i].length(); j++)
                        if(token[i].charAt(j) == '.')
                            action_case = 4;
            }
            
            if(action_case == 1)
            {
                ai = Integer.parseInt(token[0]);  
               
                partypes = new Class[1];
                partypes[0] = Integer.TYPE;
                meth = agent.getMethod("env_set_state", partypes);
                arglist = new Object[1];
                arglist[0] = new Integer(ai);
                meth.invoke(retobj, arglist);
            }
            else if(action_case == 2)
            {
                af = Double.parseDouble(token[0]);  
               
                partypes = new Class[1];
                partypes[0] = Double.TYPE;
                meth = agent.getMethod("env_set_state", partypes);
                arglist = new Object[1];
                arglist[0] = new Double(af);
                meth.invoke(retobj, arglist);
            }
            else if(action_case == 3)
            {
                aia = new int[count];
                for(int i=0;i<count;i++)
                    aia[i] = Integer.parseInt(token[i]);
             
                partypes = new Class[1];
                partypes[0] = Class.forName("[I");
                meth = agent.getMethod("env_set_state", partypes);
                arglist = new Object[1];
                arglist[0] = aia;
                meth.invoke(retobj, arglist);   
            }
            else if(action_case == 4)
            {
                afa = new double[count];
                for(int i=0;i<count;i++)
                    afa[i] = Double.parseDouble(token[i]);
             
                partypes = new Class[1];
                partypes[0] = Class.forName("[D");
                meth = agent.getMethod("env_set_state", partypes);
                arglist = new Object[1];
                arglist[0] = afa;
                meth.invoke(retobj, arglist);   
            }
        
            file.write("IMPLEMENTED"+"\n");
            file.flush();
        }
        catch (InvocationTargetException e)
        {   
            System.err.println("\nError: Error in your env_set_state method!!\n");
            e.printStackTrace();
        }
        catch (Throwable e) {
            file.write("NOTIMPLEMENTED"+"\n");
            file.flush();

        }    
    }

    private static void pipe_env_set_random_seed() throws IOException
    {
        String buf;
        int act, count=0, action_case;
        Class partypes[];
        Method meth;
        Object arglist[];
        Object retobj2 = null;
        String buf2;
            double r;
        boolean term;
        
    
        try{ 
        
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
                action_case = 1;
                for(int i=0;i<token[0].length();i++)
                    if(token[0].charAt(i) == '.')
                        action_case = 2;
            }
            else
            {
                action_case = 3;
                for(int i=0; i< count;i++)
                    for(int j=0; j< token[i].length(); j++)
                        if(token[i].charAt(j) == '.')
                            action_case = 4;
            }
            
            if(action_case == 1)
            {
                ai = Integer.parseInt(token[0]);  
               
                partypes = new Class[1];
                partypes[0] = Integer.TYPE;
                meth = agent.getMethod("env_set_random_seed", partypes);
                arglist = new Object[1];
                arglist[0] = new Integer(ai);
                meth.invoke(retobj, arglist);
            }
            else if(action_case == 2)
            {
                af = Double.parseDouble(token[0]);  
               
                partypes = new Class[1];
                partypes[0] = Double.TYPE;
                meth = agent.getMethod("env_set_random_seed", partypes);
                arglist = new Object[1];
                arglist[0] = new Double(af);
                meth.invoke(retobj, arglist);
            }
            else if(action_case == 3)
            {
                aia = new int[count];
                for(int i=0;i<count;i++)
                    aia[i] = Integer.parseInt(token[i]);
             
                partypes = new Class[1];
                partypes[0] = Class.forName("[I");
                meth = agent.getMethod("env_set_random_seed", partypes);
                arglist = new Object[1];
                arglist[0] = aia;
                meth.invoke(retobj, arglist);   
            }
            else if(action_case == 4)
            {
                afa = new double[count];
                for(int i=0;i<count;i++)
                    afa[i] = Double.parseDouble(token[i]);
             
                partypes = new Class[1];
                partypes[0] = Class.forName("[D");
                meth = agent.getMethod("env_set_random_seed", partypes);
                arglist = new Object[1];
                arglist[0] = afa;
                meth.invoke(retobj, arglist);   
            }

        
            file.write("IMPLEMENTED"+"\n");
            file.flush();    
        }
        catch (InvocationTargetException e)
        {            
            System.err.println("\nError: Error in your env_set_random_seed method!!\n");
            e.printStackTrace();
        }
        catch (Throwable e) {
            file.write("NOTIMPLEMENTED"+"\n");
            file.flush();
            

        }    
    }


}
