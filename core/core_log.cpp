#include "core_log.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <filesystem>
#include "comm_time_util.h"
#include "comm_str_util.h"
#include "core_mutex.h"
#include "core_byte_buffer.h"
#include "core_time.h"

namespace stdfs = std::filesystem;

#define MAX_BUFF_SIZE 10*1024*1024
#define LINE_BUFF_SIZE 10*1024

std::string get_log_level_name(uint8 level) {
    std::string str = " ";
    switch (level) {
    case LogLevel::LOG_FATAL:
        str.append("FATAL"); break;
    case LogLevel::LOG_ERROR:
        str.append("ERROR"); break;
    case LogLevel::LOG_WARN:
        str.append("WARN"); break;
    case LogLevel::LOG_INFO:
        str.append("INFO"); break;
    case LogLevel::LOG_DEBUG:
        str.append("DEBUG"); break;
    case LogLevel::LOG_TRACE:
        str.append("TRACE"); break;
    }
    str.append(" ");
    return str;
}

XLog::XLog() {
    fd_ = nullptr;
    file_size_ = 0;
    index_ = 0;
    mtx_ = new Mutex(false);
    data_.reserve(MAX_BUFF_SIZE);
}

XLog::~XLog() {
    if (fd_) {
        fclose(fd_);
    }
    delete mtx_;
}

bool XLog::init(const LogParam &param) {
    param_ = param;
    return open_new_file();
}

void XLog::set_level(uint8 level) {
    param_.level = level;
}

void XLog::set_option(uint8 option) {
    param_.option = option;
}

void XLog::set_keep_days(uint32 days) {
    param_.keep_days = days;
}

bool XLog::open_new_file() {
    if (fd_) {
        ::fclose(fd_);
        fd_ = nullptr;
    }
    std::string date = Time().format(TimeFormat::YMD_N);
    std::string file_name = param_.prefix + date;
    if (index_ > 0) {
        file_name.append(".");
        file_name.append(std::to_string(index_));
    }
    file_name.append(".log");
    stdfs::path dpath(param_.out_path);
    if (!stdfs::exists(dpath)) {
        if (!stdfs::create_directories(dpath)) {
            fprintf(stderr, "create_directories %s err\n", dpath.string().c_str());
            return false;
        }
    }
    dpath /= stdfs::path(file_name);
    fd_ = ::fopen(dpath.string().c_str(), "a");
    if (!fd_) {
        fprintf(stderr, "fopen %s err\n", dpath.string().c_str());
        return false;
    } else {
        fprintf(stderr, "create new log %s ok\n", dpath.string().c_str());
        return true;
    }
}

void XLog::xprintf(uint8 level, const char *func, int line, const char cszFormat[], ...) {
    if (level > param_.level) {
        return;
    }
    std::string prefix;
    prefix.append(Time().format(TimeFormat::YMDHMS_S));
    prefix.append(get_log_level_name(level));
    prefix.append("[");
    prefix.append(func, strlen(func));
    prefix.append(std::to_string(line));
    prefix.append("] ");
    static char line_buf[LINE_BUFF_SIZE];
    va_list ap;
    va_start(ap, cszFormat);
    int len = ::vsnprintf(line_buf, LINE_BUFF_SIZE-1, cszFormat, ap);
    va_end(ap);
    if (len > 0) {
        line_buf[len] = '\n';
        mtx_->lock();
        data_.append(prefix);
        data_.append(line_buf, len+1);
        mtx_->unlock();
    }
}

void XLog::run() {
    time_t now_sec = Time::utc();
    int next = TimeUtil::get_next_reset_time(now_sec, 0);
    int dur = (next - now_sec) * 1000;
    start_timer(dur, false);
    while (wait_for(1000)) {
        mtx_->lock();
        if (data_.size() <= 0) {
            mtx_->unlock();
            continue;
        }
        const char *data = data_.c_str();
        if (param_.option & LogOption::LOG_OPTION_FILE) {
            if  (!fd_) {
                if (!open_new_file()) {
                    ::fprintf(stderr, "%s", "XLog open file err");
                }
            }
            if (fd_) {
                uint32 put_len = 0;
                for (int len; put_len < data_.size(); put_len += len) {
                    len = ::fprintf(fd_, data+put_len);
                }
                ::fflush(fd_);
                file_size_ += data_.size();
                if (file_size_ >= param_.max_size) {
                    index_++;
                    if (!open_new_file()) {
                        ::fprintf(stderr, "%s", "XLog open file err");
                    }
                }
            }
        }
        if (param_.option & LogOption::LOG_OPTION_STDOUT) {
            ::fprintf(stdout, "%s", data);
        }
        data_.clear();
        mtx_->unlock();
    }
}

void XLog::on_timer() {
    //跨天重新生成文件
    index_ = 0;
    mtx_->lock();
    bool ret = open_new_file();
    mtx_->unlock();
    if (!ret) {
        ::fprintf(stderr, "%s", "XLog on_timer open file err");
    } else {
        time_t now_sec = Time::utc();
        int next = TimeUtil::get_next_reset_time(now_sec, 0);
        int dur = (next - now_sec) * 1000;
        start_timer(dur, false);
    }
}
