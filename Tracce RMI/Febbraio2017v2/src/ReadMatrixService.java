import java.io.File;
import java.io.FileReader;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.Scanner;

public class ReadMatrixService extends UnicastRemoteObject implements ReadMatrixInterface {

    protected ReadMatrixService() throws RemoteException {
        super();
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
}
