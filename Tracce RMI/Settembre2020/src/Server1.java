import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.*;

public class Server1 extends UnicastRemoteObject implements Server1Interface {

    public Server1() throws RemoteException {
        super();
    }

    @Override
    public int[][] product(int[][] matrix1, int[][] matrix2) {
        int[][] matrix = new int[matrix1.length][matrix2[0].length];

        for (int i = 0; i < matrix1.length; i++) {
            for (int j = 0; j < matrix2[0].length; j++) {
                try {
                    Server2Interface server2 = (Server2Interface) Naming.lookup("//localhost:8080/server_2");
                    matrix[i][j] = server2.scalarProduct(matrix1[i], this.columnFromMatrix(matrix2, j));
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
        return matrix;
    }

    public int[] columnFromMatrix(int[][] matrix, int columnNumber) {
        int[] column = new int[matrix.length];
        for (int i = 0; i < matrix.length; i++) {
            column[i] = matrix[i][columnNumber];
        }
        return column;
    }

    public static void main(String[] args) {
        try {
            LocateRegistry.createRegistry(8080);
            Server1 server1 = new Server1();
            Naming.rebind("//localhost:8080/server_1", server1);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
