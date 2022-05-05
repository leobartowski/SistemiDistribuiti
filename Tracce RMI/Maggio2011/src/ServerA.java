
import java.io.*;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class ServerA extends UnicastRemoteObject implements ServerAInterface {

    protected ServerA() throws RemoteException {
        super();
    }

    @Override
    public void writeFile(String fileName, String textToWrite) throws RemoteException {

        try {
            File file = new File(fileName);
            FileWriter fileWriter = new FileWriter(file);
            BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);
            bufferedWriter.write(textToWrite);
            bufferedWriter.close();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }


}
