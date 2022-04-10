import java.io.File;
import java.rmi.Naming;
import java.util.Scanner;

public class Client {

    String name;
    MatrixService matrixService;
    static int numberOfHeadsVote = 0;

    public Client(String name) {
        this.name = name;
    }

    static void printMatrix(int[][] matrix) {
        for (int i = 0; i < matrix.length; i++) {
            for (int j = 0; j < matrix[0].length; j++) {
                System.out.print(matrix[i][j] + " ");
            }
            System.out.println("");
        }
    }

    public void voteXTimes(int x) {
        try {
            VoteService voteService = (VoteService) Naming.lookup("//localhost:8080/server");
            int i = 0;
            while(i < x) {
                boolean isHead = voteService.makeVote();
                if (isHead) { Client.numberOfHeadsVote++; };
                i++;
            }
        }  catch (Exception e) {
            e.printStackTrace();
        }
    }

    public int[][] readMatrixFromFile(String fileName) {

        int[][] matrix = new int[3][3]; // Improve!
        try {
            File file = new File(fileName);
            Scanner scanner = new Scanner(file);

            while (scanner.hasNextLine()) {
                for (int i = 0; i < matrix.length; i++) {
                    String[] line = scanner.nextLine().trim().split(" ");
                    for (int j = 0; j < line.length; j++) {
                        matrix[i][j] = Integer.parseInt(line[j]);
                    }
                }
            }
            scanner.close();
        } catch (Exception e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
        return matrix;
    }

    public void activateMatrixService() {

        try {
            matrixService = (MatrixService) Naming.lookup("//localhost:8080/server");

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {

        Client c1 = new Client("C1");
        Client c2 = new Client("C2");

        c1.voteXTimes(10);
        c2.voteXTimes(10);
        boolean isC1Winner = Client.numberOfHeadsVote >= 10;
        System.out.println("Ha vinto " + (isC1Winner ? "C1" : "C2"));

        if (isC1Winner) {

            c2.activateMatrixService();
            int[][] matrix = c2.readMatrixFromFile("matrix.txt");
            try {
                int[][] transposedMatrix = c2.matrixService.transpose(matrix);
                Client.printMatrix(transposedMatrix);
            } catch (Exception e) {
                e.printStackTrace();
            }
        } else {

            c1.activateMatrixService();
            int[][] matrix = c1.readMatrixFromFile("matrix.txt");
            try {
                int[][] transposedMatrix = c1.matrixService.transpose(matrix);
                Client.printMatrix(transposedMatrix);

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
