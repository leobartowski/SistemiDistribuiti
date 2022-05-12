import java.rmi.Naming;

public class Main {

    public static void main(String[] args) {

        try {
            String readMatrixServiceName = "read_matrix";
             ActivationServiceInterface activationService = (ActivationServiceInterface) Naming.lookup("//localhost:8080/activation_service");
            activationService.activateService(readMatrixServiceName);

            TransposeMatrixInterface transposeMatrixService = (TransposeMatrixInterface) Naming.lookup("//localhost:8080/transpose_matrix_service");
            int [][] transposedMatrix = transposeMatrixService.transposeMatrix("matrix.txt", readMatrixServiceName);
            MyUtils.printMatrix(transposedMatrix);

        } catch (Exception e) {
            e.printStackTrace();
        }


    }
}
