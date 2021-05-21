#ifndef CORE_LOG_H
#define CORE_LOG_H

#include "def.h"
#include "core_thread.h"


// 日志输出级别
enum LogLevel: uint8
{
    XLOG_NONE		=	0,	// 不打印日志
    XLOG_CRASH		=   1,  // 严重错误,导致程序不能继续运行
    XLOG_ERR		=   2,  // 程序错误,导致处理失败(如申请内存失败等)
    XLOG_WARNING	=	3,	// 程序警告,可能导致错误产生(如传入不合法参数)
    XLOG_NOTICE		=   4,  // 正常但是值得注意的情况
    XLOG_INFO	    =   5,  // 业务相关的信息(不影响程序流程，如显示用户登陆下线信息)
    XLOG_DEBUG	    =   6,  // 调试信息(不影响程序流程，如打印当前内存池中未使用内存块数目等)
    XLOG_TRACE		=	7,	// 打印程序运行轨迹
    XLOG_LEVEL_LIMIT
};

// 日志输出位置
enum LogOption: uint8
{
    XLOG_OPTION_FILE		=   0x01,   // log on to file, default
    XLOG_OPTION_STDOUT		=   0x02,   // log on the console if errors in sending
    XLOG_OPTION_STDERR		=   0x04,   // log on the stderr stream
    XLOG_OPTION_TCP			=   0x08,	// log on the tcp stream
};

struct LogParam
{
    std::string prefix;
    uint8 level;
    uint8 option;
    uint32 max_line;
    uint32 keep_days;

    LogParam()
        : prefix("log"),
          level(LogLevel::XLOG_TRACE),
          option(LogOption::XLOG_OPTION_FILE),
          max_line(1000),
          keep_days(30)
    {
    }

    static LogParam default_instance;
};

class ByteBuffer;
class Mutex;

class XLog: public Runable
{
public:
    static XLog *instance() {
        static XLog obj;
        return &obj;
    }
    bool init(const LogParam &param);
    void set_prefix(std::string name);
    void set_level(uint8 level);
    void set_option(uint8);
    void set_keep_days(uint32 days);
    bool open_new_file();
    void xprintf(uint8 level, const char cszFormat[], ...);
protected:
    XLog();
    ~XLog();
    virtual void run() override;
private:
    LogParam param_;
    FILE * fd_;
    uint32 lines_;
    Mutex *mtx_;
    ByteBuffer *buffer_;
};


#define XCRASH(format, ...)	   XLog::instance()->printf(XLOG_CRASH, "[%s, %d] "format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define XERROR(format, ...)    XLog::instance()->printf(XLOG_ERR, "[%s, %d] "format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define XWARNING(format, ...)  XLog::instance()->printf(XLOG_WARNING, "[%s, %d] "format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define XNOTICE(format, ...)   XLog::instance()->printf(XLOG_NOTICE, "[%s, %d] "format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define XINFO(format, ...)     XLog::instance()->printf(XLOG_INFO, "[%s, %d] "format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define XDEBUG(format, ...)    XLog::instance()->printf(XLOG_DEBUG, "[%s, %d] "format, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#endif // CORE_LOG_H
