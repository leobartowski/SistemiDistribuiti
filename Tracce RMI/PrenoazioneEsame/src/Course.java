import java.util.ArrayList;

public class Course {


    public String name;
    public ArrayList<Student> students;
    public int id;

    public Course(String name, int id, ArrayList<Student> students) {
        this.name = name;
        this.id = id;
        this.students = students;
    }


}