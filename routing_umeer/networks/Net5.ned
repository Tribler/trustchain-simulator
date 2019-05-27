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
            datarate =  7200kbps;//https://www.fastmetrics.com/internet-connection-speed-by-country.php#top-10-worldwide with a 10% vairiance
            // packet size https://www.isi.edu/~johnh/PAPERS/Sinha07a.pdf
        }
    submodules:
        rte[50]: Node {
            address = index;
        }
    connections:
      
        
 


rte[0].port++ <--> C <--> rte[13].port++;
rte[0].port++ <--> C <--> rte[28].port++;
rte[0].port++ <--> C <--> rte[29].port++;
rte[0].port++ <--> C <--> rte[30].port++;
rte[1].port++ <--> C <--> rte[5].port++;
rte[1].port++ <--> C <--> rte[9].port++;
rte[1].port++ <--> C <--> rte[34].port++;
rte[1].port++ <--> C <--> rte[39].port++;
rte[2].port++ <--> C <--> rte[7].port++;
rte[2].port++ <--> C <--> rte[15].port++;
rte[2].port++ <--> C <--> rte[17].port++;
rte[2].port++ <--> C <--> rte[27].port++;
rte[2].port++ <--> C <--> rte[32].port++;
rte[3].port++ <--> C <--> rte[27].port++;
rte[3].port++ <--> C <--> rte[31].port++;
rte[4].port++ <--> C <--> rte[14].port++;
rte[4].port++ <--> C <--> rte[28].port++;
rte[4].port++ <--> C <--> rte[48].port++;
rte[6].port++ <--> C <--> rte[10].port++;
rte[6].port++ <--> C <--> rte[12].port++;
rte[6].port++ <--> C <--> rte[41].port++;
rte[7].port++ <--> C <--> rte[17].port++;
rte[7].port++ <--> C <--> rte[45].port++;
rte[8].port++ <--> C <--> rte[12].port++;
rte[8].port++ <--> C <--> rte[18].port++;
rte[8].port++ <--> C <--> rte[21].port++;
rte[8].port++ <--> C <--> rte[36].port++;
rte[8].port++ <--> C <--> rte[37].port++;
rte[8].port++ <--> C <--> rte[46].port++;
rte[9].port++ <--> C <--> rte[23].port++;
rte[10].port++ <--> C <--> rte[20].port++;
rte[10].port++ <--> C <--> rte[26].port++;
rte[10].port++ <--> C <--> rte[46].port++;
rte[11].port++ <--> C <--> rte[18].port++;
rte[11].port++ <--> C <--> rte[24].port++;
rte[11].port++ <--> C <--> rte[28].port++;
rte[11].port++ <--> C <--> rte[31].port++;
rte[11].port++ <--> C <--> rte[34].port++;
rte[12].port++ <--> C <--> rte[35].port++;
rte[13].port++ <--> C <--> rte[16].port++;
rte[13].port++ <--> C <--> rte[49].port++;
rte[14].port++ <--> C <--> rte[41].port++;
rte[15].port++ <--> C <--> rte[16].port++;
rte[15].port++ <--> C <--> rte[28].port++;
rte[15].port++ <--> C <--> rte[35].port++;
rte[15].port++ <--> C <--> rte[43].port++;
rte[15].port++ <--> C <--> rte[49].port++;
rte[16].port++ <--> C <--> rte[17].port++;
rte[16].port++ <--> C <--> rte[33].port++;
rte[16].port++ <--> C <--> rte[37].port++;
rte[16].port++ <--> C <--> rte[48].port++;
rte[17].port++ <--> C <--> rte[21].port++;
rte[17].port++ <--> C <--> rte[22].port++;
rte[17].port++ <--> C <--> rte[39].port++;
rte[18].port++ <--> C <--> rte[20].port++;
rte[18].port++ <--> C <--> rte[38].port++;
rte[18].port++ <--> C <--> rte[48].port++;
rte[19].port++ <--> C <--> rte[25].port++;
rte[19].port++ <--> C <--> rte[26].port++;
rte[20].port++ <--> C <--> rte[32].port++;
rte[20].port++ <--> C <--> rte[39].port++;
rte[20].port++ <--> C <--> rte[42].port++;
rte[21].port++ <--> C <--> rte[30].port++;
rte[22].port++ <--> C <--> rte[26].port++;
rte[23].port++ <--> C <--> rte[29].port++;
rte[24].port++ <--> C <--> rte[29].port++;
rte[24].port++ <--> C <--> rte[43].port++;
rte[25].port++ <--> C <--> rte[29].port++;
rte[25].port++ <--> C <--> rte[38].port++;
rte[25].port++ <--> C <--> rte[45].port++;
rte[26].port++ <--> C <--> rte[43].port++;
rte[26].port++ <--> C <--> rte[48].port++;
rte[29].port++ <--> C <--> rte[32].port++;
rte[29].port++ <--> C <--> rte[33].port++;
rte[29].port++ <--> C <--> rte[48].port++;
rte[29].port++ <--> C <--> rte[49].port++;
rte[30].port++ <--> C <--> rte[34].port++;
rte[30].port++ <--> C <--> rte[47].port++;
rte[31].port++ <--> C <--> rte[35].port++;
rte[31].port++ <--> C <--> rte[40].port++;
rte[33].port++ <--> C <--> rte[35].port++;
rte[35].port++ <--> C <--> rte[40].port++;
rte[36].port++ <--> C <--> rte[37].port++;
rte[36].port++ <--> C <--> rte[44].port++;
rte[40].port++ <--> C <--> rte[43].port++;
rte[41].port++ <--> C <--> rte[45].port++;
rte[44].port++ <--> C <--> rte[47].port++;
rte[44].port++ <--> C <--> rte[48].port++;
rte[44].port++ <--> C <--> rte[49].port++;






}

