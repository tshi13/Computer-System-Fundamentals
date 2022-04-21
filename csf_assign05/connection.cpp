#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"

Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}


Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf,fd);
}

void Connection::connect(const std::string &hostname, int port) {
  // TODO: call open_clientfd to connect to the server
  // TODO: call rio_readinitb to initialize the rio_t object
  const char* host = (char*) hostname.c_str();
  std::string port_string = std::to_string(port);
  char* port_pointer = (char*) port_string.c_str();
  int client_fd = open_clientfd(host,port_pointer);

  rio_readinitb(&m_fdbuf, client_fd);
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  Connection::close();
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  return (m_fd > 0);
}

void Connection::close() {
  // TODO: close the connection if it is open
  if(Connection::is_open()) {
    Close(m_fd); 
    m_fd = -1;
  }
 
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
}

bool Connection::receive(Message &msg) {
  // TODO: send a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
}
