import java.rmi.Remote;
import java.rmi.RemoteException;

public interface HandleServices extends Remote {

    public void registerService(String nomeServizio) throws RemoteException;
}
