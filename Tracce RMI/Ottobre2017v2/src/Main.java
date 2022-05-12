import java.rmi.Naming;

public class Main {

    static void printMatrix(int[][] matrix) {
        for (int i = 0; i < matrix.length; i++) {
            for (int j = 0; j < matrix[0].length; j++) {
                System.out.print(matrix[i][j] + " ");
            }
            System.out.println();
        }
    }

    static void printVector(int[] vector) {
        for (int i = 0; i < vector.length; i++) {
            System.out.print(vector[i] + " ");
        }
        System.out.println();
    }

    public static void main(String[] args) {

        int[][] matrix = {{13, 24, 32, 1, 34}, {42, 3, 33, 5, 64}, {2, 31, 74, 34, 13}, {32, 65,93, 15, 85}, {21, 41, 74, 14, 53}};
        int[] vector = { 1, 21, 4, 67, 3, 56};

        try {

            Server1Interface serviceMatrix = (Server1Interface) Naming.lookup("//localhost:8080/server_1");
            int[][] sortedMatrix = serviceMatrix.sortMatrix(matrix);
            Main.printMatrix(sortedMatrix);

        } catch (Exception e) {
            e.printStackTrace();
        }

    }
}
