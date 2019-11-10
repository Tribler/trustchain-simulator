import static org.junit.jupiter.api.Assertions.*;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

import org.junit.jupiter.api.*;

class Tester {

	@Test
	void test() throws FileNotFoundException, UnsupportedEncodingException {

		// File f= new File("input.txt"); //file to be delete
		// f.delete();

		PrintWriter writer = new PrintWriter("input.txt", "UTF-8");
		writer.println("Preparing for running configuration ");
		writer.println(" Good node reported as evil ");
		writer.println(" Calling finish() at end ");

		writer.println(" Preparing for running configuration  ");
		writer.println(" Calling finish() at end ");

		writer.println("Preparing for running configuration ");
		writer.println(" Good node reported as evil ");
		writer.println(" Calling finish() at end ");
		writer.close();

		ResultExtractor2 extractor = new ResultExtractor2("input.txt");
		ArrayList<Integer> counterList = extractor.getCounterList();
		assertEquals(3, counterList.size());
		assertEquals(1, counterList.get(0));
		assertEquals(0, counterList.get(1));
	}

	@Test
	void testMore() throws FileNotFoundException, UnsupportedEncodingException {

		assertTrue(true);

		PrintWriter writer = new PrintWriter("input.txt", "UTF-8");
		writer.println("Preparing for running configuration ");
		writer.println(" Good node reported as evil ");
		writer.println(" Calling finish() at end ");

		writer.println(" Preparing for running configuration  ");
		writer.println(" Calling finish() at end ");

		writer.close();

		ResultExtractor2 extractor = new ResultExtractor2("input.txt");

		assertEquals(2, extractor.getCounterList().size());
	}

}
