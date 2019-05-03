#include "MessageDispatcher.h"
#include "x_generated.h"
#include "string"
#include "Tag.h"

MessageDispatcher::MessageDispatcher(int port, int maxWorker) : Poller(port, maxWorker) {

}

int MessageDispatcher::onReadMsg(Session &conn, int bytesNum) {
    int offset = 0;

    int leftSize = conn.readBuffer.size;
    for (;;) {
        if (leftSize < 8)
            return offset;
        int msgLen = *(int *) (conn.readBuffer.buff + offset);
        //TODO size > 10M abort

        if (leftSize < msgLen)
            return offset;


        int flatTag = *(int *) (conn.readBuffer.buff + offset + 4);
        unsigned char *flatBuf = (conn.readBuffer.buff + offset + 4 + 4);

        offset += msgLen;
        leftSize -= msgLen;
        int flatLen = msgLen - 8;
        flatbuffers::FlatBufferBuilder builder_out;
        builder_out.PushBytes(flatBuf, flatLen);
        flatbuffers::Verifier verify(builder_out.GetCurrentBufferPointer(), builder_out.GetSize());
        lock.lock();
        this->onRecvMsg(conn, flatTag, builder_out, verify);
        lock.unlock();
    }
    return offset;
}

int MessageDispatcher::onRecvMsg(Session &conn, int tag, flatbuffers::FlatBufferBuilder &reqBuilder,
                                 flatbuffers::Verifier &verify) {
    switch (tag) {
        case TEST_TAG: {
            return this->handleTestTag(conn, tag, reqBuilder, verify);
        }
        case TEST_TAG2: {
            break;
        }
        default: {
            abort();
        }
    }

    return 0;
}

int MessageDispatcher::sendFlatMsg(Session &conn, int tag, flatbuffers::FlatBufferBuilder &builder) {
    unsigned char *msg1buff = builder.GetBufferPointer();
    int msg1len = builder.GetSize();

    int head[2];
    head[0] = 4 + 4 + msg1len;
    head[1] = tag;

    Msg msg = {0};
    msg.buff = (unsigned char *) head;
    msg.len = 8;
    this->sendMsg(conn, msg);

    msg.buff = (unsigned char *) msg1buff;
    msg.len = msg1len;
    this->sendMsg(conn, msg);
    return 0;
}

int MessageDispatcher::handleTestTag(Session &conn, int tag, flatbuffers::FlatBufferBuilder &reqBuilder,
                                     flatbuffers::Verifier &verify) {
    if (!FlatIDL::VerifyMsg1Buffer(verify))
        return -1;
    auto recvMsg1 = FlatIDL::GetMsg1(reqBuilder.GetCurrentBufferPointer());
    std::cout << recvMsg1->str1()->str() << std::endl;

    {
        flatbuffers::FlatBufferBuilder rspBuilder;
        auto int1 = TEST_TAG;
        auto str1 = rspBuilder.CreateString("str1");
        auto str2 = rspBuilder.CreateString("str2");
        auto rspMsg1 = FlatIDL::CreateMsg1(rspBuilder, int1, str1, str2);
        rspBuilder.Finish(rspMsg1);

        this->sendFlatMsg(conn, 1, rspBuilder);
    }
    return 0;
}