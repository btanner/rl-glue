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
		intArray = new int[numInts];
		doubleArray = new double[numDoubles];
	}
}
