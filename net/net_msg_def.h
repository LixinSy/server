#ifndef NET_MSG_DEF_H
#define NET_MSG_DEF_H

#include <google/protobuf/message.h>
#include "def.h"

#define MAX_MSG_SIZE    2*1024*1024

struct MsgHead
{
    uint32  msg_id;
    uint32  msg_seq;
};

struct ReqHead: public MsgHead
{
    uint16 src;
};

struct RespHead: public MsgHead
{
    int32 result;
};

//template <int, typename Msg>
struct ReqMessage
{
    ReqHead head;
    google::protobuf::Message *body;
};

struct RespMessage
{
    RespHead head;
    google::protobuf::Message *body;
};

#endif // NET_MSG_DEF_H
