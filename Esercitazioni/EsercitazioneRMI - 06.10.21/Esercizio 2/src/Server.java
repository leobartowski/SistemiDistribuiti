import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;

public class Server extends UnicastRemoteObject implements ServiceInterface {

    public final String name;

    protected Server(String name) throws RemoteException {
        super();
        this.name = name;
    }

    @Override
    public String getServiceInfo() throws RemoteException {
        return this.name;
    }

    public static void main(String[] args) {

        try {
            LocateRegistry.createRegistry(8080);
            Server serverA = new Server("A");
            Naming.rebind("//localhost:8080/server_service_A", serverA);

            Server serverB = new Server("B");
            Naming.rebind("//localhost:8080/server_service_B", serverB);

            System.out.println("Servers A and B ok");

        } catch (Exception error) {
            System.out.println(error.getMessage());
        }

    }
}
