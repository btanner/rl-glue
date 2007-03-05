import java.io.*;
import java.lang.*;
import java.util.regex.*;
import java.lang.reflect.*;
import java.util.*;

public class agent_pipe_handler 
{
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
    private static String [] token = new String[100];
    
    private static int si;
    private static double sf;
    private static int [] sia;
    private static double [] sfa;
    
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
		    System.out.println("\t.../tmp/RL_pipe_agent_in\n\t.../tmp/RL_pipe_agent_out");
            System.out.println("\nExecutable name: "+args[0]);
            pipe_in = "/tmp/RL_pipe_agent_in";
            pipe_out = "/tmp/RL_pipe_agent_out";
            javaName = args[0];
        }
        else
	    {
		  System.out.println("\nError: incorrect input arguments.\n\nExample of valid invokations:\n java Java_agent_handler mineAgent /tmp/RL_pipe_agent_in /tmp/RL_pipe_agent_out\n...or...\njava Java_agent_handler mineAgent\n\nExiting...\n\n");
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
                        pipe_agent_init();
                    }
                    else if(buf.equals("start")) 
                    {   
                        pipe_agent_start();
                    }        
                    else if(buf.equals("step"))    
                    {
                        pipe_agent_step();
                    }
                    else if(buf.equals("end"))    
                    {
                        pipe_agent_end();
                    }
                    else if(buf.equals("cleanup")) 
                    {
                        partypes = null;
                        Method meth = agent.getMethod("agent_cleanup",partypes);
                        arglist = null;
                        meth.invoke(retobj, arglist);
                        System.exit(0);
                    }     
                }   
                buf = br.readLine();
            }
        }
        catch (Throwable e) {
            System.err.println("\nERROR: BAD COMMAN!!  OR Error in agent_cleanup method\n");
            e.printStackTrace();
        }
    }

    private static void pipe_agent_init() throws IOException
    {
        String buf;
        
        buf = br.readLine();
        while(buf == null)
            buf = br.readLine();
    
        try{            
            Class partypes[] = new Class[1];
            partypes[0] = String.class;
            Method meth = agent.getMethod("agent_init", partypes);
            Object arglist[] = new Object[1];
            arglist[0] = new String(buf);
            
            meth.invoke(retobj, arglist);
        }
        catch (InvocationTargetException e)
        {
            System.err.println("\nError: Error in your agent_init method!!\n");
            e.printStackTrace();
        }
        catch (Throwable e) {
            System.err.println("ERROR: Arguments are not valid. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor Observation type when using pipe communication. \nExiting...\n");
            e.printStackTrace();
        } 
    }
 
    
    private static void pipe_agent_start() throws IOException
    {
        String buf;
        int act, count=0, obs_case;
        Class partypes[];
        Method meth;
        Object arglist[];
        Object retobj2 = null;
        String buf2;
    
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
                si = Integer.parseInt(token[0]);  
               
                partypes = new Class[1];
                partypes[0] = Integer.TYPE;
                meth = agent.getMethod("agent_start", partypes);
                arglist = new Object[1];
                arglist[0] = new Integer(si);
                retobj2 = meth.invoke(retobj, arglist);
            }
            else if(obs_case == 2)
            {
                sf = Double.parseDouble(token[0]);  
               
                partypes = new Class[1];
                partypes[0] = Double.TYPE;
                meth = agent.getMethod("agent_start", partypes);
                arglist = new Object[1];
                arglist[0] = new Double(sf);
                retobj2 = meth.invoke(retobj, arglist);
            }
            else if(obs_case == 3)
            {
                sia = new int[count];
                for(int i=0;i<count;i++)
                    sia[i] = Integer.parseInt(token[i]);
             
                partypes = new Class[1];
                partypes[0] = Class.forName("[I");
                meth = agent.getMethod("agent_start", partypes);
                arglist = new Object[1];
                arglist[0] = sia;
                retobj2 = meth.invoke(retobj, arglist);   
            }
            else if(obs_case == 4)
            {
                sfa = new double[count];
                for(int i=0;i<count;i++)
                    sfa[i] = Double.parseDouble(token[i]);
             
                partypes = new Class[1];
                partypes[0] = Class.forName("[D");
                meth = agent.getMethod("agent_start", partypes);
                arglist = new Object[1];
                arglist[0] = sfa;
                retobj2 = meth.invoke(retobj, arglist);   
            }

        
            buf2 = "";
            if( retobj2  instanceof Integer)
            {
                Integer retval1 = (Integer)retobj2;
                 buf2 = String.valueOf(retval1.intValue());
            }
            else if(retobj2  instanceof Double)    
            {
                Double retval2 = (Double)retobj2;
                 buf2 = String.valueOf(retval2.doubleValue());
            }
            else if ( Class.forName ( "[I" ).isInstance ( retobj2 ) )
            {
                buf2 = "";
                for(int i=0;i< Array.getLength(retobj2);i++)
                    buf2 = buf2 + String.valueOf(Array.getInt(retobj2,i))+" ";
                    buf2 = buf2.trim();
            }
            else if (Class.forName ( "[D" ).isInstance ( retobj2 )  )
            {
                buf2 = "";
                for(int i=0;i< Array.getLength(retobj2);i++)
                    buf2 = buf2 + String.valueOf(Array.getDouble(retobj2,i))+ " ";
                    buf2 = buf2.trim();
            }
            else
            {
                System.out.println("ERROR2: Action is not a valid type. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor Observation type when using pipe communication. \nExiting...\n");
                System.exit(1);
            }
            
            file.write(buf2+"\n");
            file.flush();
        }
        catch (InvocationTargetException e)
        {
            System.err.println("\nError: Error in your agent_start method!!\n");
            e.printStackTrace();
        }
        catch (Throwable e) {
            System.err.println("ERROR: Arguments are not valid. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor Observation type when using pipe communication. \nExiting...\n");
            e.printStackTrace();
        }              
    }

    private static void pipe_agent_step() throws IOException
    {
    
        String buf1, buf2;
        int act, count=0, obs_case;
        double r;
    
        Class partypes[];
        Method meth;
        Object arglist[];
        Object retobj2 = null;
      
        try{  
        
            buf1 = br.readLine();
            while(buf1 == null)
                buf1 = br.readLine();
                
            buf2 = br.readLine();
            while(buf2 == null || buf2.length() <= 0)
                buf2 = br.readLine();
                
            r = Double.parseDouble(buf2);
            
            st = new StringTokenizer(buf1);
            
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
                si = Integer.parseInt(token[0]);  
               
                partypes = new Class[2];
                partypes[0] = Double.TYPE;
                partypes[1] = Integer.TYPE;
                meth = agent.getMethod("agent_step", partypes);
                arglist = new Object[2];
                arglist[0] = new Double(r);
                arglist[1] = new Integer(si);
                retobj2 = meth.invoke(retobj, arglist);
            }
            else if(obs_case == 2)
            {
                sf = Double.parseDouble(token[0]);  
               
                partypes = new Class[2];
                partypes[0] = Double.TYPE;
                partypes[1] = Double.TYPE;
                meth = agent.getMethod("agent_step", partypes);
                arglist = new Object[2];
                arglist[0] = new Double(r);
                arglist[1] = new Double(sf);
                retobj2 = meth.invoke(retobj, arglist);
            }
            else if(obs_case == 3)
            {
                sia = new int[count];
                for(int i=0;i<count;i++)
                    sia[i] = Integer.parseInt(token[i]);
             
                partypes = new Class[2];
                partypes[0] = Double.TYPE;
                partypes[1] = Class.forName("[I");
                meth = agent.getMethod("agent_step", partypes);
                arglist = new Object[2];
                arglist[0] = new Double(r);
                arglist[1] = sia;
                retobj2 = meth.invoke(retobj, arglist);   
            }
            else if(obs_case == 4)
            {
                sfa = new double[count];
                for(int i=0;i<count;i++)
                    sfa[i] = Double.parseDouble(token[i]);
             
                partypes = new Class[2];
                partypes[0] = Double.TYPE;
                partypes[1] = Class.forName("[D");
                meth = agent.getMethod("agent_step", partypes);
                arglist = new Object[2];
                arglist[0] = new Double(r);
                arglist[1] = sfa;
                retobj2 = meth.invoke(retobj, arglist);   
            }

        
            buf2 = "";
        
            if( retobj2  instanceof Integer)
            {
                Integer retval1 = (Integer)retobj2;
                 buf2 = String.valueOf(retval1.intValue());
            }
            else if(retobj2  instanceof Double)    
            {
                Double retval2 = (Double)retobj2;
				buf2 = String.valueOf(retval2.doubleValue());

            }
            else if (Class.forName ( "[I" ).isInstance ( retobj2 )   )
            {
                buf2 = "";
                for(int i=0;i< Array.getLength(retobj2);i++)
                    buf2 = buf2 + String.valueOf(Array.getInt(retobj2,i))+" ";
                    buf2 = buf2.trim();
            }
            else if (Class.forName ( "[D" ).isInstance ( retobj2 )  )    
            {
                buf2 = "";
                for(int i=0;i< Array.getLength(retobj2);i++)
                    buf2 = buf2 + String.valueOf(Array.getDouble(retobj2,i))+ " ";
                    buf2 = buf2.trim();
            }
            else
            {
                System.out.println("ERROR4: Action is not a valid type. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor Observation type when using pipe communication. \nExiting...\n");
                System.exit(1);
            }
            
            file.write(buf2+"\n");
            file.flush();
        }
        catch (InvocationTargetException e)
        {
            System.err.println("\nError: Error in your agent_step method!!\n");
            e.printStackTrace();
        }
        catch (Throwable e) {
            System.err.println("ERROR: Arguments are not valid. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor Observation type when using pipe communication. \nExiting...\n");
            e.printStackTrace();
        }    
    }

    private static void pipe_agent_end() throws IOException
    {
        String buf;
        double r;
        
        buf = br.readLine();
        while(buf == null)
            buf = br.readLine();   
       
        r = Double.parseDouble(buf);
    
        try{    
            Class partypes[] = new Class[1];
            partypes[0] = Double.TYPE;
            Method meth = agent.getMethod("agent_end", partypes);
            Object arglist[] = new Object[1];
            arglist[0] = new Double(r);
            
            meth.invoke(retobj, arglist);
        }
        catch (InvocationTargetException e)
        {
            System.err.println("\nError: Error in your agent_end method!!\n");
            e.printStackTrace();
        }
        catch (Throwable e) {
            System.err.println("ERROR: Arguments are not valid. RL-Framework supports \nInt, \nDouble, \nInt Array, \nDouble Array \nfor Observation type when using pipe communication. \nExiting...\n");
            e.printStackTrace();
        }
             
    }

}
  
    
    