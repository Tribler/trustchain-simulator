

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.StringTokenizer;

public class SimulationsResultExtractor {

    /**
     * This program is used to filter out informations out of OMNeT++ tests log file
     * There are two outputs
     * - Simulations related informations on standard output
     * - Delta detection times
     */
    
    public static void main(String[] args) throws FileNotFoundException, UnsupportedEncodingException {

        //File reading -> filtering -> output 
        try {
            FileReader input = new FileReader(args[0]);
            PrintWriter writer = new PrintWriter("output.txt", "UTF-8");
            BufferedReader inputbufferizzato = new BufferedReader(input);
            String linea;
            while ((linea = inputbufferizzato.readLine()) != null) {
                if(linea.contains("Preparing") || linea.contains("<!>") && !(linea.contains("<!> Simulation stopped"))){
                    if(linea.contains("Preparing")){
                    System.out.println(" ");
                    }
                    System.out.println(linea);
                }
                        
                if(linea.contains("delta detection time")){
                    StringTokenizer detenctionTime = new StringTokenizer(linea, ":");
                    detenctionTime.nextElement();
                    detenctionTime.nextElement();                                               
                    writer.println(((String)(detenctionTime.nextElement())).trim());
                }
            }
            input.close();
            writer.close();
            
        } catch (Exception e) {
            System.out.println("Simulation Results Extractor: The input file can not be found or it is corrupted");
            System.exit(0);
        }
    }
}
