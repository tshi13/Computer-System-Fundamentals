Sample README.txt

Eventually your report about how you implemented thread synchronization
in the server should go here

Yixin Zheng - yzheng67 : 
receive function in connection.cpp
receiver.cpp
finished first draft of MS2

Taiming Shi - tshi13 : 
the rest of connection.cpp
sender.cpp
message.h
debugged MS2, finished Report

SYNCHRONIZATION REPORT 
CRITICAL SECTIONS, HOW YOU DETERMINED THEM,why you chose the synchronization primitives for each section.:
You should also explain how your critical sections ensure that the synchronization requires are met without introducing synchronization hazards (e.g. race conditions and deadlocks).

CRITICAL SECTIONS:
MessageQueue::enqueue()
MessageQueue::dequeue() 
Room::add_member()
Room::remove_member()
Room::broadcast_message()


MessageQueue::enqueue() and MessageQueue::dequeue() 
We are using a semaphor as well as a mutex in these 2 critical sections. 
The mutex lock helps prevent simultaneously enqueing and dequeing at the same time (race condition), 
or multiple enqueues happening at the same times. 

We use a semaphor to monitor the number of available resources (messages in the MessageQueue).
sem_wait pauses the thread when there are no more available messages in the queue

Room::add_member() and Room::remove_member()
We use a mutex in these 2 critical sections because we want to make sure that users aren't being added or removed
from the room at the same time (race condition). 

Room::broadcast_message()
We also use a mutex in this critical section.
Since the users are contained in a set, and sets are non-atomic, it means that while we are looping through the set
of users, this may be interrupted by adding or removing users in the room, which may cause race conditions. Using a
mutex in this case prevents race conditions from happening.

find_or_create_room()
we could create the same room twice, or we could have unsafe access to the map

