//
// Generated file, do not edit! Created by nedtool 5.4 from node/Packet.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include "Packet_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace {
template <class T> inline
typename std::enable_if<std::is_polymorphic<T>::value && std::is_base_of<omnetpp::cObject,T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)(static_cast<const omnetpp::cObject *>(t));
}

template <class T> inline
typename std::enable_if<std::is_polymorphic<T>::value && !std::is_base_of<omnetpp::cObject,T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)dynamic_cast<const void *>(t);
}

template <class T> inline
typename std::enable_if<!std::is_polymorphic<T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)static_cast<const void *>(t);
}

}


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule to generate operator<< for shared_ptr<T>
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const std::shared_ptr<T>& t) { return out << t.get(); }

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');

    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(Packet)

Packet::Packet(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

Packet::Packet(const Packet& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

Packet::~Packet()
{
    delete [] this->userBID;
    delete [] this->userBSeqNum;
    delete [] this->transaction;
}

Packet& Packet::operator=(const Packet& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void Packet::copy(const Packet& other)
{
    this->srcAddr = other.srcAddr;
    this->destAddr = other.destAddr;
    this->hopCount = other.hopCount;
    this->packetType = other.packetType;
    this->transactionValue = other.transactionValue;
    this->myChainSeqNum = other.myChainSeqNum;
    delete [] this->userBID;
    this->userBID = (other.userBID_arraysize==0) ? nullptr : new int[other.userBID_arraysize];
    userBID_arraysize = other.userBID_arraysize;
    for (size_t i = 0; i < userBID_arraysize; i++) {
        this->userBID[i] = other.userBID[i];
    }
    delete [] this->userBSeqNum;
    this->userBSeqNum = (other.userBSeqNum_arraysize==0) ? nullptr : new int[other.userBSeqNum_arraysize];
    userBSeqNum_arraysize = other.userBSeqNum_arraysize;
    for (size_t i = 0; i < userBSeqNum_arraysize; i++) {
        this->userBSeqNum[i] = other.userBSeqNum[i];
    }
    delete [] this->transaction;
    this->transaction = (other.transaction_arraysize==0) ? nullptr : new int[other.transaction_arraysize];
    transaction_arraysize = other.transaction_arraysize;
    for (size_t i = 0; i < transaction_arraysize; i++) {
        this->transaction[i] = other.transaction[i];
    }
    this->userXID = other.userXID;
    this->userXSeqNum = other.userXSeqNum;
    this->userYID = other.userYID;
    this->userYSeqNum = other.userYSeqNum;
}

void Packet::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->srcAddr);
    doParsimPacking(b,this->destAddr);
    doParsimPacking(b,this->hopCount);
    doParsimPacking(b,this->packetType);
    doParsimPacking(b,this->transactionValue);
    doParsimPacking(b,this->myChainSeqNum);
    b->pack(userBID_arraysize);
    doParsimArrayPacking(b,this->userBID,userBID_arraysize);
    b->pack(userBSeqNum_arraysize);
    doParsimArrayPacking(b,this->userBSeqNum,userBSeqNum_arraysize);
    b->pack(transaction_arraysize);
    doParsimArrayPacking(b,this->transaction,transaction_arraysize);
    doParsimPacking(b,this->userXID);
    doParsimPacking(b,this->userXSeqNum);
    doParsimPacking(b,this->userYID);
    doParsimPacking(b,this->userYSeqNum);
}

void Packet::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->srcAddr);
    doParsimUnpacking(b,this->destAddr);
    doParsimUnpacking(b,this->hopCount);
    doParsimUnpacking(b,this->packetType);
    doParsimUnpacking(b,this->transactionValue);
    doParsimUnpacking(b,this->myChainSeqNum);
    delete [] this->userBID;
    b->unpack(userBID_arraysize);
    if (userBID_arraysize == 0) {
        this->userBID = nullptr;
    } else {
        this->userBID = new int[userBID_arraysize];
        doParsimArrayUnpacking(b,this->userBID,userBID_arraysize);
    }
    delete [] this->userBSeqNum;
    b->unpack(userBSeqNum_arraysize);
    if (userBSeqNum_arraysize == 0) {
        this->userBSeqNum = nullptr;
    } else {
        this->userBSeqNum = new int[userBSeqNum_arraysize];
        doParsimArrayUnpacking(b,this->userBSeqNum,userBSeqNum_arraysize);
    }
    delete [] this->transaction;
    b->unpack(transaction_arraysize);
    if (transaction_arraysize == 0) {
        this->transaction = nullptr;
    } else {
        this->transaction = new int[transaction_arraysize];
        doParsimArrayUnpacking(b,this->transaction,transaction_arraysize);
    }
    doParsimUnpacking(b,this->userXID);
    doParsimUnpacking(b,this->userXSeqNum);
    doParsimUnpacking(b,this->userYID);
    doParsimUnpacking(b,this->userYSeqNum);
}

