import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;

public class Server1 extends UnicastRemoteObject implements Server1Interface {

    protected Server1() throws RemoteException {
        super();
    }

    @Override
    public int[] sortVector(int[] vector) throws RemoteException {

        for (int i = 0; i < vector.length; i++) {
            for (int j = i + 1; j < vector.length; j++) {

                if (vector[i] > vector[j]) {
                    int temp = vector[j];
                    vector[j] = vector[i];
                    vector[i] = temp;
                }
            }

        }
        return vector;
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
