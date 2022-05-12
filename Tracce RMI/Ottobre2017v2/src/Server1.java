import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;

public class Server1 extends UnicastRemoteObject implements Server1Interface {

    protected Server1() throws RemoteException {
        super();
    }

    @Override
    public int[][] sortMatrix(int[][] matrix) throws RemoteException {

        try {
            Server2Interface sortService = (Server2Interface) Naming.lookup("//localhost:8080/server_2");
            for (int i = 0; i < matrix.length; i++) {

                if (i % 2 == 1) { // dispari
                    int[] vectorToSort = sortService.sortVector(matrix[i]);
                    int[] temp = new int [vectorToSort.length];
                    for (int j = 0; j < vectorToSort.length; j++) {
                        temp[j] = vectorToSort[vectorToSort.length - 1 - j];
                    }
                    matrix[i] = temp;
                } else {
                    matrix[i] = sortService.sortVector(matrix[i]);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return matrix;
    }

    public static void main(String[] args) {

        try {
            LocateRegistry.createRegistry(8080);
            Server1 server = new Server1();
            Naming.rebind("//localhost:8080/server_1", server);


        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
