import java.rmi.Naming;

public class Main {

    public static void main(String[] args) {

        int[][] matrix1 = {{3, 5, 1}, {5, 3, 5}, {1, 3, 6}, {5, 3, 5}, {1, 3, 6}};
        int[][] matrix2 = {{3, 5, 1, 4}, {7, 1, 6, 7}, {5, 6, 9, 3}};


        try {

            Server1Interface service = (Server1Interface) Naming.lookup("//localhost:8080/server_1");
            int[][] resultMatrix = service.product(matrix1, matrix2);
            Main.printMatrix(resultMatrix);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void printMatrix(int[][] matrix) {

        for (int i = 0; i < matrix.length; i++) {
            for (int j = 0; j < matrix[i].length; j++) {
                System.out.print(matrix[i][j] + " ");
            }
            System.out.println();
        }
    }
}
