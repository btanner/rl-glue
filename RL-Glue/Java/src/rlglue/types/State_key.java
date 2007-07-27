package rlglue.types;

public class State_key {
	public int [] intArray;
	public double [] doubleArray;

	public State_key(int numInts, int numDoubles) {
		intArray = new int[numInts];
		doubleArray = new double[numDoubles];
	}
}
