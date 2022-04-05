import java.util.Random;

public class Registration {

    int id;
    Course course;
    Student student;

    public Registration(Course course, Student student) {
        this.course = course;
        this.student = student;
        this.id = getRegistrationId();
    }

    public int getRegistrationId() {
        Random random = new Random();
        return random.nextInt(0,99999999);
    }

}
