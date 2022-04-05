import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;

public class Server1 extends UnicastRemoteObject implements InterfaceServer1 {

    InterfaceServer2 server2;

    protected Server1() throws RemoteException {
        super();
        try {
            // Facendo in questo modo facciamo startare prima il server2 e poi il server 1
            // l'alternativa è chiamare quest interfaccia nel metodo che fa il sort della matrice
            LocateRegistry.createRegistry(8090);
            Server2 server2 = new Server2();
            Naming.rebind(("//localhost:8090/server_2"), server2);
            System.out.println("New server2 created!");
            this.server2 = (InterfaceServer2) Naming.lookup("//localhost:8090/server_2");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    // NON HO CAPITO COME FUNZIONA, NE TANTOMENTO COSA SIA LA TECNICA DEI MINIMI SUCCESSIVI
    @Override
    public int[][] sortMatrix(int[][] mat) throws RemoteException {

        int cols = mat[0].length;

        for (int i = 0; i < mat.length; i++) {
            int[] vet = new int[cols];
            for (int j = 0; j < cols; j++) {
                vet[j] = mat[i][j];
            }

            int[] res = new int[2];
            res = server2.calculateMinimum(vet);

            int temp;
            temp = mat[i][cols - 1];
            mat[i][cols - 1] = res[0];
            mat[i][res[1]] = temp;

            for (int z = 0; z < vet.length - 2; z++) {

                int indice_max = z;
                for (int k = z + 1; k < vet.length - 1; k++) {

                    if (mat[i][k] > mat[i][indice_max])
                        indice_max = k;
                }

                temp = mat[i][z];
                mat[i][z] = mat[i][indice_max];
                mat[i][indice_max] = temp;
            }
        }
        return mat;

    }

    public static void main(String[] args) {
        try {

            LocateRegistry.createRegistry(8080);
            Server1 server1 = new Server1();
            Naming.rebind(("//localhost:8080/server_1"), server1);
            System.out.println("New server1 created!");

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
