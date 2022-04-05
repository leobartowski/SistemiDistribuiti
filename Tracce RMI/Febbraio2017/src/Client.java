import java.io.File;
import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.util.Scanner;

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
            String serviceReadName = "read_matrix";
            HandleServices service = (HandleServices) Naming.lookup("//localhost:8080/create_service");
            service.registerService(serviceReadName);
            TransposedService transposedService = (TransposedService) Naming.lookup("//localhost:8080/transponed_matrix");
            int[][] output = transposedService.transposedMatrix("myFile.txt", serviceReadName);
            Client.printMatrix(output);


        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
