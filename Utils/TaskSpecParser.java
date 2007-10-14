import java.io.*;
import java.lang.*;
import java.util.regex.*;

public class TaskSpecParser
{        
    public static TaskSpecObject parse(String ts)
    {
				return new TaskSpecObject(ts);
    }
}
    
