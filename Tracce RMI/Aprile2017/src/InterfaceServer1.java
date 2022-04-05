import java.rmi.Remote;
import java.rmi.RemoteException;

public interface InterfaceServer1 extends Remote {

    public int[][] sortMatrix(int[][] matrix) throws RemoteException;
}
