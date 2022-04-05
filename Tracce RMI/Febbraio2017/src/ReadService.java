import java.rmi.Remote;
import java.rmi.RemoteException;

public interface ReadService extends Remote {

    public int[][] readMatrix(String fileName) throws RemoteException;
}
