#include "net_connection.h"


Connection::Connection() {

}

Connection::~Connection() {
    delete sock_;
}
