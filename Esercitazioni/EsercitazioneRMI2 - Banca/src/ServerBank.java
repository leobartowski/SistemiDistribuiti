import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;

public class ServerBank extends UnicastRemoteObject implements InterfaceBank {

    float balance;

    protected ServerBank(float startingBalance) throws RemoteException {
        super();
        this.balance = startingBalance;
    }

    // Utilizzo la chiamata a questo metodo solo dal service che mi serve per creare un account
    @Override
    public void createAccount(Float startingBalance, String password) throws RemoteException {
        try {
            ServerBank serverClient1 = new ServerBank(startingBalance);
            Naming.rebind(("//localhost:8080/bank_client_" + password), serverClient1);
            System.out.println("New account created!");
        } catch (Exception e) {
            e.printStackTrace();
        }
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

    public static void main(String[] args) {
        try {
            LocateRegistry.createRegistry(8080);

            // Creo questo servizio per aver un servizio di cui conosco l'url a cui chiedere eventualmente se creare o meno
            // un nuovo account con una data password e un dato starting balance
            // TODO: Non so se dovrei creare una nuova classe con una nuova interfaccia per evitare di dare la possibilità al client di poter accedere a tutti i metodi di questa classe
            ServerBank serverForLogin = new ServerBank(0);
            Naming.rebind(("//localhost:8080/bank_handle_login"), serverForLogin);

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
