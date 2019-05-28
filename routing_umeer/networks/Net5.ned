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
        rte[3]: Node {
            address = index;
        }
    connections:
      
        
 
rte[0].port++ <--> C <--> rte[1].port++;
rte[0].port++ <--> C <--> rte[2].port++;
rte[1].port++ <--> C <--> rte[2].port++;







}

