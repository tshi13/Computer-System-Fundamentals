/*
* Representing receiver of a connection
* CSF assignment 5
* Yixin Zheng yzheng67
* Taiming Shi tshi13
*/

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

  // TODO: connect to server
  Connection connection; //
  connection.connect(server_hostname, server_port);
  if (!connection.is_open()) {
    cerr << "connection failed" << "\n";
    return 1;
  } 

  // TODO: send slogin message
  connection.send(Message(TAG_SLOGIN,username)); //deal with usneranem 
  Message temp = Message(TAG_EMPTY, "");
  connection.receive(temp);
  if (temp.tag != TAG_OK) {
  //we have an issue
  cerr << temp.data << "\n";
  return -1;
  }
  

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  std::string line;
  while(std::getline(std::cin,line)) {
    std::stringstream ss(line);
    std::string command;
    ss >> command;
    if (command[0] != '/') {
      //send message using sendall
      connection.send(Message(TAG_SENDALL,line));
      
      //take feedback from server, check if it's OK
      Message temp1 = Message(TAG_EMPTY, "");
      connection.receive(temp1);
      if (temp1.tag != TAG_OK) {
        //we have an issue
        cerr << temp1.data << "\n";
      }
    } else if (command == "/quit") {
      //quit
      connection.send(Message(TAG_QUIT,"bye"));
      Message temp2 = Message(TAG_EMPTY, "");
      connection.receive(temp2);
      if (temp2.tag != TAG_OK) {
        //we have an issue
        cerr << temp2.data << "\n";
      } else {
        return 0;
      }

    } else if (command == "/join") {
      std::string room_name;
      ss >> room_name;
      //send join message
      connection.send(Message(TAG_JOIN,room_name));
      Message temp3 = Message(TAG_EMPTY, "");
      connection.receive(temp3);
      if (temp3.tag != TAG_OK) {
        //we have an issue
        cerr << temp3.data << "\n";
      }
    } else if (command == "/leave") {
      connection.send(Message(TAG_LEAVE, "leave"));
      Message temp2 = Message(TAG_EMPTY, "");
      connection.receive(temp2);
      if (temp2.tag != TAG_OK) {
        //we have an issue
        cerr << temp2.data << "\n";
      }
    }
  }
  return 0;
}
