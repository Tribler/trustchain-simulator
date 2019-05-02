//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

package networks;

import node.Node;
import ned.DatarateChannel;

//
// A ten-node network topology
//
network Net5
{
    types:
        //here it set the connection speed and dealy (eventualy also packet drop)
        channel C extends DatarateChannel
        {
            delay = uniform(0.01ms, 250ms);
            datarate = uniform(100kbps, 500kbps);
        }
    submodules:
        rte[3]: Node {
            address = index;
        }
    connections:
        rte[1].port++ <--> C <--> rte[0].port++;
        rte[1].port++ <--> C <--> rte[2].port++;
        rte[0].port++ <--> C <--> rte[2].port++;

}

