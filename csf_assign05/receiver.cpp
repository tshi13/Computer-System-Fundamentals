#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

using std::cerr;
using std::cout;

bool is_err(Message response) {
  if(response.tag != TAG_OK) return true;
  return false;
}
int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];

  Connection conn;

  // TODO: connect to server
  Connection connection;
  connection.connect(server_hostname, server_port);
   if (!connection.is_open()) {
    cerr << "connection failed" << "\n";
    return 1;
  } 


  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
  Message response_login  = Message(TAG_EMPTY, "");
  Message response_join_room  = Message(TAG_EMPTY, "");
  connection.send(Message(TAG_RLOGIN, username));
  connection.receive(response_login);
  if(is_err(response_login)) {
    cerr << response_login.data << "\n";
    return -1;
  } 

  // //TEST: DELETE LATER!
  // if(response_login.tag == TAG_OK) cerr << "login successful! THIS IS FOR DEBUG. DELETE LATER!" << "\n";


  connection.send(Message(TAG_JOIN, room_name));
  connection.receive(response_join_room);
  if(is_err(response_join_room)) {
    cerr << response_join_room.data << "\n";
    return -1;
  }

  // //TEST: DELETE LATER!
  // if(response_join_room.tag == TAG_OK) cerr << "join room successful! THIS IS FOR DEBUG. DELETE LATER!" << "\n";

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  bool receiving = true;
  Message response  = Message(TAG_EMPTY, "");
  while(receiving) {
    receiving = connection.receive(response);
    if(response.tag == TAG_DELIVERY) {
      std::vector<std::string> message = response.split_payload();
      // cout << "results for message" << "\n";
      cout << message[1] << ": " << message[2] << "\n";
      
    } else if (is_err(response)) cerr << response.data << "\n"; //Print out err if server sends back err
  }
  return 0;
}
