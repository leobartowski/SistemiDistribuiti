import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;

public class ServerC extends UnicastRemoteObject implements ServerCInterface {

    ArrayList<String> serviceListServerA = new ArrayList<>();
    ArrayList<String> serviceListServerB = new ArrayList<>();

    protected ServerC() throws RemoteException {
        super();
        this.serviceListServerA.add("//localhost:8080/server_a");
        this.serviceListServerB.add("//localhost:8080/server_b");
    }

    @Override
    public void activateService(String serverName, String serviceName) throws RemoteException {

        if (serverName == "server_a") {
            ServerA server = new ServerA();
            try {
                Naming.rebind("//localhost:8080/" + serviceName, server);
                System.out.println("Ho attivato il server A");

            } catch (Exception e) {
                e.printStackTrace();
            }

        } else if (serverName == "server_b") {
            ServerB server = new ServerB();
            try {
                Naming.rebind("//localhost:8080/" + serviceName, server);

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public ArrayList<String> serviceList(String serverName) throws RemoteException {

        return serverName == "server_a" ? this.serviceListServerA : this.serviceListServerB;
    }

    public static void main(String[] args) {

        try {

            LocateRegistry.createRegistry(8080);
            ServerC server = new ServerC();
            Naming.rebind("//localhost:8080/server_c", server);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
