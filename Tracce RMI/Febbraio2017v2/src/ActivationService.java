import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class ActivationService extends UnicastRemoteObject implements ActivationServiceInterface {

    protected ActivationService() throws RemoteException {
        super();
    }

    public void activateService(String serviceName) throws RemoteException {

        try {
            ReadMatrixService readMatrixService = new ReadMatrixService();
            Naming.rebind("//localhost:8080/" + serviceName, readMatrixService);

        } catch (MalformedURLException e) {
            throw new RuntimeException(e);
        }


    }
}
