import java.rmi.Remote;
import java.rmi.RemoteException;

public interface InterfaceAuthBank extends Remote {

    public  void createAccount(Float startingBalance, String password) throws RemoteException;
}
