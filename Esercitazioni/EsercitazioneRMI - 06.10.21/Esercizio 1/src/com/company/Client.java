package com.company;

import java.rmi.Naming;

public class Client {

    public static void main(String arg[]) {
        MyInterfaceRMI myserver1;
        MyInterfaceRMI myserver2;
        MyInterfaceRMI myserver3;

        try {
            myserver1 = (MyInterfaceRMI) Naming.lookup("//localhost:8001/EsercizioRMI");
            var time1 = myserver1.getTime();
            System.out.println("Timestamp server1: " + time1);

            myserver2 = (MyInterfaceRMI) Naming.lookup("//localhost:8001/EsercizioRMI");
            var time2 = myserver2.getTime();
            System.out.println("Timestamp server2: " + time2);

            myserver3 = (MyInterfaceRMI) Naming.lookup("//localhost:8001/EsercizioRMI");
            var time3 = myserver3.getTime();
            System.out.println("Timestamp server3: " + time3);

            var mediaTimestamp = (time1 + time2 + time3) / 3;
            System.out.println("Timestamp Media: " + mediaTimestamp);
        } catch (Exception error) {
            System.out.println(error);
        }
    }

}
