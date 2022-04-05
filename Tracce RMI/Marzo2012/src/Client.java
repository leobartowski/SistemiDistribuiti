import java.rmi.Naming;
import java.rmi.RemoteException;
import java.sql.Time;

public class Client {

    public static void main(String[] args) {

        try {

            ArbitroForClient arbitro = (ArbitroForClient) Naming.lookup("//localhost:8080/arbitro");
            String serverTimestamp = arbitro.dammiServer();

            TimestampService timestampService = (TimestampService) Naming.lookup("//localhost:8080/" + serverTimestamp);
            int timestamp = timestampService.fornisciTimestamp();
            System.out.println("Timestamp fornito da " + serverTimestamp + "con valore: " + timestamp);

        } catch (Exception e) {

            e.printStackTrace();
        }

    }
}
