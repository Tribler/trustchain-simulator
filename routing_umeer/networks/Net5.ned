package networks;

import node.Node;
import ned.DatarateChannel;



network Net5
{
    
    types:
        //here it set the connection speed and dealy (eventualy also packet drop)
        channel C extends DatarateChannel
        {
            delay =  100ms;
            datarate =  7200kbps; //https://www.fastmetrics.com/internet-connection-speed-by-country.php#top-10-worldwide 
        }
    submodules:
        rte[10]: Node {
            address = index;
        }
    connections:
             
    rte[0].port++ <--> C <--> rte[3].port++;
    rte[1].port++ <--> C <--> rte[4].port++;
    rte[1].port++ <--> C <--> rte[8].port++;
    rte[1].port++ <--> C <--> rte[9].port++;
    rte[2].port++ <--> C <--> rte[4].port++;
    rte[3].port++ <--> C <--> rte[6].port++;
    rte[3].port++ <--> C <--> rte[7].port++;
    rte[4].port++ <--> C <--> rte[5].port++;
    rte[4].port++ <--> C <--> rte[6].port++;
    rte[6].port++ <--> C <--> rte[7].port++;
    rte[6].port++ <--> C <--> rte[9].port++;

}

