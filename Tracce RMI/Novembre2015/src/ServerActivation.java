import java.rmi.Remote;
import java.rmi.RemoteException;

public interface ServerActivation extends Remote {

    public void activateRegistry(String serviceName) throws RemoteException;
    public void deActivateRegistry(String serviceName) throws RemoteException;
}
