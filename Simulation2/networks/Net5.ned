package networks;

import node.Node;
import ned.DatarateChannel;


network Net5
{
    parameters:
        int totalNodes;
        int evilNodeIdSingle = intuniform(0, totalNodes-1);
        double delay @unit(s);
        double datarate @unit(kbps);

    types:
        //here it set the connection speed and dealy (eventualy also packet drop)
        channel C extends DatarateChannel
        {
            delay = delay;
            datarate = datarate;
        }
    submodules:
        rte[totalNodes]: Node {
            address = index;
        }
    connections:

        rte[0].port++ <--> C <--> rte[1].port++;
        rte[0].port++ <--> C <--> rte[2].port++;
        rte[0].port++ <--> C <--> rte[3].port++;
        rte[1].port++ <--> C <--> rte[3].port++;


}
