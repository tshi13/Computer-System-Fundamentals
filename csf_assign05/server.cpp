#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"

using std::cout;
////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions
struct ConnInfo {
  Connection *conn;
  Server *server;
};

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace {
void chat_with_sender(User *cur_user, ConnInfo* info) {
  bool receiving = true;
  Connection *conn = info->conn;
  Server *server = info->server;
  Room *cur_room = nullptr;

  while(receiving) {
    Message response = Message(TAG_EMPTY, "");
    conn->receive(response);
    if(response.tag == TAG_SENDALL) {
      if(cur_room == nullptr) conn->send(Message(TAG_EMPTY, "You should join room before sending message"));
      cur_room->broadcast_message(cur_user->username, response.data);
    } 
    else if(response.tag == TAG_LEAVE) {
      if(cur_room == nullptr) conn->send(Message(TAG_EMPTY, "You should join room before sending message"));
      cur_room->remove_member(cur_user);
    } 
    else if(response.tag == TAG_QUIT) {
      if(cur_room != nullptr) cur_room->remove_member(cur_user);
      break;
    } 
    else if(response.tag == TAG_JOIN) {
      cur_room = server->find_or_create_room(response.data);
      cur_room->add_member(cur_user);
      conn->send(Message(TAG_OK, "Join room successful"));
      cout << cur_user->username << " joined " << cur_room->get_room_name() << "\n";
    }
  }
}

void chat_with_receiver(User* cur_user, ConnInfo* info) {
  Connection *receiver_conn = info->conn;
  Server *server = info->server;

  Message response_join_room;
  receiver_conn->receive(response_join_room);
  if(response_join_room.tag == TAG_JOIN) {
    Room *cur_room = server->find_or_create_room(response_join_room.data);
    cur_room->add_member(cur_user);
    receiver_conn->send(Message(TAG_OK, "Join room successful"));
    cout << cur_user->username << " joined " << cur_room->get_room_name() << "\n";
  }
  else receiver_conn->send(Message(TAG_ERR, "Receiver should join a server after log in"));
  

  bool receiving = true;
  while(receiving) {
    Message *msg = cur_user->mqueue.dequeue();
    if(msg != nullptr){
      cout << "sending message to receiver" << "\n";
      //delivery:party:alice:payload
      receiver_conn->send(*msg);
      delete msg;
    }
  }
}

void *worker(void *arg) {
  pthread_detach(pthread_self());

  // TODO: use a static cast to convert arg from a void* to
  //       whatever pointer type describes the object(s) needed
  //       to communicate with a client (sender or receiver)
  ConnInfo *info = static_cast<ConnInfo*>(arg);
  Connection *client_conn = info->conn;
  
  // TODO: read login message (should be tagged either with
  //       TAG_SLOGIN or TAG_RLOGIN), send response
  Message response_login  = Message(TAG_EMPTY, "");
  client_conn->receive(response_login);
  if(response_login.tag != TAG_SLOGIN && response_login.tag != TAG_RLOGIN) {
    client_conn->send(Message(TAG_ERR, "Log in as sender or receiver! Incorrect tag"));
  } else {
    cout << "login successful. Username: " << response_login.data << "\n"; //DELETE
    client_conn->send(Message(TAG_OK, "Log in successful"));
  }
  //Instantiate a user
  User *cur_user = new User(response_login.data);
  // TODO: depending on whether the client logged in as a sender or
  //       receiver, communicate with the client (implementing
  //       separate helper functions for each of these possibilities
  //       is a good idea)
  if(response_login.tag == TAG_SLOGIN) {
    chat_with_sender(cur_user, info);
  } else {
    chat_with_receiver(cur_user, info);
  }
  
  return nullptr;
}

}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // TODO: initialize mutex
  pthread_mutex_init(&m_lock, NULL);

}

Server::~Server() {
  // TODO: destroy mutex
  pthread_mutex_destroy(&m_lock);
}

bool Server::listen() {
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not
  std::string port_string = std::to_string(m_port);
  m_ssock = open_listenfd(port_string.c_str());
  if (m_ssock < 0) return false;
  return true;
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  //Question: Lock here???? I don't think so
  while(1) {
    int clientfd = Accept(m_ssock, NULL, NULL);
    if (clientfd < 0) {
			//TODO: Error with accepting a connection from a client
		}

    ConnInfo *info = new ConnInfo;
    info->conn = new Connection(clientfd); //On the heap
    info->server = this;
    pthread_t thr;
    if (pthread_create(&thr, NULL, worker, static_cast<void*>(info)) != 0) {
      /* error creating thread */
    }
    //close(clientfd);
  }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
  //Question: Probably lock here
  {
    Guard g(m_lock);
    auto room_iterator = m_rooms.find(room_name);
    if(room_iterator == m_rooms.end()) {
      Room *new_room = new Room(room_name);
      m_rooms[room_name] = new_room;
      return new_room;
    }
    return room_iterator->second;
  }
}
