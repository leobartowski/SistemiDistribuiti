import java.rmi.Remote;
import java.rmi.RemoteException;

public interface ServerBInterface extends Remote {

    String readFile(String fileName) throws RemoteException;
}
