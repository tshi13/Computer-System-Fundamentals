#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"
using std::cerr;

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

  //connect to server
  Connection connection; 
  connection.connect(server_hostname, server_port);
  if (!connection.is_open()) {
    cerr << "connection failed" << "\n";
    return 1;
  } 

  // send slogin message
  connection.send(Message(TAG_SLOGIN,username)); 
  Message send_successful = Message(TAG_EMPTY, "");
  connection.receive(send_successful);
  if (send_successful.tag != TAG_OK) {
  cerr << send_successful.data << "\n";
  return -1;
  }
  

  // loop reading commands from user, sending messages to
  // server as appropriate
  std::string line;
  while(std::getline(std::cin,line)) {
    std::stringstream ss(line);
    std::string command;
    ss >> command;
    if (command[0] != '/') {
      //send message using sendall
      connection.send(Message(TAG_SENDALL,line));
      
      //take feedback from server, check if it's OK
      Message send_successful = Message(TAG_EMPTY, "");
      connection.receive(send_successful);
      if (send_successful.tag != TAG_OK) {
        cerr << send_successful.data << "\n";
      }
    } else if (command == "/quit") { //QUIT PROGRAM
      connection.send(Message(TAG_QUIT,"bye"));
      Message send_successful = Message(TAG_EMPTY, "");
      connection.receive(send_successful);
      if (send_successful.tag != TAG_OK) {
        cerr << send_successful.data << "\n";
      } else {
        return 0; //quit program
      }
    } else if (command == "/join") { //JOIN ROOM
      std::string room_name;
      ss >> room_name;
      connection.send(Message(TAG_JOIN,room_name));
      Message send_successful = Message(TAG_EMPTY, "");
      connection.receive(send_successful);
      if (send_successful.tag != TAG_OK) {
        cerr << send_successful.data << "\n";
      }
    } else if (command == "/leave") { //LEAVE ROOM
      connection.send(Message(TAG_LEAVE, "leave"));
      Message send_successful = Message(TAG_EMPTY, "");
      connection.receive(send_successful);
      if (send_successful.tag != TAG_OK) {
        cerr << send_successful.data << "\n";
      }
    }
  }
  return 0;
}
