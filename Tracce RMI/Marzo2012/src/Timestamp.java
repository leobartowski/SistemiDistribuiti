import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;


public class Timestamp extends UnicastRemoteObject implements TimestampService {

    static int value = 0;

    protected Timestamp() throws RemoteException {
        super();
    }


    @Override
    public int fornisciTimestamp() throws RemoteException {
        Timestamp.value++;
        return Timestamp.value;
    }

    public static void main(String[] args) {
        try {
            ArbitroForServer arbitroForServer = (ArbitroForServer) Naming.lookup("//localhost:8080/arbitro");
            arbitroForServer.miCandido("timestamp1");
            arbitroForServer.miCandido("timestamp2");
            arbitroForServer.miCandido("timestamp3");

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
