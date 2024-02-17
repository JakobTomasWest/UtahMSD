package msd.benjones;

import java.sql.SQLOutput;

public class Main {

    public static void main(String[] args) throws InterruptedException {
        for ( int i = 5; i < 1001; i = i +10) {
//            System.out.println("Starting iteration for size: " + i);
                Network.makeProbablisticNetwork(i);
                Network.dump();
                Network.startup();
                Network.runBellmanFord();
//
            //System.out.println("done building tables!");
            for (Router r : Network.getRouters()) {
                r.dumpDistanceTable();
            }
            System.out.println( Network.getMessageCount() );

            Network.reset();
        }

    }
}
