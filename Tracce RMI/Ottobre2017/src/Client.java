import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;

public class Client {

    public static void printMatrix(int [][] matrix) {

        for (int i = 0; i < matrix.length; i++) {
            for (int j = 0; j < matrix[i].length; j++) {
                System.out.print(matrix[i][j] + " ");
            }
            System.out.println();
        }
    }

    public static void main(String[] args) {

        try {
            Server1Interface service = (Server1Interface) Naming.lookup("//localhost:8080/server_1");
            int[][] matrix = {{5, 6, 3}, {9, 4, 7}, {4, 3, 6}};
            Client.printMatrix(matrix);
            int[][] sortedMatrix = service.sortMatrix(matrix);
            System.out.println("matrice ordinata");
            Client.printMatrix(sortedMatrix);

        } catch (Exception e) {
            e.printStackTrace();
        }


    }
}
