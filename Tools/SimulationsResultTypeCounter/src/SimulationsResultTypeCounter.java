
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.StringTokenizer;

public class SimulationsResultTypeCounter {

	/**
	 * This program is used to filter out informations out of OMNeT++ tests log file
	 * The console output of this program return the number of detection caused by detection in chain verification and in dissemination 
	 */

	public static void main(String[] args) throws FileNotFoundException, UnsupportedEncodingException {

		// File reading -> filtering -> console output
		try {
			int disseminationDetection = 0, chainDissemination = 0;

			// FileReader input = new FileReader(args[0]);
			FileReader input = new FileReader("input.txt");
			BufferedReader inputbufferizzato = new BufferedReader(input);
			String linea;
			while ((linea = inputbufferizzato.readLine()) != null) {
				if (linea.contains("detected double spending during dissemination")) {
					disseminationDetection++;
				} else if (linea.contains("detected double spending during chain")) {
					chainDissemination++;
				}
			}
			input.close();
			System.out.println(chainDissemination);
			System.out.println(disseminationDetection);

		} catch (Exception e) {
			System.out.println("Simulation Results Extractor: The input file can not be found or it is corrupted");
			System.exit(0);
		}
	}
}