int Packet::getSrcAddr() const
{
    return this->srcAddr;
}

void Packet::setSrcAddr(int srcAddr)
{
    this->srcAddr = srcAddr;
}

int Packet::getDestAddr() const
{
    return this->destAddr;
}

void Packet::setDestAddr(int destAddr)
{
    this->destAddr = destAddr;
}

int Packet::getHopCount() const
{
    return this->hopCount;
}

void Packet::setHopCount(int hopCount)
{
    this->hopCount = hopCount;
}

int Packet::getPacketType() const
{
    return this->packetType;
}

void Packet::setPacketType(int packetType)
{
    this->packetType = packetType;
}

int Packet::getTransactionValue() const
{
    return this->transactionValue;
}

void Packet::setTransactionValue(int transactionValue)
{
    this->transactionValue = transactionValue;
}

int Packet::getMyChainSeqNum() const
{
    return this->myChainSeqNum;
}

void Packet::setMyChainSeqNum(int myChainSeqNum)
{
    this->myChainSeqNum = myChainSeqNum;
}

size_t Packet::getUserBIDArraySize() const
{
    return userBID_arraysize;
}

int Packet::getUserBID(size_t k) const
{
    if (k >= userBID_arraysize) throw omnetpp::cRuntimeError("Array of size userBID_arraysize indexed by %lu", (unsigned long)k);
    return this->userBID[k];
}

void Packet::setUserBIDArraySize(size_t newSize)
{
    int *userBID2 = (newSize==0) ? nullptr : new int[newSize];
    size_t minSize = userBID_arraysize < newSize ? userBID_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        userBID2[i] = this->userBID[i];
    for (size_t i = minSize; i < newSize; i++)
        userBID2[i] = 0;
    delete [] this->userBID;
    this->userBID = userBID2;
    userBID_arraysize = newSize;
}

