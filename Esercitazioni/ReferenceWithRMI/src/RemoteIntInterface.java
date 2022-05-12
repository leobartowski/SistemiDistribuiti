import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RemoteIntInterface extends Remote {

    public int get() throws RemoteException;
    public void set(int x) throws RemoteException;
}
