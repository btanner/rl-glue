import java.io.*;
import java.lang.*;
import java.util.regex.*;

public class Java_agent_parser{
        
        float version;			
        char episodic;			
        int obs_dim;			
        char [] obs_types;	    
        float [] obs_mins;           
        float [] obs_maxs;			
        int action_dim;			
        char [] action_types;		
        float [] action_mins;		
        float [] action_maxs;	

public void parse(String ts)
{
    String [] tmp = null;
    String [] tmp2 = null;

//    String [] tmp2 = null;
    String [] tmp3 = null;

try {
    tmp = ts.split(":");
	
    
    version = Float.parseFloat(tmp[0]);    
    episodic = (tmp[1].trim()).charAt(0);
	
    tmp2 = tmp[2].split("_");

    obs_dim = Integer.parseInt(tmp2[0]);
    obs_mins = new float[obs_dim];           
    obs_maxs = new float[obs_dim]; 
    obs_types = new char[obs_dim];
        
    tmp3 = tmp2[1].split(",");
    
    if(obs_dim == 1)
        obs_types[0] = tmp3[0].substring(1,tmp3[0].length()-1).trim().charAt(0);
    else
    {
            
        tmp3[0] = tmp3[0].substring(1,tmp3[0].length()); 
        tmp3[obs_dim-1] = tmp3[obs_dim-1].substring(0, tmp3[obs_dim-1].length()-1);
    
        for(int i = 0; i < obs_dim; i++)
            obs_types[i] = (tmp3[i].trim()).charAt(0);
    }

    for(int i = 2; i < obs_dim + 2; i++)
    {
        tmp3 = null;
        tmp3 = tmp2[i].split(",");
        tmp3[0] = tmp3[0].substring(1,tmp3[0].length());
        tmp3[1] = tmp3[1].substring(0, tmp3[1].length()-1);
        obs_mins[i-2] = Float.parseFloat(tmp3[0]);
        obs_maxs[i-2] = Float.parseFloat(tmp3[1]);
    }
    
    tmp2 = null;
    tmp2 = tmp[3].split("_");

    action_dim = Integer.parseInt(tmp2[0]);
    action_mins = new float[action_dim];           
    action_maxs = new float[action_dim]; 
    action_types = new char[action_dim];
        
    tmp3 = tmp2[1].split(",");
    
    if(action_dim == 1)
        action_types[0] = tmp3[0].substring(1,tmp3[0].length()-1).trim().charAt(0);        
    else
    {   
        tmp3[0] = tmp3[0].substring(1,tmp3[0].length()); 
        tmp3[action_dim-1] = tmp3[action_dim-1].substring(0, tmp3[action_dim-1].length()-1);
    
        for(int i = 0; i < action_dim; i++)
            action_types[i] = (tmp3[i].trim()).charAt(0);
    }

    for(int i = 2; i < action_dim + 2; i++)
    {
        tmp3 = null;
        tmp3 = tmp2[i].split(",");
        tmp3[0] = tmp3[0].substring(1,tmp3[0].length());
        tmp3[1] = tmp3[1].substring(0, tmp3[1].length()-1);
        action_mins[i-2] = Float.parseFloat(tmp3[0]);
        action_maxs[i-2] = Float.parseFloat(tmp3[1]);
    }
    
    }catch (Exception e){
        System.out.println("\nError: Your Task Specification has incorrect format! \nExiting ... \n");
        System.exit(0);
    }
    
}
}
    