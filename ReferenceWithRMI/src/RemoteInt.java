import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class RemoteInt extends UnicastRemoteObject implements RemoteIntInterface {

    public int value;

    protected RemoteInt(int value) throws RemoteException {
        super();
        this.value = value;
    }

    @Override
    public int get() throws RemoteException {
        return value;
    }

    @Override
    public void set(int x) throws RemoteException {
        this.value = x;
    }
}
