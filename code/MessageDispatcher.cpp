#include "MessageDispatcher.h"
#include "x_generated.h"
#include "string"

MessageDispatcher::MessageDispatcher(int port, int maxWorker) : Poller(port, maxWorker) {
    flatbuffers::FlatBufferBuilder builder;
    auto str3 = builder.CreateString("str3");
    auto orc = FlatIDL::CreateMsg1(builder, 1, 2, str3);
    builder.Finish(orc);
    unsigned char *buff = builder.GetBufferPointer();
    int len = builder.GetSize();
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
        switch (flatTag) {
            case 1: {
                flatbuffers::FlatBufferBuilder builder_out;
                builder_out.PushBytes(flatBuf, flatLen);

                flatbuffers::Verifier verify(builder_out.GetCurrentBufferPointer(), builder_out.GetSize());
                bool verify_flag = FlatIDL::VerifyMsg1Buffer(verify);
                if (!verify_flag) {
                    return -1;
                }

                auto msg1 = FlatIDL::GetMsg1(builder_out.GetCurrentBufferPointer());
                std::cout << msg1->str1()->str() << std::endl;



                {
                    flatbuffers::FlatBufferBuilder builder;
                    auto int1 = 1;
                    auto str1 = builder.CreateString("str1");
                    auto str2 = builder.CreateString("str2");

                    auto msg1 = FlatIDL::CreateMsg1(builder, int1, str1, str2);
                    builder.Finish(msg1);

                    unsigned char *msg1buff = builder.GetBufferPointer();
                    int msg1len = builder.GetSize();
                    unsigned char * s = (unsigned char*)xmalloc(msg1len + 4 + 4);
                    int msgsize = msg1len + 4 + 4;
                    *(((int*)s) + 0) = msgsize;
                    *(((int*)s) + 1) = 1;
                    memcpy(((int*)s) + 2, msg1buff, msg1len);

                    Msg msg = {0};
                    msg.buff = s;
                    msg.len = msgsize;
                    this->sendMsg(conn, msg);//TODO force send
                    xfree(s);
                }

                break;
            }
            case 2: {
                break;
            }
            default: {
                abort();
            }
        }

    }

    Msg msg = {0};
    msg.buff = conn.readBuffer.buff;
    msg.len = conn.readBuffer.size;
    //this->sendMsg(conn, msg);
    return bytesNum;
}

int MessageDispatcher::onRecvProto(Session &conn, int bytesNum) {
    return 0;
}