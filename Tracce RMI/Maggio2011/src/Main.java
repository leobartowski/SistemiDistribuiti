import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.util.ArrayList;

public class Main {

    public static void main(String[] args) {

        try {
            ServerCInterface serviceC = (ServerCInterface) Naming.lookup("//localhost:8080/server_c");
            serviceC.activateService("server_a", "server_a");

            ArrayList<String> list = serviceC.serviceList("server_a");
            String service = list.stream().findFirst().toString(); // force unwrap not safe

            //! LA PARTE DI ATTIVAZIONE DEL SERVIZIO NON FUNZIONA, CHIEDERE AL PROF!
//            System.out.println(service);
//
//            ServerAInterface serverA = (ServerAInterface) Naming.lookup(service);
//            serverA.writeFile("file.txt", "ciao ciao");

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
