import java.lang.reflect.Array;
import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;
import java.util.Arrays;

public class Server1 extends UnicastRemoteObject implements Server1Interface {

    public Server1() throws RemoteException {
        super();
    }

    @Override
    public int[][] sortMatrix(int[][] matrix) throws RemoteException {

        Server2Interface server2 = null;
        try {
            server2 = (Server2Interface) Naming.lookup("//localhost:8080/server_2");
            for (int i = 0; i < matrix.length; i++) {

                if (i % 2 == 0) { // Riga pari
                    matrix[i] = server2.sortVector(matrix[i]);
                } else { // Riga dispari

                    int[] vectorToSort = server2.sortVector(matrix[i]);

                    int[] temp = new int [vectorToSort.length];

                    // Inverto il sorted da crescente a decrescente
                    for (int j = 0; j < vectorToSort.length; j++) {
                        temp[j] = vectorToSort[vectorToSort.length - 1 - j];
                    }
                    matrix[i] = temp;
                }
            }
        } catch (Exception e) {

        }


        return matrix;
    }

    public static void main(String[] args) {
        try {
            LocateRegistry.createRegistry(8080);

            Server1 server = new Server1();
            Naming.rebind(("//localhost:8080/server_1"), server);

        } catch (Exception e) {
            e.printStackTrace();
        }

    }
}
