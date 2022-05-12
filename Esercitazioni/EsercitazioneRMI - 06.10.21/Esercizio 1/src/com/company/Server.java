package com.company;

import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;

public class Server extends UnicastRemoteObject implements MyInterfaceRMI {

    protected Server() throws RemoteException {
        super();
    }

    //    @Override
    public long getTime() throws RemoteException {
        try {

            return getCurrentMill();
//            FileWriter myfile = new FileWriter(filename);
//            myfile.write(getCurrentMill());
//            myfile.close();
        } catch (Exception error) {
            System.out.println(error.getMessage());
        }
        return 0;
    }

    public long getCurrentMill() {
        return System.currentTimeMillis();
    }

    public static void main(String[] args) {

        try {
            LocateRegistry.createRegistry(8001);
//            LocateRegistry.createRegistry(8002);
//            LocateRegistry.createRegistry(8003);
            Server server1 = new Server();
            Naming.rebind("//localhost:8001/EsercizioRMI", server1);

            Server server2 = new Server();
            Naming.rebind("//localhost:8001/EsercizioRMI", server2);

            Server server3 = new Server();
            Naming.rebind("//localhost:8001/EsercizioRMI", server3);


            System.out.println("servers ok");
        } catch (Exception error) {
            System.out.println(error.getMessage());
        }
    }


}
