import java.rmi.Remote;
import java.rmi.RemoteException;

public interface VoteService extends Remote {

    public boolean makeVote() throws RemoteException;
}
