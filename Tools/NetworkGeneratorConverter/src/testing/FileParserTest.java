package testing;

import static org.junit.jupiter.api.Assertions.*;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;

import org.junit.jupiter.api.Test;

import networkgeneratorconverter.Edge;
import networkgeneratorconverter.FileParser;

class FileParserTest {

	@Test
	void test() throws FileNotFoundException, IOException {
		 
		PrintWriter writer = new PrintWriter("test.txt", "UTF-8");
		writer.println("1 2");
		writer.println("2 1");
		writer.close();
		
        FileParser parser = new FileParser();
        ArrayList<Edge> edges  = parser.parseFile("test.txt");           

        File f= new File("test.txt"); //file to be delete
		 f.delete();
		 
		 
		 assertEquals(2, edges.size());
		 assertEquals(1, edges.get(0).a);
		 assertEquals(2, edges.get(0).b);
		 assertEquals(2, edges.get(1).a);
		 assertEquals(1, edges.get(1).b);
	}

}
