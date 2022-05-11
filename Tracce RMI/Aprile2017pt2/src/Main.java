import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;

public class Main {

    public static void main(String[] args) {

        int[][] matrix = new int[][]{{41, 65, 33}, {46, 5, 14}, {64, 4, 1}};

        try {
            Server2Interface service = (Server2Interface) Naming.lookup("//localhost:8080/server_2");
            int[][] sortedMatrix = service.sortMatrixByRow(matrix);
            MyUtils.printMatrix(sortedMatrix);


        } catch (Exception e) {
            e.printStackTrace();
        }


    }
}
