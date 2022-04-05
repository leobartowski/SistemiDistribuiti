import java.io.File;
import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;
import java.util.Locale;
import java.util.Scanner;

public class Server extends UnicastRemoteObject implements TransposedService, ReadService, HandleServices {


    protected Server() throws RemoteException {
        super();
    }

    @Override
    public void registerService(String nomeServizio) throws RemoteException {

        try {
            Naming.rebind("//localhost:8080/" + nomeServizio, this);
            System.out.println("Ho attivato il servizio " + nomeServizio);


        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public int[][] readMatrix(String fileName) throws RemoteException {

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

    @Override
    public int[][] transposedMatrix(String fileName, String serviceForReadingMatrix) throws RemoteException {

        try {
            ReadService readService = (ReadService) Naming.lookup("//localhost:8080/" + serviceForReadingMatrix);

            int[][] matrix = readService.readMatrix(fileName);
            Client.printMatrix(matrix);
            int[][] matrixTransposed = new int[matrix.length][matrix.length];
            for (int i = 0; i < matrix.length; i++) {
                for (int j = 0; j < matrix[0].length; j++) {
                    matrixTransposed[i][j] = matrix[j][i];
                }
            }
            return matrixTransposed;

        } catch (Exception e) {

            e.printStackTrace();
        }
        return new int[0][];
    }

    public static void main(String[] args) {

        try {
            LocateRegistry.createRegistry(8080);
            Server server = new Server();
            Naming.rebind(("//localhost:8080/create_service"), server);
            Naming.rebind(("//localhost:8080/transponed_matrix"), server);

        } catch (Exception e) {
            e.printStackTrace();
        }

    }
}
