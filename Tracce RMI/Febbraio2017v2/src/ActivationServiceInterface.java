import java.rmi.Remote;
import java.rmi.RemoteException;

public interface ActivationServiceInterface extends Remote {


    public void activateService(String serviceName) throws RemoteException;
}
