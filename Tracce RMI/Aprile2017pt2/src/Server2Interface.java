import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Server2Interface extends Remote {

    int[][] sortMatrixByRow(int[][] matrix) throws RemoteException;
}