void Packet::setUserBID(size_t k, int userBID)
{
    if (k >= userBID_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    this->userBID[k] = userBID;
}

void Packet::insertUserBID(size_t k, int userBID)
{
    if (k > userBID_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = userBID_arraysize + 1;
    int *userBID2 = new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        userBID2[i] = this->userBID[i];
    userBID2[k] = userBID;
    for (i = k + 1; i < newSize; i++)
        userBID2[i] = this->userBID[i-1];
    delete [] this->userBID;
    this->userBID = userBID2;
    userBID_arraysize = newSize;
}

void Packet::insertUserBID(int userBID)
{
    insertUserBID(userBID_arraysize, userBID);
}

void Packet::eraseUserBID(size_t k)
{
    if (k >= userBID_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = userBID_arraysize - 1;
    int *userBID2 = (newSize == 0) ? nullptr : new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        userBID2[i] = this->userBID[i];
    for (i = k; i < newSize; i++)
        userBID2[i] = this->userBID[i+1];
    delete [] this->userBID;
    this->userBID = userBID2;
    userBID_arraysize = newSize;
}

size_t Packet::getUserBSeqNumArraySize() const
{
    return userBSeqNum_arraysize;
}

int Packet::getUserBSeqNum(size_t k) const
{
    if (k >= userBSeqNum_arraysize) throw omnetpp::cRuntimeError("Array of size userBSeqNum_arraysize indexed by %lu", (unsigned long)k);
    return this->userBSeqNum[k];
}

void Packet::setUserBSeqNumArraySize(size_t newSize)
{
    int *userBSeqNum2 = (newSize==0) ? nullptr : new int[newSize];
    size_t minSize = userBSeqNum_arraysize < newSize ? userBSeqNum_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        userBSeqNum2[i] = this->userBSeqNum[i];
    for (size_t i = minSize; i < newSize; i++)
        userBSeqNum2[i] = 0;
    delete [] this->userBSeqNum;
    this->userBSeqNum = userBSeqNum2;
    userBSeqNum_arraysize = newSize;
}

void Packet::setUserBSeqNum(size_t k, int userBSeqNum)
{
    if (k >= userBSeqNum_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    this->userBSeqNum[k] = userBSeqNum;
}

void Packet::insertUserBSeqNum(size_t k, int userBSeqNum)
{
    if (k > userBSeqNum_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = userBSeqNum_arraysize + 1;
    int *userBSeqNum2 = new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        userBSeqNum2[i] = this->userBSeqNum[i];
    userBSeqNum2[k] = userBSeqNum;
    for (i = k + 1; i < newSize; i++)
        userBSeqNum2[i] = this->userBSeqNum[i-1];
    delete [] this->userBSeqNum;
    this->userBSeqNum = userBSeqNum2;
    userBSeqNum_arraysize = newSize;
}

void Packet::insertUserBSeqNum(int userBSeqNum)
{
    insertUserBSeqNum(userBSeqNum_arraysize, userBSeqNum);
}

void Packet::eraseUserBSeqNum(size_t k)
{
    if (k >= userBSeqNum_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = userBSeqNum_arraysize - 1;
    int *userBSeqNum2 = (newSize == 0) ? nullptr : new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        userBSeqNum2[i] = this->userBSeqNum[i];
    for (i = k; i < newSize; i++)
        userBSeqNum2[i] = this->userBSeqNum[i+1];
    delete [] this->userBSeqNum;
    this->userBSeqNum = userBSeqNum2;
    userBSeqNum_arraysize = newSize;
}

size_t Packet::getTransactionArraySize() const
{
    return transaction_arraysize;
}

int Packet::getTransaction(size_t k) const
{
    if (k >= transaction_arraysize) throw omnetpp::cRuntimeError("Array of size transaction_arraysize indexed by %lu", (unsigned long)k);
    return this->transaction[k];
}

void Packet::setTransactionArraySize(size_t newSize)
{
    int *transaction2 = (newSize==0) ? nullptr : new int[newSize];
    size_t minSize = transaction_arraysize < newSize ? transaction_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        transaction2[i] = this->transaction[i];
    for (size_t i = minSize; i < newSize; i++)
        transaction2[i] = 0;
    delete [] this->transaction;
    this->transaction = transaction2;
    transaction_arraysize = newSize;
}

void Packet::setTransaction(size_t k, int transaction)
{
    if (k >= transaction_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    this->transaction[k] = transaction;
}

void Packet::insertTransaction(size_t k, int transaction)
{
    if (k > transaction_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = transaction_arraysize + 1;
    int *transaction2 = new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        transaction2[i] = this->transaction[i];
    transaction2[k] = transaction;
    for (i = k + 1; i < newSize; i++)
        transaction2[i] = this->transaction[i-1];
    delete [] this->transaction;
    this->transaction = transaction2;
    transaction_arraysize = newSize;
}

void Packet::insertTransaction(int transaction)
{
    insertTransaction(transaction_arraysize, transaction);
}

void Packet::eraseTransaction(size_t k)
{
    if (k >= transaction_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = transaction_arraysize - 1;
    int *transaction2 = (newSize == 0) ? nullptr : new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        transaction2[i] = this->transaction[i];
    for (i = k; i < newSize; i++)
        transaction2[i] = this->transaction[i+1];
    delete [] this->transaction;
    this->transaction = transaction2;
    transaction_arraysize = newSize;
}

int Packet::getUserXID() const
{
    return this->userXID;
}

void Packet::setUserXID(int userXID)
{
    this->userXID = userXID;
}

int Packet::getUserXSeqNum() const
{
    return this->userXSeqNum;
}

void Packet::setUserXSeqNum(int userXSeqNum)
{
    this->userXSeqNum = userXSeqNum;
}

int Packet::getUserYID() const
{
    return this->userYID;
}

void Packet::setUserYID(int userYID)
{
    this->userYID = userYID;
}

int Packet::getUserYSeqNum() const
{
    return this->userYSeqNum;
}

void Packet::setUserYSeqNum(int userYSeqNum)
{
    this->userYSeqNum = userYSeqNum;
}

class PacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
        FIELD_srcAddr,
        FIELD_destAddr,
        FIELD_hopCount,
        FIELD_packetType,
        FIELD_transactionValue,
        FIELD_myChainSeqNum,
        FIELD_userBID,
        FIELD_userBSeqNum,
        FIELD_transaction,
        FIELD_userXID,
        FIELD_userXSeqNum,
        FIELD_userYID,
        FIELD_userYSeqNum,
    };
  public:
    PacketDescriptor();
    virtual ~PacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(PacketDescriptor)

PacketDescriptor::PacketDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(Packet)), "omnetpp::cPacket")
{
    propertynames = nullptr;
}

PacketDescriptor::~PacketDescriptor()
{
    delete[] propertynames;
}

bool PacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<Packet *>(obj)!=nullptr;
}

const char **PacketDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *PacketDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int PacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 13+basedesc->getFieldCount() : 13;
}

unsigned int PacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_srcAddr
        FD_ISEDITABLE,    // FIELD_destAddr
        FD_ISEDITABLE,    // FIELD_hopCount
        FD_ISEDITABLE,    // FIELD_packetType
        FD_ISEDITABLE,    // FIELD_transactionValue
        FD_ISEDITABLE,    // FIELD_myChainSeqNum
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_userBID
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_userBSeqNum
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_transaction
        FD_ISEDITABLE,    // FIELD_userXID
        FD_ISEDITABLE,    // FIELD_userXSeqNum
        FD_ISEDITABLE,    // FIELD_userYID
        FD_ISEDITABLE,    // FIELD_userYSeqNum
    };
    return (field >= 0 && field < 13) ? fieldTypeFlags[field] : 0;
}

const char *PacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "srcAddr",
        "destAddr",
        "hopCount",
        "packetType",
        "transactionValue",
        "myChainSeqNum",
        "userBID",
        "userBSeqNum",
        "transaction",
        "userXID",
        "userXSeqNum",
        "userYID",
        "userYSeqNum",
    };
    return (field >= 0 && field < 13) ? fieldNames[field] : nullptr;
}

int PacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0] == 's' && strcmp(fieldName, "srcAddr") == 0) return base+0;
    if (fieldName[0] == 'd' && strcmp(fieldName, "destAddr") == 0) return base+1;
    if (fieldName[0] == 'h' && strcmp(fieldName, "hopCount") == 0) return base+2;
    if (fieldName[0] == 'p' && strcmp(fieldName, "packetType") == 0) return base+3;
    if (fieldName[0] == 't' && strcmp(fieldName, "transactionValue") == 0) return base+4;
    if (fieldName[0] == 'm' && strcmp(fieldName, "myChainSeqNum") == 0) return base+5;
    if (fieldName[0] == 'u' && strcmp(fieldName, "userBID") == 0) return base+6;
    if (fieldName[0] == 'u' && strcmp(fieldName, "userBSeqNum") == 0) return base+7;
    if (fieldName[0] == 't' && strcmp(fieldName, "transaction") == 0) return base+8;
    if (fieldName[0] == 'u' && strcmp(fieldName, "userXID") == 0) return base+9;
    if (fieldName[0] == 'u' && strcmp(fieldName, "userXSeqNum") == 0) return base+10;
    if (fieldName[0] == 'u' && strcmp(fieldName, "userYID") == 0) return base+11;
    if (fieldName[0] == 'u' && strcmp(fieldName, "userYSeqNum") == 0) return base+12;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *PacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_srcAddr
        "int",    // FIELD_destAddr
        "int",    // FIELD_hopCount
        "int",    // FIELD_packetType
        "int",    // FIELD_transactionValue
        "int",    // FIELD_myChainSeqNum
        "int",    // FIELD_userBID
        "int",    // FIELD_userBSeqNum
        "int",    // FIELD_transaction
        "int",    // FIELD_userXID
        "int",    // FIELD_userXSeqNum
        "int",    // FIELD_userYID
        "int",    // FIELD_userYSeqNum
    };
    return (field >= 0 && field < 13) ? fieldTypeStrings[field] : nullptr;
}

