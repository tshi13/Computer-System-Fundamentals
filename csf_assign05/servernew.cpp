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

struct ConnInfo {
  Connection *conn;
  Server *server;

  ConnInfo(Connection *conn, Server *server) : conn(conn), server(server) { }
  ~ConnInfo() {
    // destroy connection when ConnInfo object is destroyed
    delete conn;
  }
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
      if(cur_room == nullptr) {
        conn->send(Message(TAG_EMPTY, "You should join room before sending message"));
      } else {
      cur_room->broadcast_message(cur_user->username, response.data);
      conn->send(Message(TAG_OK, "ok"));
      }
    } 
    else if(response.tag == TAG_LEAVE) {
      if(cur_room == nullptr) {
        conn->send(Message(TAG_EMPTY, "You should join room before sending message"));
      } else {
      cur_room->remove_member(cur_user);
      conn->send(Message(TAG_OK, "ok"));
      }
    } 
    else if(response.tag == TAG_QUIT) {
      if(cur_room != nullptr) cur_room->remove_member(cur_user);
      break;
    } 
    else if(response.tag == TAG_JOIN) {
      cur_room = server->find_or_create_room(response.data);
      cur_room->add_member(cur_user);
      conn->send(Message(TAG_OK, "ok"));
    }
  }
}

void chat_with_receiver(User* cur_user, ConnInfo* info) {
  Connection *conn = info->conn;
  Server *server = info->server;

  Message response_join_room = Message(TAG_EMPTY, "");
  conn->receive(response_join_room);
  if(response_join_room.tag == TAG_JOIN) {
    Room *cur_room = server->find_or_create_room(response_join_room.data);
    cur_room->add_member(cur_user);
  }
  else conn->send(Message(TAG_ERR, "Receiver should join a server after log in"));
  

  bool receiving = true;
  while(receiving) {  //why do we need a new message???
    Message *msg = new Message(TAG_OK,cur_user->mqueue.dequeue()->data);
    msg ->tag = TAG_OK;
    if (conn->send(*msg) == false) {
      delete msg;
      
      break;
    }
    delete msg;
    
  }
}



void *worker(void *arg) {
  pthread_detach(pthread_self());

  ConnInfo *info_ = static_cast<ConnInfo *>(arg);

  // use a std::unique_ptr to automatically destroy the ConnInfo object
  // when the worker function finishes; this will automatically ensure
  // that the Connection object is destroyed
  std::unique_ptr<ConnInfo> info(info_);

  Message msg;

  if (!info->conn->receive(msg)) {
    if (info->conn->get_last_result() == Connection::INVALID_MSG) {
      info->conn->send(Message(TAG_ERR, "invalid message"));
    }
    return nullptr;
  }

  if (msg.tag != TAG_SLOGIN && msg.tag != TAG_RLOGIN) {
    info->conn->send(Message(TAG_ERR, "first message should be slogin or rlogin"));
    return nullptr;
  }

  std::string username = msg.data;
  if (!info->conn->send(Message(TAG_OK, "welcome " + username))) {
    return nullptr;
  }

  // Just loop reading messages and sending an ok response for each one
  while (true) {
    if (!info->conn->receive(msg)) {
      if (info->conn->get_last_result() == Connection::INVALID_MSG) {
        info->conn->send(Message(TAG_ERR, "invalid message"));
      }
      break;
    }

    if (!info->conn->send(Message(TAG_OK, "this is just a dummy response"))) {
      break;
    }
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
  pthread_mutex_init(&m_lock, nullptr);
}

Server::~Server() {
  pthread_mutex_destroy(&m_lock);
}

bool Server::listen() {
  std::string port = std::to_string(m_port);
  m_ssock = open_listenfd(port.c_str());
  return m_ssock >= 0;
}

void Server::handle_client_requests() {
  assert(m_ssock >= 0);

  while (true) {
    int clientfd = accept(m_ssock, nullptr, nullptr);
    if (clientfd < 0) {
      std::cerr << "Error accepting connection\n";
      return;
    }

    ConnInfo *info = new ConnInfo(new Connection(clientfd), this);

    pthread_t thr_id;
    if (pthread_create(&thr_id, nullptr, worker, static_cast<void *>(info)) != 0) {
      std::cerr << "Could not create thread\n";
      return;
    }
  }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // this function can be called from multiple threads, so
  // make sure the mutex is held while accessing the shared
  // data (the map of room names to room objects)
  Guard g(m_lock);

  Room *room;

  auto i = m_rooms.find(room_name);
  if (i == m_rooms.end()) {
    // room does not exist yet, so create it and add it to the map
    room = new Room(room_name);
    m_rooms[room_name] = room;
  } else {
    room = i->second;
  }

  return room;
}