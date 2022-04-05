import java.rmi.Naming;

public class Client {

    String name;

    public Client(String name) {
        this.name = name;
    }

    public static void main(String[] args) {

        Client c1 = new Client("C1");
        Client c2 = new Client("C2");

        try {
            VoteService voteService = (VoteService) Naming.lookup("//localhost:8080/server");
            int i = 0;
            while(i < 10) {
                voteService.makeVote();
                i++;
            }

        }  catch (Exception e) {
            e.printStackTrace();
        }
    }
}
