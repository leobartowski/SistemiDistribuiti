import java.rmi.Remote;
import java.rmi.RemoteException;

public interface TimestampService extends Remote {

public int fornisciTimestamp() throws RemoteException;

}
