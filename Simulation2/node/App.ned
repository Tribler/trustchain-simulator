package node;

//
// Application model to generate traffic for the network.
//
simple App like IApp
{
    parameters:
        int address;  // local node address
        int totalNodes;
        int initialMoney;
        int evilNumberOfSleepingTransaction;
        int evilNumberOfTransaction;
        int numberOfAnonymizer;
        int numberOfAnonymizerThreshold;
        int probabilityAnonymizerToBeEvil;
        string evilNodeIds;
        int transaction_limit;
        int randomSeed = intuniform(0, 1000000);
        volatile double sendIaTimeInit @unit(s);
        volatile double sendIaTime @unit(s) = default(exponential(1s)); // time between generating packets
        volatile double transactionTimeoutTime @unit(s);
        volatile double anonymusAuditingTimeoutTime @unit(s);
        volatile double sendIaTimeEvil @unit(s);
        volatile int packetLength @unit(byte);  // length of one message (fixed! no "volatile" modifier)
        volatile double anonymizerLifeTime @unit(s);
        volatile double anonymizerDisseminationTimeInit @unit(s);
        volatile double anonymizerDisseminationTime @unit(s);
        @display("i=block/browser");
        @signal[endToEndDelay](type="simtime_t");
        @signal[hopCount](type="long");
        @signal[sourceAddress](type="long");
        @statistic[endToEndDelay](title="end-to-end delay of arrived packets"; unit=s; record=vector,mean,max; interpolationmode=none);
        @statistic[hopCount](title="hop count of arrived packets"; interpolationmode=none; record=vector?,mean,max);
        @statistic[sourceAddress](title="source address of arrived packets"; interpolationmode=none; record=vector?);
    gates:
        input in;
        output out;
        output direct;
}


