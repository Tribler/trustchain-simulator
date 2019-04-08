packet PacketChain
{
    int srcAddr @packetData;
    int destAddr @packetData;
    int hopCount @packetData;
    int extraInfo @packetData;
    
    int databaseSize @packetData;
    int userA[] @packetData;
    int userB[] @packetData;
    int transaction[] @packetData;
}
