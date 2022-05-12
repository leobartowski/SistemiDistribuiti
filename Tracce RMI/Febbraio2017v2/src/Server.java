import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;

public class Server {


    public static void main(String[] args) {
        try {


            LocateRegistry.createRegistry(8080);

            ActivationService activationService = new ActivationService();
            Naming.rebind("//localhost:8080/activation_service", activationService);
            TransposeMatrixService transposeMatrixService = new TransposeMatrixService();
            Naming.rebind("//localhost:8080/transpose_matrix_service", transposeMatrixService);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
