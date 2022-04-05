import java.rmi.Naming;

public class Client {


    public static void printMatrix(int [][] matrix) {

        for (int i = 0; i < matrix.length; i++) {
            for (int j = 0; j < matrix[i].length; j++) {
                System.out.print(matrix[i][j] + " ");
            }
            System.out.println();
        }
    }


    public static void main(String[] args) {

        int[][] matrix = {{1, 2}, {3, 4}};
        Client.printMatrix(matrix);
        try {
            String serviceName = "sort_matrix";
            ServerActivation serverActivation = (ServerActivation) Naming.lookup("//localhost:8080/serverActivation");
            System.out.println("Mi sono connesso al service server Activation!");

            serverActivation.activateRegistry(serviceName);
            Service service = (Service) Naming.lookup("//localhost:8080/" + serviceName);
            System.out.println("Mi sono connesso al service " + serviceName + " !");

            int [][] invertedMatrix = service.inversionColumnMatrix(matrix);
            Client.printMatrix(invertedMatrix);
            serverActivation.deActivateRegistry(serviceName);


        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
