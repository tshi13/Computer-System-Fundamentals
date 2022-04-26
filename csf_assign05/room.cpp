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
  members.insert(user);
}

void Room::remove_member(User *user) {
  // TODO: remove User from the room
  members.erase(user);
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // TODO: send a message to every (receiver) User in the room
  { 
    Guard g(lock);
    auto set_iterator = members.begin();
    while(set_iterator != members.end()) {
      if((*set_iterator)->username != sender_username) {
        Message *delivery = new Message(TAG_DELIVERY, message_text);
        (*set_iterator)->mqueue.enqueue(delivery);
      }
      set_iterator++;
    }
  }

  cout << sender_username << " sent " << message_text <<"\n";
}
