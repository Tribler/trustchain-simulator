/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package networkgeneratorconverter;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.StringTokenizer;
import javax.swing.JFrame;
import javax.swing.JOptionPane;

public class NetworkGeneratorConverter {

    public static void main(String[] args) throws FileNotFoundException, UnsupportedEncodingException {

        ArrayList<Integer> a = new ArrayList<Integer>();
        ArrayList<Integer> b = new ArrayList<Integer>();

        //File reading
        try {
            FileReader input = new FileReader("input.txt");
            BufferedReader inputbufferizzato = new BufferedReader(input);
            String Linea, one;
            while ((Linea = inputbufferizzato.readLine()) != null) {
                System.out.println(Linea);
                StringTokenizer st = new StringTokenizer(Linea);
                while (st.hasMoreTokens()) {
                    a.add(Integer.parseInt(st.nextToken()));
                    b.add(Integer.parseInt(st.nextToken()));
                    break;
                }
            }
            input.close();
        } catch (Exception e) {
            final JFrame frame = new JFrame();
            JOptionPane.showMessageDialog(frame, "The input file can not be found or it is corrupted.");
            System.exit(0);
        }

        if (a.size() > 0) {
            //Find the min and the max
            int minNumber = a.get(0);
            int maxNumber = a.get(0);
            for (int number : a) {
                if (minNumber > number) {
                    minNumber = number;
                }
                if (maxNumber < number) {
                    maxNumber = number;
                }
            }
            for (int number : b) {
                if (minNumber > number) {
                    minNumber = number;
                }
                if (maxNumber < number) {
                    maxNumber = number;
                }
            }

            //Solitaire node detection (Check if is there any node not connected to anyone else)
            boolean isSolitaire = true;
            for (int i = 0; i < maxNumber - minNumber; i++) {
                isSolitaire = true;
                for (int j = 0; j < a.size(); j++) {
                    if (a.get(j) == i + minNumber) {
                        isSolitaire = false;
                        break;
                    }
                    if (b.get(j) == i + minNumber) {
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
                for (int i = 0; i < a.size(); i++) {
                    System.out.println("rte[" + (a.get(i) - minNumber) + "].port++ <--> C <--> rte[" + (b.get(i) - minNumber) + "].port++;");
                    writer.println("rte[" + (a.get(i) - minNumber) + "].port++ <--> C <--> rte[" + (b.get(i) - minNumber) + "].port++;");
                }
            } else {
                System.out.println("ERROR: There is a solitaire node");
                writer.println("ERROR: There is a solitaire node");
            }
            writer.close();
        }
    }

}
