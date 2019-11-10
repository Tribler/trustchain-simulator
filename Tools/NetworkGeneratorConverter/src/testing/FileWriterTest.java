package testing;

import static org.junit.jupiter.api.Assertions.*;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

import org.junit.jupiter.api.Test;

import networkgeneratorconverter.Edge;
import networkgeneratorconverter.FileWriter;

class FileWriterTest {

	@Test
	void test() throws FileNotFoundException, UnsupportedEncodingException {

		FileWriter fileWriter = new FileWriter();

		ArrayList<Edge> edges = new ArrayList<Edge>();
		edges.add(new Edge(0, 1));
		edges.add(new Edge(1, 2));

		fileWriter.write("test.txt", edges);

		File f = new File("test.txt"); // file to be delete
		f.delete();
		
		assertTrue(true);
	}
	
	@Test
	void testFail() throws FileNotFoundException, UnsupportedEncodingException {

		FileWriter fileWriter = new FileWriter();

		ArrayList<Edge> edges = new ArrayList<Edge>();
		edges.add(new Edge(0, 1));
		edges.add(new Edge(1, 2));
		edges.add(new Edge(3, 2));

		edges.add(new Edge(5, 8));


		fileWriter.write("test.txt", edges);

		File f = new File("test.txt"); // file to be delete
		f.delete();
		
		assertTrue(true);
	}

}
