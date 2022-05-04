/*
* Class implementation representing room
* CSF assignment 5
* Yixin Zheng yzheng67
* Taiming Shi tshi13
*/

#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"
#include <stdio.h>

using std::cout;

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  // TODO: initialize the mutex
  pthread_mutex_init(&lock, NULL);
}

Room::~Room() {
  // TODO: destroy the mutex
  pthread_mutex_destroy(&lock);
}

void Room::add_member(User *user) {
  // TODO: add User to the room
  Guard guard(lock); //locking mutex
  members.insert(user);
}

void Room::remove_member(User *user) {
  // TODO: remove User from the room
  Guard guard(lock); //locking mutex
  members.erase(user);
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // TODO: send a message to every (receiver) User in the room
  Guard guard(lock); //locking mutex
  
  //For all users in the room, send the message
  std::string message = room_name + ":" + sender_username + ":" + message_text;
  for(User *users : members) {
    if (users->username != sender_username) {
    Message *delivery = new Message(TAG_DELIVERY, message);
    users->mqueue.enqueue(delivery);
    }
  }

  cout << sender_username << " sent " << message_text <<"\n";
}
