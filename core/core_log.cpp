#include "core_log.h"
#include <stdio.h>
#include <stdarg.h>
#include "comm_time_util.h"
#include "comm_str_util.h"
#include "core_mutex.h"
#include "core_byte_buffer.h"

#define MAX_BUFF_SIZE 1024


XLog::XLog() {
    fd_ = nullptr;
    lines_ = 0;
    mtx_ = new Mutex(false);
    data_.reserve(MAX_BUFF_SIZE*1024);
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

void XLog::set_prefix(std::string name) {
    param_.prefix = name;
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
    std::string file_name = "./log/" + param_.prefix + "_" +std::to_string(TimeUtil::get_millisecond()) + ".log";
    fd_ = fopen(file_name.c_str(), "w");
    if (!fd_) {
        fprintf(stderr, "%s fopen err\n", file_name.c_str());
        return false;
    } else {
        return true;
    }
}

void XLog::xprintf(uint8 level, const char cszFormat[], ...) {
    if (level > param_.level) {
        return;
    }
    char buf[MAX_BUFF_SIZE];
    va_list ap;
    va_start(ap, cszFormat);
    int len = ::vsnprintf(buf, MAX_BUFF_SIZE, cszFormat, ap);
    va_end(ap);
    if (len > 0) {
        mtx_->lock();
        ++lines_;
        data_.append(buf, len);
        mtx_->unlock();
    }
}

void XLog::run() {
    while (wait_for(2000)) {
        printf("log run\n");
        //ttl log ...
        mtx_->lock();
        if (data_.size() <= 0) {
            mtx_->unlock();
            continue;
        }
        const char *data = data_.c_str();
        if (param_.option & LogOption::XLOG_OPTION_FILE) {
            if  (!fd_) {
                open_new_file();
            }
            if (fd_) {
                uint32 put_len = 0;
                for (int len; put_len < data_.size(); put_len += len) {
                    len = ::fprintf(fd_, "%s", data+put_len);
                }
                fflush(fd_);
                if (lines_ >= param_.max_line) {
                    open_new_file();
                }
            }
        }
        if (param_.option & LogOption::XLOG_OPTION_STDOUT) {
            ::fprintf(stdout, "%s", data);
        }
        if (param_.option & LogOption::XLOG_OPTION_STDERR) {
            ::fprintf(stderr, "%s", data);
        }
        data_.clear();
        mtx_->unlock();
    }
}
