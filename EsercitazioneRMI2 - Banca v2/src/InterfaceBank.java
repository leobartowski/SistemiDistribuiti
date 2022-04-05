import java.rmi.Remote;
import java.rmi.RemoteException;

public interface InterfaceBank extends Remote {

    public float getBalance() throws RemoteException;
    public void addMoney(float value) throws RemoteException;
    public float withdrawMoney(float value) throws RemoteException;
}
