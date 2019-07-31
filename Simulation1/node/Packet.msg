//
// Represents a packet in the network.
//
packet Packet
{
    int srcAddr @packetData;
    int destAddr @packetData;
    int hopCount @packetData;
    
   // This is an enum and it has the following meaning
   // 0 - Transaction  
   // 1 - Chain Request
   // 2 - Send Chain
   // 3 - Ack of Transaction
   // 4 - Information Dissemination
   // 5 - Busy Message
    int packetType @packetData;
    
    //Send Transaction
    int transactionValue @packetData;
    int myChainSeqNum @packetData;   
   
    //Sending Chain Information
    //int databaseSize @packetData;
    //int userA[] @packetData;
    int userBID[] @packetData;
    int userBSeqNum[] @packetData;
    int transaction[] @packetData;
    
    //Dissemination data
    int userXID @packetData;
    int userXSeqNum @packetData;
    int userYID @packetData;
    int userYSeqNum @packetData;
}

