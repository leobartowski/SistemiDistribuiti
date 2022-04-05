import java.rmi.Remote;
import java.rmi.RemoteException;

public interface ServiceInterface extends Remote {

    public String getServiceInfo() throws RemoteException;
}
