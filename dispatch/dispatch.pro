TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += \
    /usr/include/google \
    /usr/include/jsoncpp \
    /usr/include/openssl \
    $$PWD/../include \
    $$PWD/../core \
    $$PWD/../common \
    $$PWD/../net \
    $$PWD/../proto \
    $$PWD/../message \


LIBS += \
    /usr/lib/x86_64-linux-gnu/libprotobuf.a \
    /usr/lib/x86_64-linux-gnu/libprotobuf-lite.a    \
    /usr/lib/x86_64-linux-gnu/libprotoc.a   \
    /usr/lib/x86_64-linux-gnu/libjsoncpp.a  \
    /usr/lib/x86_64-linux-gnu/libpthread.so  \
    /usr/lib/x86_64-linux-gnu/libssl.a \
    /usr/lib/x86_64-linux-gnu/libcrypto.a \


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
    ../common/comm_str_util.cpp \
    ../common/comm_time_util.cpp \
    ../common/comm_util.cpp \
    ../core/core_byte_buffer.cpp \
    ../core/core_log.cpp \
    ../core/core_md5.cpp \
    ../core/core_murmurhash.cpp \
    ../core/core_sha.cpp \
    ../core/core_singleton.cpp \
    ../core/core_timer.cpp \
    ../net/net_base_message.cpp \
    ../net/net_connection.cpp \
    ../net/net_epoll.cpp \
    ../net/net_event.cpp \
    ../net/net_event_module.cpp \
    ../net/net_http_message.cpp \
    ../net/net_parser.cpp \
    ../net/net_pb_message.cpp \
    ../net/net_pb_parser.cpp \
    ../net/net_reactor.cpp \
    ../net/net_session.cpp \
    ../net/net_sock_addr.cpp \
    ../net/net_sock_channel.cpp \
    ../net/net_socket.cpp \
    ../net/net_tcp_server.cpp \
    ../proto/msg_cd.pb.cc \
    ../proto/msg_def.pb.cc


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
    ../common/comm_inc.h \
    ../common/comm_str_util.h \
    ../common/comm_time_util.h \
    ../common/comm_util.h \
    ../core/core_byte_buffer.h \
    ../core/core_log.h \
    ../core/core_md5.h \
    ../core/core_murmurhash.h \
    ../core/core_sha.h \
    ../core/core_singleton.h \
    ../core/core_timer.h \
    ../net/net_base_message.h \
    ../net/net_connection.h \
    ../net/net_epoll.h \
    ../net/net_event.h \
    ../net/net_event_module.h \
    ../net/net_http_message.h \
    ../net/net_msg_def.h \
    ../net/net_parser.h \
    ../net/net_pb_message.h \
    ../net/net_pb_parser.h \
    ../net/net_reactor.h \
    ../net/net_session.h \
    ../net/net_sock_addr.h \
    ../net/net_sock_channel.h \
    ../net/net_socket.h \
    ../net/net_tcp_server.h \
    ../proto/msg_cd.pb.h \
    ../proto/msg_def.pb.h \
    frame/test.hpp


DISTFILES += \
    $$PWD/config/config.json \
    $$PWD/../proto/*.proto \ \
    ../proto/msg_def.proto

