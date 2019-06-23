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
            //https://wondernetwork.com/pings
            //https://enterprise.verizon.com/terms/latency/
            //https://www.dotcom-tools.com/internet-backbone-latency.aspx
            datarate =  7200kbps; 
            //https://www.akamai.com/us/en/about/news/press/2017-press/akamai-releases-first-quarter-2017-state-of-the-internet-connectivity-report.jsp
            //https://www.fastmetrics.com/internet-connection-speed-by-country.php#top-10-worldwide 
        }
    submodules:
        rte[10]: Node {
            address = index;
        }
    connections:
             
rte[0].port++ <--> C <--> rte[3].port++;
rte[1].port++ <--> C <--> rte[2].port++;
rte[1].port++ <--> C <--> rte[4].port++;
rte[2].port++ <--> C <--> rte[4].port++;
rte[2].port++ <--> C <--> rte[8].port++;
rte[2].port++ <--> C <--> rte[9].port++;
rte[3].port++ <--> C <--> rte[5].port++;
rte[3].port++ <--> C <--> rte[8].port++;
rte[3].port++ <--> C <--> rte[9].port++;
rte[4].port++ <--> C <--> rte[5].port++;
rte[5].port++ <--> C <--> rte[6].port++;
rte[5].port++ <--> C <--> rte[7].port++;
   

}

