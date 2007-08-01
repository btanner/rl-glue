package rlglue.types;

public class Action 
{
	public int [] intArray;
	public double [] doubleArray;

	public Action() 
	{
	}

	public Action(int numInts, int numDoubles)
	{
		if (numInts > 0)
			intArray = new int[numInts];
		else
			intArray = null;
		
		if (numDoubles > 0)
			doubleArray = new double[numDoubles];
		else
			doubleArray = null;
	}
}
