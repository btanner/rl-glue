package RLGlue;

public class StateKey {
    public int [] intArray;
    public double [] doubleArray;

    public StateKey(int numInts, int numDoubles) {
	intArray = new int[numInts];
	doubleArray = new double[numDoubles];
    }
}
