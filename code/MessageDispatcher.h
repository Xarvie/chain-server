#ifndef CHAIN_SERVER_MESSAGEDISPATCHER_H
#define CHAIN_SERVER_MESSAGEDISPATCHER_H

#include <Poller.h>
#include "x_generated.h"
class MessageDispatcher : public Poller {
public:
    explicit MessageDispatcher(int port, int maxWorker = 4);

    int onReadMsg(Session &conn, int bytesNum) override;

    virtual int onRecvMsg(Session &conn, int tag, flatbuffers::FlatBufferBuilder& msg, flatbuffers::Verifier & verifier);

    virtual int sendFlatMsg(Session &conn, int tag, flatbuffers::FlatBufferBuilder& msg);

public:
    int handleTestTag(Session &conn, int tag, flatbuffers::FlatBufferBuilder &reqBuilder,
                      flatbuffers::Verifier &verify);
    std::mutex lock;
};


#endif //CHAIN_SERVER_MESSAGEDISPATCHER_H
