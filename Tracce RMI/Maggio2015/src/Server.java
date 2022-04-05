import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;
import java.util.Random;

public class Server extends UnicastRemoteObject implements MatrixService, VoteService {

    public Server() throws RemoteException {
        super();
    }

    @Override
    public int[][] transpose(int[][] matrix) throws RemoteException {

        int[][] result = new int[matrix.length][matrix.length];
        for (int i = 0; i < matrix.length; i++) {
            for (int j = 0; j < matrix.length; j++) {
                result[i][j] = matrix[j][i];
            }
        }
        return result;
    }

    @Override
    public boolean makeVote() throws RemoteException {
        Random random = new Random();
        return random.nextBoolean();
    }

    public static void main(String[] args) {

        try {

            LocateRegistry.createRegistry(8080);
            Server server = new Server();
            Naming.rebind(("//localhost:8080/server"), server);

        } catch (Exception e) {

            e.printStackTrace();
        }

    }
}
