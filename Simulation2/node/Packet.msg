//
// Represents a packet in the network.
//
cplusplus {{
#include "PacketType.h"
}}

enum PacketType;

packet Packet
{
    int srcAddr @packetData;
    int destAddr @packetData;
    int hopCount @packetData;
    
   // This is an enum and it has the following meaning
   // 0 - Transaction  
   
   // 1 - Chain Request (with anonimyzer if the userXID is set with the target ID (different id from destination))
   // 11 - Okay i will help you back
   // 2 - Send Chain (with anonimyzer if userXID is set)
   
   // 3 - Ack of Transaction
   
   // 4 - Information Dissemination [OBSOLETE]
   // 5 - Busy Message
   
   
   
    int packetType @packetData;  
    //int packetType @enum(PacketType);
    
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
    