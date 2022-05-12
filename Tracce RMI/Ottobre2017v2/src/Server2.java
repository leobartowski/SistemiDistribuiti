import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;
import java.util.Arrays;

public class Server2 extends UnicastRemoteObject implements Server2Interface {

    protected Server2() throws RemoteException {
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
//        Arrays.sort(vector);
        return vector;
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
