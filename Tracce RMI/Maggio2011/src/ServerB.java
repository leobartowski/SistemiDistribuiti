import java.rmi.RemoteException;
import java.io.*;
import java.rmi.server.UnicastRemoteObject;

public class ServerB extends UnicastRemoteObject implements ServerBInterface {

    protected ServerB() throws RemoteException {
        super();
    }


    @Override
    public String readFile(String fileName) throws RemoteException {

        try {
            String fileText = "";
            File file = new File(fileName);
            FileReader fileReader = new FileReader(file);
            BufferedReader bufferedReader = new BufferedReader(fileReader);
            fileText = bufferedReader.readLine();

        } catch (Exception e) {
            e.printStackTrace();
        }


        return "";
    }
}
