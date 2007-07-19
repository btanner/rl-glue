public class TaskSpecObject
{
    public float version;			
    public char episodic;			
    public int obs_dim;	
	public int num_discrete_obs_dims;
	public int num_continuous_obs_dims;		
    public char [] obs_types;	    
    public float [] obs_mins;           
    public float [] obs_maxs;			
    public int action_dim;
	public int num_discrete_action_dims;
	public int num_continuous_action_dims;			
    public char [] action_types;		
    public float [] action_mins;		
    public float [] action_maxs;	
}
