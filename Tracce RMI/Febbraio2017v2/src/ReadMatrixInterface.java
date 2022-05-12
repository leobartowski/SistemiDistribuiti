import java.rmi.Remote;
import java.rmi.RemoteException;

public interface ReadMatrixInterface extends Remote {

    public int[][] readMatrix(String fileName) throws RemoteException;

}
