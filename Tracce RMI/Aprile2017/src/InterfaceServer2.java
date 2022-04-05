import java.rmi.Remote;
import java.rmi.RemoteException;

public interface InterfaceServer2 extends Remote {

    public int[] calculateMinimum(int [] vector) throws RemoteException;

}
