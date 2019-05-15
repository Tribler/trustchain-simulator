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
        rte[10]: Node {
            address = index;
        }
    connections:
      
        
//        rte[1].port++ <--> C <--> rte[0].port++;
//        rte[1].port++ <--> C <--> rte[2].port++;
//        rte[0].port++ <--> C <--> rte[2].port++;
//        rte[1].port++ <--> C <--> rte[3].port++;
        
    rte[0].port++ <--> C <--> rte[3].port++;
rte[0].port++ <--> C <--> rte[4].port++;
rte[0].port++ <--> C <--> rte[6].port++;
rte[0].port++ <--> C <--> rte[8].port++;
rte[1].port++ <--> C <--> rte[3].port++;
rte[1].port++ <--> C <--> rte[5].port++;
rte[1].port++ <--> C <--> rte[6].port++;
rte[1].port++ <--> C <--> rte[7].port++;
rte[1].port++ <--> C <--> rte[8].port++;
rte[1].port++ <--> C <--> rte[9].port++;
rte[2].port++ <--> C <--> rte[4].port++;
rte[2].port++ <--> C <--> rte[5].port++;
rte[2].port++ <--> C <--> rte[9].port++;
rte[3].port++ <--> C <--> rte[4].port++;
rte[3].port++ <--> C <--> rte[7].port++;
rte[3].port++ <--> C <--> rte[8].port++;
rte[4].port++ <--> C <--> rte[6].port++;
rte[4].port++ <--> C <--> rte[7].port++;
rte[5].port++ <--> C <--> rte[6].port++;
rte[5].port++ <--> C <--> rte[7].port++;
rte[6].port++ <--> C <--> rte[7].port++;
rte[6].port++ <--> C <--> rte[8].port++;
rte[7].port++ <--> C <--> rte[8].port++;
}

