import java.rmi.Naming;

public class Client {

    public static void main(String[] args) {


        try {
            ServerInterface server = (ServerInterface) Naming.lookup("//localhost:8080/Server");
            RemoteInt x = new RemoteInt(5);
            RemoteInt y = new RemoteInt(6);

            System.out.println(server.testMethod());

            int firstSum = server.add(x, y); // Mi aspetto 11 come risultato
            System.out.println("Il risultato della prima somma è: " + firstSum);

            // Ricordiamo che durante il metodo add per dimostrare di come cambiano i RemoteInt andiamo a moltiplicarli entrambi per 2
            // Quindi le due somme daranno risultati diversi
            int secondSum = server.add(x, y); // Mi aspetto 22 poiché in add ho moltiplicato sia x che y per 2
            System.out.println("Il risultato della seconda somma è: " + secondSum);

        } catch(Exception exception) {
            exception.printStackTrace();
        }

    }
}
