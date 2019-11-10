import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;

/**
 * This program is used to extract the results from the evaluation set 11. It counts how many time in a run a node is reported because of malicious anonymizer.
 */
public class ResultExtractor2 {
	private ArrayList<Integer> counterList;

	ResultExtractor2(String fileName) {
		// File reading -> filtering -> console output

		counterList = new ArrayList<Integer>();

		try {
			int nodeReportCounter = 0;

			FileReader input = new FileReader(fileName);

			BufferedReader inputbufferizzato = new BufferedReader(input);
			String linea;
			while ((linea = inputbufferizzato.readLine()) != null) {

				if (linea.contains("Preparing for running configuration")) {
					nodeReportCounter = 0;
				}

				if (linea.contains("Good node reported as evil")) {
					nodeReportCounter++;
				}

				if (linea.contains("Calling finish() at end")) {
					System.out.println(nodeReportCounter);
					counterList.add(nodeReportCounter);
					nodeReportCounter = 0;
				}

			}
			input.close();

		} catch (Exception e) {
			System.out.println("Simulation Results Extractor 2: The input file can not be found or it is corrupted");
			System.exit(0);
		}
	}

	public ArrayList<Integer> getCounterList() {
		return counterList;
	}

}
