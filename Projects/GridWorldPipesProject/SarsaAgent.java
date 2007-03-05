import java.util.*;
 
public class SarsaAgent 
{

    private int A, S;
    private int [] actions;
    private double [][] Q;
    private double alpha = .1, epsilon = .1;
    private int m;
    private int n;
    Java_agent_parser jap = new Java_agent_parser();

    public void agent_init(String ts)
    {
        jap.parse(ts);
        m = (int)jap.action_maxs[0] +1;
        n = (int)jap.obs_maxs[0] +1;
    
        actions =  new int [m];
        Q = new double[n][m];
        
        for(int i =0;i< m; i++)
            actions[i] = i;
            
        for( int i = 0; i < n; i++)
            for(int j =0; j < m; j++)
                Q[i][j] = 0; 
                
    }            
    
    public int agent_start(int obs)
    {
        S = obs;
        A = egreedy(S);
        
        return actions[A];     
    }

    
    public int agent_step(double r, int obs)
    {
        double delta;
        int iNew, iOld, a, s;
            
        s = obs;
        a = egreedy(s);
    
        delta = r + Q[s][a] - Q[S][A];
        Q[S][A] = Q[S][A] + alpha*delta;
        A = a;
        S = s;
    
        return actions[a];
    }
    
    public void agent_end(double r)
    {
        int i;
        double error;
        error = r - Q[S][A];
        Q[S][A] = Q[S][A] + alpha*error;
    }    
    
	public void agent_cleanup()
	{}
	
    public int egreedy(int s)
    {
        double ran = Math.random();
        if( ran > epsilon ) 
        {
            return argmax(s);
        }
        else
        {
            return (int)(Math.random()*m);
        }
    }
    
    public int argmax(int s)
    {
        //find the index of the maximal action, break ties randomly
        int bestIndex;
        double bestValue, value;
        int numBests = 1;   
          
        bestIndex = 0;
        bestValue = Q[s][0];	
    
        for(int i = 1; i < m; i++)
        {
                value = Q[s][i];
                if( value >= bestValue )
                {
                    bestValue = value;
                    bestIndex = i;
                    numBests = 1;
                } 
        }
        return bestIndex;        
    }
}