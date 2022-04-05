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

            InterfaceServer1 server1 = (InterfaceServer1) Naming.lookup("//localhost:8080/server_1");
            System.out.println("Client connesso al server");
            int [][] matrix = {{13,11,10}, {4,30,11}, {5, 22, 9}};
            Client.printMatrix(matrix);
            System.out.println("Matrice sorted:");
            int [][] matrixSorted = server1.sortMatrix(matrix);
            Client.printMatrix(matrixSorted);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
