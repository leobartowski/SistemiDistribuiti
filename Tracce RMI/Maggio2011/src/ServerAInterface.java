import java.rmi.Remote;
import java.rmi.RemoteException;

public interface ServerAInterface extends Remote {

    void writeFile(String fileName, String textToWrite) throws RemoteException;
}
