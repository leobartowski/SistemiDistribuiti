import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.*;

public class Server2 extends UnicastRemoteObject implements Server2Interface {

    protected Server2() throws RemoteException {
        super();
    }

    @Override
    public int[][] sortMatrixByRow(int[][] matrix) throws RemoteException {

        try {
            Server1Interface service1 = (Server1Interface) Naming.lookup("//localhost:8080/server_1");

            for (int i = 0; i < matrix.length; i++) {

                matrix[i] = service1.sortVector(matrix[i]);
            }

        } catch (Exception e) {
            e.printStackTrace();
        }

        return matrix;
    }

    public static void main(String[] args) {

        try {
            Server2 server = new Server2();
            Naming.rebind("//localhost:8080/server_2", server);


        } catch (Exception e) {
            e.printStackTrace();
        }


    }
}
