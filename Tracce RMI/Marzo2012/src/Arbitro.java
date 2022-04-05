import java.lang.reflect.Array;
import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;
import java.util.Random;

public class Arbitro extends UnicastRemoteObject implements ArbitroForClient, ArbitroForServer {

    static ArrayList<String> candidati = new ArrayList<String>();

    protected Arbitro() throws RemoteException {
        super();
    }

    @Override
    public void miCandido(String serverTimestamp) throws RemoteException {
        Arbitro.candidati.add(serverTimestamp);
    }

    @Override
    public String dammiServer() throws RemoteException {

        int size = Arbitro.candidati.size(); // Presuppone che ci siano state già le candidature altrimenti è vuoto!
        Random random = new Random();
        int randomNumber = random.nextInt(0, size - 1);
        String servizioScelto = Arbitro.candidati.get(randomNumber);
        Arbitro.candidati.clear(); // Puliamo la lista in caso ci debbano essere altre candidature!
        Timestamp timestamp = new Timestamp();
        System.out.println();
        try {
            Naming.rebind(("//localhost:8080/" + servizioScelto), timestamp);

        } catch (Exception e) {
            e.printStackTrace();
        }
        return servizioScelto;
    }

    public static void main(String[] args) {

        try {

            LocateRegistry.createRegistry(8080);
            Arbitro arbitro = new Arbitro();
            Naming.rebind(("//localhost:8080/arbitro"), arbitro);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