const char **PacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case FIELD_srcAddr: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_destAddr: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_hopCount: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_packetType: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_transactionValue: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_myChainSeqNum: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_userBID: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_userBSeqNum: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_transaction: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_userXID: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_userXSeqNum: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_userYID: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        case FIELD_userYSeqNum: {
            static const char *names[] = { "packetData",  nullptr };
            return names;
        }
        default: return nullptr;
    }
}

const char *PacketDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case FIELD_srcAddr:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_destAddr:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_hopCount:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_packetType:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_transactionValue:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_myChainSeqNum:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_userBID:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_userBSeqNum:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_transaction:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_userXID:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_userXSeqNum:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_userYID:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        case FIELD_userYSeqNum:
            if (!strcmp(propertyname, "packetData")) return "";
            return nullptr;
        default: return nullptr;
    }
}

int PacketDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    Packet *pp = (Packet *)object; (void)pp;
    switch (field) {
        case FIELD_userBID: return pp->getUserBIDArraySize();
        case FIELD_userBSeqNum: return pp->getUserBSeqNumArraySize();
        case FIELD_transaction: return pp->getTransactionArraySize();
        default: return 0;
    }
}

const char *PacketDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Packet *pp = (Packet *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string PacketDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Packet *pp = (Packet *)object; (void)pp;
    switch (field) {
        case FIELD_srcAddr: return long2string(pp->getSrcAddr());
        case FIELD_destAddr: return long2string(pp->getDestAddr());
        case FIELD_hopCount: return long2string(pp->getHopCount());
        case FIELD_packetType: return long2string(pp->getPacketType());
        case FIELD_transactionValue: return long2string(pp->getTransactionValue());
        case FIELD_myChainSeqNum: return long2string(pp->getMyChainSeqNum());
        case FIELD_userBID: return long2string(pp->getUserBID(i));
        case FIELD_userBSeqNum: return long2string(pp->getUserBSeqNum(i));
        case FIELD_transaction: return long2string(pp->getTransaction(i));
        case FIELD_userXID: return long2string(pp->getUserXID());
        case FIELD_userXSeqNum: return long2string(pp->getUserXSeqNum());
        case FIELD_userYID: return long2string(pp->getUserYID());
        case FIELD_userYSeqNum: return long2string(pp->getUserYSeqNum());
        default: return "";
    }
}

bool PacketDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    Packet *pp = (Packet *)object; (void)pp;
    switch (field) {
        case FIELD_srcAddr: pp->setSrcAddr(string2long(value)); return true;
        case FIELD_destAddr: pp->setDestAddr(string2long(value)); return true;
        case FIELD_hopCount: pp->setHopCount(string2long(value)); return true;
        case FIELD_packetType: pp->setPacketType(string2long(value)); return true;
        case FIELD_transactionValue: pp->setTransactionValue(string2long(value)); return true;
        case FIELD_myChainSeqNum: pp->setMyChainSeqNum(string2long(value)); return true;
        case FIELD_userBID: pp->setUserBID(i,string2long(value)); return true;
        case FIELD_userBSeqNum: pp->setUserBSeqNum(i,string2long(value)); return true;
        case FIELD_transaction: pp->setTransaction(i,string2long(value)); return true;
        case FIELD_userXID: pp->setUserXID(string2long(value)); return true;
        case FIELD_userXSeqNum: pp->setUserXSeqNum(string2long(value)); return true;
        case FIELD_userYID: pp->setUserYID(string2long(value)); return true;
        case FIELD_userYSeqNum: pp->setUserYSeqNum(string2long(value)); return true;
        default: return false;
    }
}

const char *PacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *PacketDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    Packet *pp = (Packet *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

