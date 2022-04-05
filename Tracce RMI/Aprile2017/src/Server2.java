import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;
import java.util.Locale;

public class Server2 extends UnicastRemoteObject implements InterfaceServer2 {

    public Server2() throws RemoteException {
        super();
    }

    @Override
    public int[] calculateMinimum(int[] vector) throws RemoteException {
        int[] output = new int[2];

        System.out.println("Ho chiamato getMinumumIndexVector Server 2");
        output[0] = vector[0];
        output[1] = 0;

        for (int i = 0; i < vector.length; i++) {
            if (vector[i] < output[0]) {
                output[1] = i;
                output[0] = vector[i];
            }
        }
        return output;
    }
}
