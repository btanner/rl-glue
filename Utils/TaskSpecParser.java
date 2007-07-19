import java.io.*;
import java.lang.*;
import java.util.regex.*;

public class TaskSpecParser
{        
    public static TaskSpecObject parse(String ts)
    {
	TaskSpecObject tsObject = new TaskSpecObject();
	String [] tmp = null;
	String [] tmp2 = null;
	String [] tmp3 = null;
	int discrete =0;
	int continuous =0;

	try 
	{
	    tmp = ts.split(":");
	   
	    tsObject.version = Float.parseFloat(tmp[0]);    
	    tsObject.episodic = (tmp[1].trim()).charAt(0);
	
	    tmp2 = tmp[2].split("_");

	    tsObject.obs_dim = Integer.parseInt(tmp2[0]);
	    tsObject.obs_mins = new float[tsObject.obs_dim];           
	    tsObject.obs_maxs = new float[tsObject.obs_dim]; 
	    tsObject.obs_types = new char[tsObject.obs_dim];
        
	    tmp3 = tmp2[1].split(",");
	    
	    if(tsObject.obs_dim == 1){
		tsObject.obs_types[0] = tmp3[0].substring(1,tmp3[0].length()-1).trim().charAt(0);
		if(tsObject.obs_types[0] == 'i')
			discrete++;
		else
			continuous++;
		}
	    else
	    {
		tmp3[0] = tmp3[0].substring(1,tmp3[0].length()); 
		tmp3[tsObject.obs_dim-1] = tmp3[tsObject.obs_dim-1].substring(0, tmp3[tsObject.obs_dim-1].length()-1);
		
		for(int i = 0; i < tsObject.obs_dim; i++){
		    tsObject.obs_types[i] = (tmp3[i].trim()).charAt(0);
			if(tsObject.obs_types[i] == 'i')
				discrete++;
			else
				continuous++;
		}
	    }

	    for(int i = 2; i < tsObject.obs_dim + 2; i++)
	    {
		tmp3 = null;
		tmp3 = tmp2[i].split(",");
		tmp3[0] = tmp3[0].substring(1,tmp3[0].length());
		tmp3[1] = tmp3[1].substring(0, tmp3[1].length()-1);
		tsObject.obs_mins[i-2] = Float.parseFloat(tmp3[0]);
		tsObject.obs_maxs[i-2] = Float.parseFloat(tmp3[1]);
	    }
		
		tsObject.num_discrete_obs_dims = discrete;
		tsObject.num_continuous_obs_dims = continuous;
		discrete =0;
		continuous =0;
    
	    tmp2 = null;
	    tmp2 = tmp[3].split("_");
	    
	    tsObject.action_dim = Integer.parseInt(tmp2[0]);
	    tsObject.action_mins = new float[tsObject.action_dim];           
	    tsObject.action_maxs = new float[tsObject.action_dim]; 
	    tsObject.action_types = new char[tsObject.action_dim];
	    
	    tmp3 = tmp2[1].split(",");
	    
	    if(tsObject.action_dim == 1){
		tsObject.action_types[0] = tmp3[0].substring(1,tmp3[0].length()-1).trim().charAt(0);        
		if(tsObject.obs_types[0] == 'i')
			discrete++;
		else
			continuous++;
		}
	    else
	    {   
		tmp3[0] = tmp3[0].substring(1,tmp3[0].length()); 
		tmp3[tsObject.action_dim-1] = tmp3[tsObject.action_dim-1].substring(0, tmp3[tsObject.action_dim-1].length()-1);
		
		for(int i = 0; i < tsObject.action_dim; ++i){
		    tsObject.action_types[i] = (tmp3[i].trim()).charAt(0);
			if(tsObject.obs_types[0] == 'i')
				discrete++;
			else
				continuous++;
	    }
	    }
		
	    for(int i = 2; i < tsObject.action_dim + 2; ++i)
	    {
		tmp3 = null;
		tmp3 = tmp2[i].split(",");
		tmp3[0] = tmp3[0].substring(1,tmp3[0].length());
		tmp3[1] = tmp3[1].substring(0, tmp3[1].length()-1);
		tsObject.action_mins[i-2] = Float.parseFloat(tmp3[0]);
		tsObject.action_maxs[i-2] = Float.parseFloat(tmp3[1]);
	    }
	    
		tsObject.num_discrete_action_dims = discrete;
		tsObject.num_continuous_action_dims = continuous;
		discrete =0;
		continuous =0;
	}
	catch (Exception e)
	{
	    System.out.println("\nError: Your Task Specification has incorrect format! \nExiting ... \n");
	    System.exit(0);
	}

	return tsObject;
    }
}
    
