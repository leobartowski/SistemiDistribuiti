import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;

public class Server extends UnicastRemoteObject implements ServerActivation, Service {

    protected Server() throws RemoteException {
        super();
    }

    @Override
    public void activateRegistry(String serviceName) throws RemoteException {

        try {

            // Questa cosa ha senso ??? Potrei tranquillamente creare un nuova istanza della classe server
            // ma così facendo, dato che il client, chiama l'interfaccia non c'è bisogno
            Naming.rebind(("//localhost:8080/" + serviceName), this);
            System.out.println("Ho attivato il service " + serviceName);


        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void deActivateRegistry(String serviceName) {
        try {

            Naming.unbind(("//localhost:8080/" + serviceName));
            System.out.println("Ho disattivato il service " + serviceName);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public int[][] inversionColumnMatrix(int[][] matrix) throws RemoteException {

        for (int j = 0; j < matrix[0].length; j++) {
            for (int i = 0; i < matrix.length / 2; i++) {

                int temp = matrix[i][j];
                matrix[i][j] = matrix[matrix.length - 1 - i][j];
                matrix[matrix.length - 1 - i][j] = temp;
            }
        }
        return  matrix;
    }

    public static void main(String[] args) {

        try {
            LocateRegistry.createRegistry(8080);
            Server server = new Server();
            Naming.rebind(("//localhost:8080/serverActivation"), server);
            System.out.println("Ho startato il server!");

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
