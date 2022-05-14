import java.rmi.*;

public interface Server1Interface extends Remote {

    public int[][] product(int[][] matrix1, int[][] matrix2) throws RemoteException;
}
