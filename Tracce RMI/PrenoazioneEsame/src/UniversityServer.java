import com.sun.tools.javac.Main;

import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;
import java.util.Optional;

public class UniversityServer extends UnicastRemoteObject implements ServerInterface {

    ArrayList<Course> courses;
    ArrayList<Registration> registrations;

    public UniversityServer() throws RemoteException {
        super();
        this.courses = new ArrayList<>();
        this.registrations = new ArrayList<>();
    }

    @Override
    public int bookExam(String name, String lastName, int courseCode) throws RemoteException {

        Student student = new Student(name, lastName);
        Course myCourse = this.courses.stream().filter(x -> x.id == courseCode).findFirst().get();
        myCourse.students.add(student);
        Registration registration = new Registration(myCourse, student);
        this.registrations.add(registration);
        System.out.println(student.name + " " + student.lastName + " si è prenotato al corso di " + myCourse.name + " con il codice " + registration.id);
        return registration.id;
    }

    @Override
    public Boolean unbookExam(int registrationId) throws RemoteException {

        Optional<Registration> myRegistration = this.registrations.stream().filter(x -> x.id == registrationId).findFirst();
        if (myRegistration.isPresent()) {

            Registration registration = myRegistration.get();
            this.registrations.remove(registration);
            Optional<Course> myCourse = this.courses.stream().filter(x -> x.id == registration.course.id).findFirst();
            if (myCourse.isPresent()) {
                Course course = myCourse.get();
                course.students.remove(registration.student);
                System.out.println(registration.student.name + " " + registration.student.lastName + " si è rimosso dal corso di " + course.name);
                return true;
            }
        }
        return false;
    }

    @Override
    public int getNumberOfStudent(int courseCode) throws RemoteException {

        Optional<Course> myCourse = this.courses.stream().filter( x -> x.id == courseCode).findFirst();
        if (myCourse.isPresent()) {
            Course course = myCourse.get();
            System.out.println("Il numero di studenti per il corso di " + course.name + " è " + course.students.size());
            return course.students.size();
        }

        return 0;
    }

    @Override
    public ArrayList<Student> getStudents(int courseCode) throws RemoteException {
        Optional<Course> myCourse = this.courses.stream().filter( x -> x.id == courseCode).findFirst();
        if (myCourse.isPresent()) {
            Course course = myCourse.get();
            System.out.println("Il numero di studenti per il corso di " + course.name + " è " + course.students.size());
            return course.students;
        }
        return new ArrayList<>();
    }

    public static void main(String[] args) {

        // Credo dei corsi
        Course physics1 = new Course("Fisica 1", 1520, new ArrayList<>());
        Course math1 = new Course("Matematica 1", 1412, new ArrayList<>());
        Course english1 = new Course("Inglese 1", 5520, new ArrayList<>());

        ArrayList<Course> courses = new ArrayList<>();
        courses.add(physics1);
        courses.add(math1);
        courses.add(english1);

        try {
            LocateRegistry.createRegistry(8080);
            UniversityServer university = new UniversityServer();
            university.courses = courses;
            Naming.rebind(("//localhost:8080/university"), university);


        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
