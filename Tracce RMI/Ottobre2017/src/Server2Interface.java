import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Server2Interface extends Remote {

    public int[] sortVector(int [] vector) throws RemoteException;
}
