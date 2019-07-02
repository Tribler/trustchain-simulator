package networkgeneratorconverter;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.StringTokenizer;

public class FileParser {

   private ArrayList<Edge> edges;
    
   public ArrayList<Edge> parseFile(String fileName) throws FileNotFoundException, IOException, FileNotFoundException {
    edges = new ArrayList<>();
        FileReader input = new FileReader(fileName);
        BufferedReader inputbufferizzato = new BufferedReader(input);
        String Linea;
        while ((Linea = inputbufferizzato.readLine()) != null) {
            StringTokenizer st = new StringTokenizer(Linea);
            while (st.hasMoreTokens()) {
                edges.add(new Edge(Integer.parseInt(st.nextToken()), Integer.parseInt(st.nextToken())));
                break;
            }
        }
        input.close();
        return edges;
   } 
}
