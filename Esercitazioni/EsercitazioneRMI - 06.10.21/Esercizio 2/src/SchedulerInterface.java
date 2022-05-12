import java.rmi.Remote;
import java.rmi.RemoteException;

public interface SchedulerInterface extends Remote {

    public String getServiceToUse() throws RemoteException;
}
