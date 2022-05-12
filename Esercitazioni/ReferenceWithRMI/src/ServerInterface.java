import java.rmi.Remote;
import java.rmi.RemoteException;

public interface ServerInterface  extends Remote {

    public int add(RemoteIntInterface x, RemoteIntInterface y) throws RemoteException;

    public String testMethod() throws RemoteException;

}
