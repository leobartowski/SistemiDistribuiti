import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.ArrayList;

public interface ServerInterface extends Remote {
    
    public int bookExam(String name, String lastName, int courseCode) throws RemoteException;
    public Boolean unbookExam(int registrationId) throws RemoteException;
    public int getNumberOfStudent(int courseCode) throws RemoteException;

    public ArrayList<Student> getStudents(int courseCode) throws RemoteException; // Non parte della prova!
    
}
