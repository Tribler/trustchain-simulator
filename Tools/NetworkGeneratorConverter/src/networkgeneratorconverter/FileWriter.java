package networkgeneratorconverter;

import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

public class FileWriter {

    public void write(String fileName, ArrayList<Edge> edges) throws FileNotFoundException, UnsupportedEncodingException {

        if (edges.size() > 0) {
            //Find the min and the max
            int minNumber = edges.get(0).a;
            int maxNumber = edges.get(0).a;

            for (Edge edge : edges) {
                if (minNumber > edge.a) {
                    minNumber = edge.a;
                }
                if (maxNumber < edge.a) {
                    maxNumber = edge.a;
                }

                if (minNumber > edge.b) {
                    minNumber = edge.b;
                }
                if (maxNumber < edge.b) {
                    maxNumber = edge.b;
                }
            }

            //Solitaire node detection (Check if is there any node not connected to anyone else)
            boolean isSolitaire = true;
            for (int i = 0; i < maxNumber - minNumber; i++) {
                isSolitaire = true;
                for (int j = 0; j < edges.size(); j++) {
                    if (edges.get(j).a == i + minNumber) {
                        isSolitaire = false;
                        break;
                    }
                    if (edges.get(j).b == i + minNumber) {
                        isSolitaire = false;
                        break;
                    }
                }
                if (isSolitaire == true) {
                    break;                   
                }
            }

            //Print on file
            PrintWriter writer = new PrintWriter("output.txt", "UTF-8");
            if (isSolitaire == false) {
                System.out.println("Number of nodes: " + (maxNumber - minNumber + 1));
                writer.println("Number of nodes: " + (maxNumber - minNumber + 1));
                for (int i = 0; i < edges.size(); i++) {
                    System.out.println("rte[" + (edges.get(i).a - minNumber) + "].port++ <--> C <--> rte[" + (edges.get(i).b - minNumber) + "].port++;");
                    writer.println("rte[" + (edges.get(i).a - minNumber) + "].port++ <--> C <--> rte[" + (edges.get(i).b - minNumber) + "].port++;");
                }
            } else {
                System.out.println("ERROR: There is a solitaire node");
                writer.println("ERROR: There is a solitaire node");
            }
            writer.close();
        }

    }
}
