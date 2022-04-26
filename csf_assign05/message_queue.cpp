#include <cassert>
#include <ctime>
#include "message_queue.h"

MessageQueue::MessageQueue() {
  // TODO: initialize the mutex and the semaphore
  int max_messages = 100;
  pthread_mutex_init(&m_lock, NULL);
  sem_init(&m_avail, 0, max_messages);
}

MessageQueue::~MessageQueue() {
  // TODO: destroy the mutex and the semaphore
  pthread_mutex_destroy(&m_lock);
  sem_destroy(&m_avail);
}

void MessageQueue::enqueue(Message *msg) {
  // TODO: put the specified message on the queue
  m_messages.push_back(msg);
  sem_post(&m_avail);
  // be sure to notify any thread waiting for a message to be
  // available by calling 

}

Message *MessageQueue::dequeue() {
  struct timespec ts;

  sem_wait(&m_avail);
  Message *to_return = m_messages.front();
  m_messages.pop_front();
  return to_return;
}
