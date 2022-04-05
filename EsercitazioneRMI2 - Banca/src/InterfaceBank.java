import java.rmi.Remote;
import java.rmi.RemoteException;

public interface InterfaceBank extends Remote {

    public  void createAccount(Float startingBalance, String password) throws RemoteException;
    public float getBalance() throws RemoteException;
    public void addMoney(float value) throws RemoteException;
    public float withdrawMoney(float value) throws RemoteException;
}
