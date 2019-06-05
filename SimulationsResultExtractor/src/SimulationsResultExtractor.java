

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.StringTokenizer;
import javax.swing.JFrame;
import javax.swing.JOptionPane;

public class SimulationsResultExtractor {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws FileNotFoundException, UnsupportedEncodingException {
        ArrayList<Integer> a = new ArrayList<Integer>();
        ArrayList<Integer> b = new ArrayList<Integer>();

        //File reading -> filtering -> output 
        try {
            FileReader input = new FileReader("input.txt");
            PrintWriter writer = new PrintWriter("output.txt", "UTF-8");
            BufferedReader inputbufferizzato = new BufferedReader(input);
            String linea;
            while ((linea = inputbufferizzato.readLine()) != null) {
                if(linea.contains("Preparing") || linea.contains("<!>") && !(linea.contains("<!> Simulation stopped"))){
                    if(linea.contains("Preparing")){
                    System.out.println(" ");
                    writer.println(" ");
                    }
                    System.out.println(linea);
                    writer.println(linea);
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
