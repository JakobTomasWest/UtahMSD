package msd.benjones;

import java.util.HashMap;
import java.util.Set;

public class Router {

    private HashMap<Router, Integer> distances;
    private String name;
    public Router(String name) {
        this.distances = new HashMap<>();
        this.name = name;
    }

    public void onInit() throws InterruptedException {

		//TODO: IMPLEMENT ME
		//As soon as the network is online,
		//fill in your initial distance table and broadcast it to your neighbors
        //my distance is always 0
        this.distances.put(this, 0);
        //get all teh networks routers
        Set<Router> allRoutersGotten = Network.getRouters();
        //initialize distances to neighbors
        for (Router r : allRoutersGotten ){
            distances.put(r,Integer.MAX_VALUE); //initalize with max value so smaller values will be properly created
        }
        for (Neighbor neighbor : Network.getNeighbors(this)) {
            this.distances.put(neighbor.router, neighbor.cost);
        }
        // Broadcast initial distances to neighbors
        for (Neighbor neighbor : Network.getNeighbors(this)) {
            Message message = new Message(this, neighbor.router, distances);
            //add the message into the queue
            Network.sendDistanceMessage(message);
        }
    }

    public void onDistanceMessage(Message message) throws InterruptedException {
		//update your distance table and broadcast it to your neighbors if it changed
        boolean isUpdated = false;
        // if the new distance is less than the original distance
        // change the value and report an update occurred
        for(Router router: message.distances.keySet()){
            // calculate the new potential distance to each router reported in the message.
            // this is the sum of the distance from this router to the message's sender
            // and the sender's reported distance to the target router.
            int distance = message.distances.get(router) + distances.get(message.sender);
            //check if the new distance is less than the currently knowned distance
            if (distance > 0 && distance< distances.get(router)){
                //update each distance from receiver routers and senders neighbors in distance table
                distances.put(router,distance);
                isUpdated = true;
            }
        }
        //if distance table is updated, broadcast it to my neighbors (queue is empty)
        if(isUpdated) {
            for (Neighbor neighbor : Network.getNeighbors(this)) {
                Network.sendDistanceMessage(new Message(this, neighbor.router, distances));
            }
        }
    }


    public void dumpDistanceTable() {
        //System.out.println("router: " + this);
        for(Router r : distances.keySet()){
           // System.out.println("\t" + r + "\t" + distances.get(r));
        }
    }

//    @Override
//    public String toString(){
//        return "Router: " + name;
//    }
}
