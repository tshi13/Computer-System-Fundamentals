#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname;
  int server_port;
  std::string username;

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);
  username = argv[3];

  // TODO: connect to server
  Connection connection; //
  connection.connect(server_hostname, server_port);
  connection.is_open(); //check print error message and exit

  // TODO: send slogin message
  connection.send(Message(TAG_SLOGIN,username));
  

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  std::string line;
  while(std::getline(std::cin,line)) {
    std::stringstream ss(line);
    std::string command;
    if (command[0] != '/') {
      //send message using sendall
    } else if (command == "/quit") {
      //quit
    } else if (command == "/join") {
      std::string room_name;
      ss >> room_name;
      //send join message
      connection.send(Message(TAG_JOIN,room_name));
    }
  }


  return 0;
}
