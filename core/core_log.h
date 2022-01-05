#ifndef CORE_LOG_H
#define CORE_LOG_H

#include "def.h"
#include "core_thread.h"
#include "core_timer.h"

// 日志输出级别
enum LogLevel: uint8
{
    LOG_NONE		=	0,	// 不打印日志
    LOG_FATAL		=   1,  // 严重错误,导致程序不能继续运行
    LOG_ERROR       =   2,  // 程序错误,导致处理失败(如申请内存失败等)
    LOG_WARN        =	3,	// 程序警告,可能导致错误产生(如传入不合法参数)
    LOG_NOTICE		=   4,  // 正常但是值得注意的情况
    LOG_INFO	    =   5,  // 业务相关的信息(不影响程序流程，如显示用户登陆下线信息)
    LOG_DEBUG	    =   6,  // 调试信息(不影响程序流程，如打印当前内存池中未使用内存块数目等)
    LOG_TRACE		=	7,	// 打印程序运行轨迹
};

// 日志输出位置
enum LogOption: uint8
{
    LOG_OPTION_FILE         =   0x01,   // log on to file, default
    LOG_OPTION_STDOUT		=   0x02,   // log on the console if errors in sending
    //LOG_OPTION_STDERR		=   0x04,   // log on the stderr stream
    //LOG_OPTION_TCP		=   0x08,	// log on the tcp stream
};

struct LogParam
{
    std::string out_path;
    std::string prefix;
    uint8 level;
    uint8 option;
    uint32 max_size;
    uint32 keep_days;

    LogParam()
        : out_path("./log/"),
          prefix("game_"),
          level(LogLevel::LOG_TRACE),
          option(LogOption::LOG_OPTION_FILE),
          max_size(100*1024*1024), //100M
          keep_days(30)
    {
    }

    static LogParam default_instance;
};

class ByteBuffer;
class Mutex;

class XLog: public Runable, public Timer
{
public:
    static XLog *instance() {
        static XLog obj;
        return &obj;
    }
    bool init(const LogParam &param);
    void set_level(uint8 level);
    void set_option(uint8 option);
    void set_keep_days(uint32 days);
    void xprintf(uint8 level, const char *func, int line, const char cszFormat[], ...);
protected:
    XLog();
    ~XLog();
    virtual void run() override;
    virtual void on_timer() override;
    bool open_new_file();
private:
    LogParam param_;
    FILE * fd_;
    uint32 file_size_;
    uint32 index_;
    Mutex *mtx_;
    std::string data_;
};


#define XFATAL(format, ...)	   XLog::instance()->xprintf(LOG_FATAL, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define XERROR(format, ...)    XLog::instance()->xprintf(LOG_ERROR, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define XWARN(format, ...)     XLog::instance()->xprintf(LOG_WARN, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define XINFO(format, ...)     XLog::instance()->xprintf(LOG_INFO, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define XDEBUG(format, ...)    XLog::instance()->xprintf(LOG_DEBUG, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)


#endif // CORE_LOG_H
