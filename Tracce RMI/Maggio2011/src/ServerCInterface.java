import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.ArrayList;

public interface ServerCInterface extends Remote {

    void activateService(String serverName, String serviceName) throws RemoteException;

    ArrayList<String> serviceList(String serverName) throws RemoteException;


}
