# TrustChain Simulator
The programs present in this repository are used simulate the TrustChain technology described in this [paper](https://www.sciencedirect.com/science/article/pii/S0167739X17318988) and are built with the [OMNeT++](https://omnetpp.org/) simulation framework.


## Project Goal
The goal of these simulations is to study and evaluate a better defence system against double-spending attack.  
The simulations and the results collected will be used to write and defend my master thesis at TuDelft.

![alt text](https://raw.githubusercontent.com/umeer/TrustChain-Simulator/master/MATLAB%20Graphs/network_graph_image.JPG)  
*This is one of the networks used to perform behavioural analyses with super connected nodes. (Based on the [Watts Strogatz](http://www.stats.ox.ac.uk/~reinert/talks/swtalkparis.pdf) model.)*

## TrustChain Description
The TrustChain is a protocol for a share interconnected data structure with enormous potential, it is designed to record transactions among strangers without central control, and it is able to support high transaction volumes.  

This system is highly scalable while maintaining stable performance independent of the numbers of active users, which is hard to achieve in blockchain structure where there is a unique shared ledger.  
([Bitcoin](https://bitcoin.org/bitcoin.pdf) can achieve between **3.3 and 7** transactions per second, the Visa system does maximum **24,000** transacions/s and the  protocol described here has **no theoretical limit** :open_mouth:.)  
This is enabled by a network composed of individual semi-independent ledger (every node has it's own) and rules that define how these are constructed.   

A transaction between two parties creates a new element in both the participant’s ledger and this new element contains some pieces of information like the value of transaction itself, details of the previous element in the ledger and signatures of both the parties.
 
## Double-Spending Attack
A double spending attack allows the attacker to utilize resources already consumed before, allowing him to fraud two or more node with the same resource set.   

Let's consider a real-world example to better understand the problem.   
You decide to go to a restaurant to order a pizza :pizza: worth 10 euro, you pay in cash and the note goes into the shop's cash desk. Now by no means you can spend that note somewhere else that day. This constriction is easy to understand in the real world, but in TrustChain things are a bit different, here using many parallel ledgers it is possible to double spent your 10 euro somewhere else multiple times before being caught by a cashier, which can take quite some time.  

This is possible because the defence system against these attacks is not very efficient.
The key idea is that soon or later an adversarial node will enter in contact with a node that has knowledge of his previous transactions, in such a situation attempting a double spending attack will result in failure. 
In a low-entropy transactions system, behaviour such as this can take a long time before being detected and this can result in chains correction increasingly complicated.   

## Motivation
Like in any good idea, there are weak points that need to be addressed and re-enforced in order for it to become a commercially approved technology.  
With this project, I want to approach one of the biggest weaknesses on TrustChain with the goal of finding a better solution for it because I believe that there are a lot of potential in this technology.:+1:


## Development Phases
- [x] Develop on paper system architecture
- [x] Created standard implementation
- [x] Implemented malicious node/s (attack)
- [x] Develop detenction performance logging system
- [x] Modify and run software on distributed super computer cluster
- [x] Test simulator in different scenarios and analyse data
- [x] Update the simulator to replicate the TrustChain testing done on [Tribler](https://github.com/Tribler/tribler)
- [ ] Study efficient algorithms and methodology to improve the performance of detection time
- [ ] Validate on paper the solution/s
- [ ] Update the standard implementation with new defence system

## Documentation
[OMNeT++ Website](https://omnetpp.org/)

[OMNeT++ Guide](https://doc.omnetpp.org/omnetpp/manual/)

[TrustChain article](https://www.sciencedirect.com/science/article/pii/S0167739X17318988)

[Thesis description doc](https://www.overleaf.com/read/mqxnswyjzcbm)

[Attack and malicious node definition doc](https://www.overleaf.com/read/wmnnttvdggzb)




