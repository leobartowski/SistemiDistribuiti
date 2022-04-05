import java.io.Serializable;

public class Student implements Serializable {

    public String name;
    public String lastName;

    public Student(String name, String lastName) {
        this.name = name;
        this.lastName = lastName;
    }


}
