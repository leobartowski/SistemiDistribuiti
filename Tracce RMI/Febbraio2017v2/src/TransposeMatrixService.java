import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class TransposeMatrixService extends UnicastRemoteObject implements TransposeMatrixInterface {


    protected TransposeMatrixService() throws RemoteException {
        super();
    }

    @Override
    public int[][] transposeMatrix(String fileName, String serviceNameToReadMatrix) throws RemoteException {
        int[][] matrix = new int[3][3];

        try {
            ReadMatrixInterface readMatrixService = (ReadMatrixInterface) Naming.lookup("//localhost:8080/" + serviceNameToReadMatrix);
            int[][] fileMatrix = readMatrixService.readMatrix(fileName);

            for (int i = 0; i < fileMatrix.length; i++) {
                for (int j = 0; j < fileMatrix[0].length; j++) {

                    matrix[i][j] = fileMatrix[j][i];
                }
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
        return matrix;
    }
}
