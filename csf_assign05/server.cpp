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

void *worker(void *arg) {
  pthread_detach(pthread_self());

  // TODO: use a static cast to convert arg from a void* to
  //       whatever pointer type describes the object(s) needed
  //       to communicate with a client (sender or receiver)
  ConnInfo *info = (ConnInfo*) arg;
  Connection client_conn = info->conn;
  
  // TODO: read login message (should be tagged either with
  //       TAG_SLOGIN or TAG_RLOGIN), send response
  Message response_login  = Message(TAG_EMPTY, "");
  client_conn.receive(response_login);
  if(response_login.tag != TAG_SLOGIN && response_login.tag != TAG_RLOGIN) {
    client_conn.send(Message(TAG_ERR, "Log in as sender or receiver! Incorrect tag"));
  } else {
    client_conn.send(Message(TAG_OK, "Log in successful"));
  }

  // TODO: depending on whether the client logged in as a sender or
  //       receiver, communicate with the client (implementing
  //       separate helper functions for each of these possibilities
  //       is a good idea)
  if(response_login.tag == TAG_SLOGIN) chat_with_sender();
  else chat_with_receiver();
  
  //Create user object in client thread to track pending messages and resgiter it to a Room when the client sends a join request
  return nullptr;
}

void chat_with_sender() {
  bool receiving = true;
  while(receiving) {

  }
}

void chat_with_receiver() {
  bool receiving = true;
  while(receiving) {
    
  }
}

}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // TODO: initialize mutex
}

Server::~Server() {
  // TODO: destroy mutex
}

bool Server::listen() {
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not
  m_ssock = open_listenfd((char*) m_port);
  if (m_ssock < 0) return false;
  return true;
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  while(1) {
    int clientfd = Accept(m_ssock, NULL, NULL);
    if (clientfd < 0) {
			//TODO: Error with accepting a connection from a client
		}

    ConnInfo *info = new ConnInfo;
    info->conn = new Connection(clientfd);
    info->server = this;
    pthread_t thr;
    if (pthread_create(&thr, NULL, worker, static_cast<void*>(info)) != 0) {
      /* error creating thread */
    }
    close(clientfd);
  }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
}
