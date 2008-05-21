import java.io.*;
import java.lang.*;
import java.util.regex.*;

/**
* @deprecated
*/
public class TaskSpecParser
{        
    public static TaskSpecObject parse(String ts)
    {
				return new TaskSpecObject(ts);
    }
}
    
