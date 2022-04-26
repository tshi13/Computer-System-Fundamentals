#include <cassert>
#include <ctime>
#include "message_queue.h"
#include "guard.h"
#include <iostream> //FOR DEBUG DELETE LATER
using std::cout; //FOR DEBUG DELETE LATER

MessageQueue::MessageQueue() {
  // TODO: initialize the mutex and the semaphore
  int max_messages = 100;
  pthread_mutex_init(&m_lock, NULL);
  sem_init(&m_avail, 0, 0);
}

MessageQueue::~MessageQueue() {
  // TODO: destroy the mutex and the semaphore
  pthread_mutex_destroy(&m_lock);
  sem_destroy(&m_avail);
}

void MessageQueue::enqueue(Message *msg) {
  // TODO: put the specified message on the queue
  {
    Guard g(m_lock);
    cout << "enqueuing" << "\n";
    m_messages.push_back(msg);
    sem_post(&m_avail);
    // be sure to notify any thread waiting for a message to be
    // available by calling 
  }

}

Message *MessageQueue::dequeue() {
  sem_wait(&m_avail);
  {
    Guard g(m_lock);
    Message *to_return = m_messages.front();
    if(m_messages.empty()) return nullptr;
    m_messages.pop_front();
    cout << "dequeuing" << "\n";
    return to_return;
  }
  
}
