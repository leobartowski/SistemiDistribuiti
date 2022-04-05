import java.rmi.Remote;
import java.rmi.RemoteException;

public interface MatrixService extends Remote {

    public int[][] transpose(int[][] matrix) throws RemoteException;
}
