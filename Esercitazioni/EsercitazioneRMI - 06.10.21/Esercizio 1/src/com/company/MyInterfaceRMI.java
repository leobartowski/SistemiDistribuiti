package com.company;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface MyInterfaceRMI extends Remote {

    public long getTime() throws RemoteException;
}
