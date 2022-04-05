import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.util.ArrayList;

public class Client {

    public static void main(String[] args) {

        try {
            ServerInterface service = (ServerInterface) Naming.lookup("//localhost:8080/university");
            int registrationId = service.bookExam("Francesco", "Ciro", 1520);
            int registrationId2 = service.bookExam("Marco", "Cugino", 1520);
            int registrationId3 = service.bookExam("Maria", "Geppetto", 1520);

            service.unbookExam(registrationId);
            service.getNumberOfStudent(1520);

            ArrayList<Student> students = service.getStudents(1520);
            for (Student student: students) {
                System.out.println("Sono lo studente " + student.name + " " + student.lastName);
            }

        } catch (Exception e) {
            e.printStackTrace();
        }


    }
}
