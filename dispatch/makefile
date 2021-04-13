
CXX = g++
CC = gcc

CFLAGS += -g -O0 -Wall -fPIC
CXXFLAGS += -std=c++11 -Wall -g -pipe -rdynamic -fno-strict-aliasing -Wno-unused-function -Wno-sign-compare -fpermissive -Wno-invalid-offsetof

INC += -I/usr/include/google -I/usr/include/jsoncpp -I./../include
INC += -I./../core -I./../common -I./../proto -I./../message 
INC += -I./cache -I./context -I./frame

LIB += /usr/lib/x86_64-linux-gnu/libprotobuf.a
LIB += /usr/lib/x86_64-linux-gnu/libprotobuf-lite.a
LIB += /usr/lib/x86_64-linux-gnu/libprotoc.a
LIB += /usr/lib/x86_64-linux-gnu/libjsoncpp.a
LIB += /usr/lib/x86_64-linux-gnu/libpthread.so  

CPP_FILES += $(wildcard ../core/*.cpp) $(wildcard ../common/*.cpp) $(wildcard ../proto/*.cc) $(wildcard ../message/*.cc)
CPP_FILES += $(wildcard ./cache/*.cpp) $(wildcard ./context/*.cpp) $(wildcard ./frame/*.cpp)

OBJ = $(patsubst %.cpp,%.o,$(CPP_FILES))

TARGET = dispatchserver

all: $(TARGET)

$(TARGET): $(OBJ)
	@echo -e Linking $@
	@$(CXX) -o $@ $^ $(CFLAGS) $(LIB)
	#install $(TARGET) . 
%.o: %.cpp
	@echo -e Compling $<
	@$(CXX) $(CXXFLAGS) -c -o $@ $< $(INC) 
%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $< $(INC)
clean:
	@rm -f $(OBJ) $(TARGET)



