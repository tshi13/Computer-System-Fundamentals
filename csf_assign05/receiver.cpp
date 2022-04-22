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

//Checking if a reponse has error or not
//Return true if there is an error, false otherwise
bool is_err(Message response) {
  if(response.tag != TAG_OK && response.tag != TAG_DELIVERY) return true;
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

  //Connecting to the server
  Connection connection;
  connection.connect(server_hostname, server_port);
   if (!connection.is_open()) {
    cerr << "connection failed" << "\n";
    return 1;
  } 

  //Send login message and check if login is successful
  Message response_login  = Message(TAG_EMPTY, "");
  connection.send(Message(TAG_RLOGIN, username));
  connection.receive(response_login);
  if(is_err(response_login)) {
    cerr << response_login.data << "\n";
    return -1;
  } 

  //Send join room message and check if join room is successful
  Message response_join_room  = Message(TAG_EMPTY, "");
  connection.send(Message(TAG_JOIN, room_name));
  connection.receive(response_join_room);
  if(is_err(response_join_room)) {
    cerr << response_join_room.data << "\n";
    return -1;
  }

  //Loop getting message, check erros, and printto output
  bool receiving = true;
  Message response  = Message(TAG_EMPTY, "");
  while(receiving) {
    receiving = connection.receive(response);
    if (receiving == false) {
      break;
    }
    if(response.tag == TAG_DELIVERY) {
      std::vector<std::string> message = response.split_payload();
      cout << message[1] << ": " << message[2] << "\n";
    } 
    //Break from the loop if there is an error
    else if (is_err(response)){
        break;
      } 
  }
  return 0;
}
