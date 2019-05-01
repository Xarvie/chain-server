#ifndef CHAIN_SERVER_MESSAGEDISPATCHER_H
#define CHAIN_SERVER_MESSAGEDISPATCHER_H

#include <Poller.h>

class MessageDispatcher : public Poller {
public:
    explicit MessageDispatcher(int port, int maxWorker = 4);

    int onReadMsg(Session &conn, int bytesNum) override;
    virtual int onRecvProto(Session &conn, int bytesNum);
};


#endif //CHAIN_SERVER_MESSAGEDISPATCHER_H
