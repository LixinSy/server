#ifndef NET_PB_PARSER_H
#define NET_PB_PARSER_H

#include "net_msg_def.h"
#include "net_parser.h"
#include "net_pb_message.h"

class PBParser: public Parser
{
public:
    PBParser();
    virtual ~PBParser();
    virtual int to_read_buffer(const void *src, int len);
    virtual int need_bytes();
    virtual BaseMessage *get_message();
    virtual int to_write_buffer(const BaseMessage *msg);
    virtual ByteBuffer* get_write_buffer();
    PBMessage * create_message(int cmd);
private:
    ByteBuffer *read_buffer_;
    ByteBuffer *write_buffer_;
};


class PBParserCreator: public ParserCreator
{
    virtual ~PBParserCreator(){}
    Parser * create() override {
        return new PBParser();
    }
};

#endif // NET_PB_PARSER_H
