#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

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
  m_fd = client_fd;
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

//checks if msg is in correct format for send
bool Connection::valid_send_msg(const Message &msg) {
  std::string tag = msg.tag;
  if (tag != TAG_SLOGIN && tag != TAG_JOIN && tag != TAG_LEAVE && tag != TAG_SENDALL && tag != TAG_QUIT ) {
    return false;
  }
  int colon_index = msg.data.find(":");
  if (colon_index != -1) {
    return false;
  }
  return true;
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  if (!valid_send_msg(msg)) {
    m_last_result = INVALID_MSG;
    return false;
  }

  std::string encoded_msg = msg.tag + ":" + trim(msg.data) + "\n"; //encode msg object into proper string

  int written_bytes = rio_writen(m_fd,encoded_msg.c_str(),encoded_msg.size());
  if (written_bytes != encoded_msg.size()) {
    m_last_result = EOF_OR_ERROR;
    return false;
  }

  m_last_result = SUCCESS;
  return true;
}

bool Connection::receive(Message &msg) {
  // TODO: receive a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately

  
  char buf[Message::MAX_LEN];
  ssize_t read = rio_readlineb(&m_fdbuf, buf, sizeof(buf));
  if(read <= 0) {
    m_last_result = EOF_OR_ERROR;
    return false;
  }

  //Get the length of the received message
  int length = 0;
  int index = 0;
  while(buf[index] != 0 || index < 1000) {
    index ++;
    length++;
  }

  //Convert buffer to string
  std::string received = "";
  for (int i = 0; i < length; i++) {
      received = received + buf[i];
  }

  //split the received message to tag and paylod, and put them into msg
  int left_index = 0; 
  int right_index = -1;
  for (int i = 0; i < length; i++) {
    if (buf[i] == ':') {
      right_index = i;
      left_index = right_index + 1;
      msg.tag = received.substr(0, right_index);
      msg.data = received.substr(left_index , length - i - 1);
      break;
    }
    //Can't split the message into tag and data
    if(i == length - 1) {
      m_last_result = INVALID_MSG;
      return false;
    }
  }

  m_last_result = SUCCESS;
  return true;


}
