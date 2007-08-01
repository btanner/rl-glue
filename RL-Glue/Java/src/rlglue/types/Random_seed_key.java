package rlglue.types;

public class Random_seed_key {
	public int [] intArray;
	public double [] doubleArray;

	public Random_seed_key(int numInts, int numDoubles) {
		intArray = new int[numInts];
		doubleArray = new double[numDoubles];
	}
}
