#ifndef NET_PARSER_H
#define NET_PARSER_H


class BaseMessage;
class ByteBuffer;

class Parser
{
public:
    Parser() { };
    virtual ~Parser() { };
    virtual int to_read_buffer(const void *src, int len) = 0;
    virtual int need_bytes() = 0;
    virtual BaseMessage *get_message() = 0;
    virtual void reset_read_buffer() = 0;
    virtual int to_write_buffer(const BaseMessage *msg) = 0;
    virtual ByteBuffer* get_write_buffer() = 0;
};

class ParserCreator
{
public:
    virtual ~ParserCreator() { }
    virtual Parser* create() = 0;
};

#endif // NET_PARSER_H
