import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;

public class Scheduler extends UnicastRemoteObject implements SchedulerInterface {

    public int counter = 0;

    protected Scheduler() throws RemoteException {
        super();
    }

    @Override
    public String getServiceToUse() throws RemoteException {
        if (counter == 0) {
            counter++;
            return "A";
        } else {
            counter--;
            return "B";
        }
    }

    public static void main(String[] args) {

        try {
            Scheduler scheduler = new Scheduler();
            Naming.rebind("//localhost:8080/scheduler_service", scheduler);
            System.out.println("Scheduler Server ok");

        } catch (Exception error) {
            System.out.println(error.getMessage());
        }

    }
}
