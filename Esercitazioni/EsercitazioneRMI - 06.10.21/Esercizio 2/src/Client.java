import java.rmi.Naming;
import java.util.concurrent.TimeUnit;

public class Client {

    public String name;

    Client(String name) {
        this.name = name;
    }

    private void startClient() {
        try {

            SchedulerInterface scheduler = (SchedulerInterface) Naming.lookup("//localhost:8080/scheduler_service");
            final String serviceToUse = scheduler.getServiceToUse();

            ServiceInterface server = (ServiceInterface) Naming.lookup("//localhost:8080/server_service_" + serviceToUse);
            final String serviceInfo = server.getServiceInfo();
            System.out.println("Sono il " + this.name + " ho utilizzato il service " + serviceInfo);

        } catch (Exception error) {
            System.out.println(error.getMessage());
        }
    }

    public static void main(String[] args) {

        try {
            Client client1 = new Client("Client 1");
            client1.startClient();

            TimeUnit.SECONDS.sleep(1);

            Client client2 = new Client("Client 2");
            client2.startClient();
        } catch(Exception error) {
            System.out.println(error.getMessage());
        }
    }

}
