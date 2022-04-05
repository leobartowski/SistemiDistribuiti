import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;

public class Server extends UnicastRemoteObject implements ServerInterface {

    protected Server() throws RemoteException {
        super();
    }

    @Override
    public int add(RemoteIntInterface x, RemoteIntInterface y) throws RemoteException {

        int sum = x.get() + y.get();

        System.out.println("Il valore originale di x: " + x.get());
        System.out.println("Il valore originale di y: " + y.get());
        System.out.println("La somma originale è:" + sum);

        // Andiamo a moltiplicare sia x che y per 2 in modo da dimostrare che possiamo cambiare il valore originale
        // come se fosse un passaggio per riferimento
        x.set(x.get() * 2);
        y.set(y.get() * 2);
        System.out.println("Il nuovo valore di x è:" + x.get());
        return sum ;
    }

    @Override
    public String testMethod() {
        return "Metodo di prova ok!";
    }

    public static void main(String[] args) {

        try {
            LocateRegistry.createRegistry(8080);
            Server server = new Server();
            Naming.rebind("//localhost:8080/Server", server);
            System.out.println("Server ok");

        } catch(Exception exception) {
            System.out.println(exception.getMessage());
        }

    }
}



