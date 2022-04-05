import java.rmi.Remote;
import java.rmi.RemoteException;

public interface ArbitroForServer extends Remote {

    public void miCandido(String timestampServer) throws RemoteException;
}
