package networkgeneratorconverter;

import java.util.ArrayList;
import javax.swing.JFrame;
import javax.swing.JOptionPane;

public class NetworkGeneratorConverter {
    public static void main(String[] args) {
        
        FileParser parser = new FileParser();
        FileWriter fileWriter = new FileWriter();
        ArrayList<Edge> edges;

        try {            
            edges = parser.parseFile("input.txt");           
            fileWriter.write("output.txt", edges);
        } catch (Exception e) {
            final JFrame frame = new JFrame();
            JOptionPane.showMessageDialog(frame, "The input file for Gephi can not be found or it is corrupted.");
        } 
    }
}
