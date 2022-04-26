/*
* Class implementation representing connection
* CSF assignment 5
* Yixin Zheng yzheng67
* Taiming Shi tshi13
*/

#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"
#include<iostream>

Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}


Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  // call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf,fd);
}

void Connection::connect(const std::string &hostname, int port) {
  // call open_clientfd to connect to the server
  // call rio_readinitb to initialize the rio_t object
  const char* host = (char*) hostname.c_str();
  std::string port_string = std::to_string(port);
  char* port_pointer = (char*) port_string.c_str();
  int client_fd = open_clientfd(host,port_pointer);
  m_fd = client_fd;
  rio_readinitb(&m_fdbuf, client_fd);
}

Connection::~Connection() {
  // close the socket if it is open
  Connection::close();
}

bool Connection::is_open() const {
  // return true if the connection is open
  return (m_fd > 0);
}

void Connection::close() {
  // close the connection if it is open
  if(Connection::is_open()) {
    Close(m_fd); 
    m_fd = -1;
  }
 
}

//checks if msg is in correct format for send
bool Connection::valid_send_msg(const Message &msg) {
  std::string tag = msg.tag;
  if (tag != TAG_DELIVERY && tag != TAG_SLOGIN && tag != TAG_RLOGIN && tag != TAG_JOIN && tag != TAG_LEAVE && tag != TAG_SENDALL && tag != TAG_QUIT && tag != TAG_OK) {
    return false;
  }
  int colon_index = msg.data.find(":");
  if (colon_index != -1) {
    return false;
  }
  return true;
}

bool Connection::send(const Message &msg) {
  // send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  if (!valid_send_msg(msg)) {
    m_last_result = INVALID_MSG;
    return false;
  }

  std::string encoded_msg = msg.tag + ":" + trim(msg.data) + "\n"; //encode msg object into proper string

  size_t written_bytes = rio_writen(m_fd,encoded_msg.c_str(),encoded_msg.size()); //write to server
  if (written_bytes != encoded_msg.size()) { //
    m_last_result = EOF_OR_ERROR;
    return false;
  }
  m_last_result = SUCCESS;
  return true;
}
// Receive a message, storing its tag and data in msg
// return true if successful, false if not
// make sure that m_last_result is set appropriately
bool Connection::receive(Message &msg) {
  //Read the message to a buffer
  char buf[Message::MAX_LEN];
  ssize_t read = rio_readlineb(&m_fdbuf, buf, sizeof(buf));
  if(read <= 0) {
    m_last_result = EOF_OR_ERROR;
    return false;
  }

  //Get the length of the received message
  int length = 0;
  int index = 0;
  while(buf[index] != '\n') {
    index++;
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
      msg.data = received.substr(left_index, length);
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
