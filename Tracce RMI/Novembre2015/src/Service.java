import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Service extends Remote {

    public int [][] inversionColumnMatrix(int [][] matrix) throws RemoteException;
}
