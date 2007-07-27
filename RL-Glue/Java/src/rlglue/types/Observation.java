package rlglue.types;

public class Observation 
{
	public int [] intArray;
	public double [] doubleArray;

	public Observation()
	{
	}

	public Observation(int numInts, int numDoubles) 
	{
		intArray = new int[numInts];
		doubleArray = new double[numDoubles];
	}
}
