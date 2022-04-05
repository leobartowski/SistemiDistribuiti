import java.rmi.Remote;
import java.rmi.RemoteException;

public interface TransposedService extends Remote {

    public int [][] transposedMatrix(String fileName, String serviceForReadingMatrix) throws RemoteException;
}
