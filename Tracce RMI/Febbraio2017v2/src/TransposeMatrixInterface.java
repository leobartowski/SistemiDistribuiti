import java.rmi.Remote;
import java.rmi.RemoteException;

public interface TransposeMatrixInterface extends Remote {

    public int [][] transposeMatrix(String fileName, String serviceNameToReadMatrix) throws RemoteException;
}
