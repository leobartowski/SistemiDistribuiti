import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Server2Interface extends Remote {

    public int scalarProduct(int[] vector1, int[] vector2) throws RemoteException;
}
