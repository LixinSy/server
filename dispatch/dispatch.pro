TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += \
    /usr/include/google \
    /usr/include/jsoncpp \
    $$PWD/../include \
    $$PWD/../core \
    $$PWD/../common \
    $$PWD/../proto \
    $$PWD/../message \


LIBS += \
    /usr/lib/x86_64-linux-gnu/libprotobuf.a \
    /usr/lib/x86_64-linux-gnu/libprotobuf-lite.a    \
    /usr/lib/x86_64-linux-gnu/libprotoc.a   \
    /usr/lib/x86_64-linux-gnu/libjsoncpp.a  \
    /usr/lib/x86_64-linux-gnu/libpthread.so  \


SOURCES += \
#    $$PWD/../core/*.cpp \
#    $$PWD/../common/*.cpp \
#    $$PWD/../proto/*.cc  \
#    $$PWD/../message/*.cc \
#    $$PWD/frame/*.cpp \
#    $$PWD/cache/*.cpp \
#    $$PWD/context/*.cpp \
    $$PWD/../core/core_thread.cpp \
    $$PWD/../core/core_thread_pool.cpp \
    $$PWD/../core/core_channel.cpp \
    $$PWD/../core/core_mutex.cpp \
    $$PWD/../core/core_cond.cpp \
    $$PWD/frame/main.cpp \
    ../common/comm_time_util.cpp \
    ../core/core_timer.cpp


HEADERS += \
#    $$PWD/../core/*.h \
#    $$PWD/../common/*.h \
#    $$PWD/../proto/*.h  \
#    $$PWD/../message/*.h \
#    $$PWD/frame/*.h \
#    $$PWD/cache/*.h \
#    $$PWD/context/*.h \
    $$PWD/../core/core_thread.h \
    $$PWD/../core/core_thread_pool.h \
    $$PWD/../core/core_channel.h \
    $$PWD/../core/core_mutex.h \
    $$PWD/../core/core_cond.h \
    $$PWD/../common/macro.h \
    $$PWD/../common/def.h \
    ../common/comm_time_util.h \
    ../core/core_timer.h \
    frame/test.hpp


DISTFILES += \
    $$PWD/config/config.json \
    $$PWD/../proto/*.proto \

