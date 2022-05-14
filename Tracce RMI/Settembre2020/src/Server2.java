import java.rmi.*;
import java.rmi.server.*;

public class Server2 extends UnicastRemoteObject implements Server2Interface {
    protected Server2() throws RemoteException {
        super();
    }

    @Override
    public int scalarProduct(int[] vector1, int[] vector2) throws RemoteException {
        if (vector1.length != vector2.length) {
            System.out.println("Errore i vettori devono essere uguali");
            return 0;
        }
        int result = 0;
        for (int i = 0; i < vector1.length; i++) {
            result += vector1[i] * vector2[i];
        }
        return result;
    }

    public static void main(String[] args) {

        try {
            Server2 server2 = new Server2();
            Naming.rebind("//localhost:8080/server_2", server2);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
