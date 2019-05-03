package networks;

import node.Node;
import ned.DatarateChannel;



network Net5
{
    
    types:
        //here it set the connection speed and dealy (eventualy also packet drop)
        channel C extends DatarateChannel
        {
            delay = uniform(0.01ms, 100ms);
            datarate = uniform(300kbps, 1000kbps);
        }
    submodules:
        rte[4]: Node {
            address = index;
        }
    connections:
      
        
        rte[1].port++ <--> C <--> rte[0].port++;
        rte[1].port++ <--> C <--> rte[2].port++;
        rte[0].port++ <--> C <--> rte[2].port++;
        rte[1].port++ <--> C <--> rte[3].port++;
        
              
      //  for i=0..10 {                       
        //	rte[intuniform(1, 3)].port++ <--> C <--> rte[intuniform(1, 3)].port++;			
        //}   
}

