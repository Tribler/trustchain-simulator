import java.io.BufferedReader;
import java.io.FileReader;

public class SimulationsResultExtractor2 {

	/**
	 * This program is used to extract the results from the evaluation set 11.
	 * It counts how many time in a run a node is reported because of malicious anonymizer.
	 */
	public static void main(String[] args) {

		// File reading -> filtering -> console output
				try {
					
					int numeberOfReportsForThisRun = 0;
					
					FileReader input = new FileReader("input.txt");
					//FileReader input = new FileReader(args[0]);

					BufferedReader inputbufferizzato = new BufferedReader(input);
					String linea;
					while ((linea = inputbufferizzato.readLine()) != null) {

						if (linea.contains("Preparing for running configuration")) {
							numeberOfReportsForThisRun=0;
						}
						
						
						if (linea.contains("Good node reported as evil")) {
							numeberOfReportsForThisRun++;
						}
						
						if (linea.contains("Calling finish() at end")) {
							System.out.println(numeberOfReportsForThisRun);
							numeberOfReportsForThisRun = 0;
						}
					
					}
					input.close();

				} catch (Exception e) {
					System.out.println("Simulation Results Extractor 2: The input file can not be found or it is corrupted");
					System.exit(0);
				}
		
	}

}
