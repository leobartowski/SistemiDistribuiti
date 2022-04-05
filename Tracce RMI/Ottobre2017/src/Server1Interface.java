import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Server1Interface extends Remote {

    public int[][] sortMatrix(int [][] matrix) throws RemoteException;
}
