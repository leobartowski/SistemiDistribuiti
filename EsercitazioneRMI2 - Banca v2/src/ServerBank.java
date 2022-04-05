import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;

public class ServerBank extends UnicastRemoteObject implements InterfaceBank, InterfaceAuthBank {

    float balance;

    protected ServerBank(float startingBalance) throws RemoteException {
        super();
        this.balance = startingBalance;
    }

    @Override
    public float getBalance() throws RemoteException {
        return this.balance;
    }

    @Override
    public void addMoney(float value) throws RemoteException {
        this.balance += value;
    }

    @Override
    public float withdrawMoney(float value) throws RemoteException {
        if (value > this.balance) {
            return -1;
        }
        this.balance -= value;
        return this.balance;
    }

    @Override
    public void createAccount(Float startingBalance, String password) throws RemoteException {
        try {
            ServerBank serverClient = new ServerBank(startingBalance);
            Naming.rebind(("//localhost:8080/bank_client_" + password), serverClient);
            System.out.println("New account created with starting balance: " + startingBalance + " and password: " + password);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        try {
            LocateRegistry.createRegistry(8080);

            // Creo questo servizio per aver un servizio di cui conosco l'url a cui chiedere eventualmente se creare o meno
            // un nuovo account con una data password e un dato starting balance
            ServerBank serverAuth = new ServerBank(0);
            Naming.rebind(("//localhost:8080/bank_handle_login"), serverAuth);

            ServerBank serverClient1 = new ServerBank(100);
            Naming.rebind(("//localhost:8080/bank_client_password1"), serverClient1);
            ServerBank serverClient2 = new ServerBank(200);
            Naming.rebind(("//localhost:8080/bank_client_password2"), serverClient2);
            ServerBank serverClient3 = new ServerBank(500);
            Naming.rebind(("//localhost:8080/bank_client_password3"), serverClient2);
            System.out.println("Bank Server running!!!");

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
