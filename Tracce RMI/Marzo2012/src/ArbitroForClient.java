import java.rmi.Remote;
import java.rmi.RemoteException;

public interface ArbitroForClient extends Remote {

    public String dammiServer() throws RemoteException;

}
