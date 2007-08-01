package rlglue.types;

public class Random_seed_key {
	public int [] intArray;
	public double [] doubleArray;

	public Random_seed_key(int numInts, int numDoubles) {
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
