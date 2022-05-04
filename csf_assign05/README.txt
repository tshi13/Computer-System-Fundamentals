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
Room *Server::find_or_create_room()

We used mutexes (within the Guard class) and semephores to achieve synchronization in this assignment.
The Guard objects allow us to unlock the mutexes automatically once the critical section goes out of scope.


MessageQueue::enqueue() and MessageQueue::dequeue() 
We set these 2 critical sections because they share the same queue data structure. When 1 read/write operation
is happening, we don't want the data structure to be modified in any other way. We also want to keep track of 
the number of available resources in the message queue.We are using semaphores and mutexes in these 2 critical sections. 

The mutex lock helps prevent simultaneously enqueing and dequeing at the same time (race condition), 
or multiple enqueues happening at the same time. 

We use a semaphore to monitor the number of available resources (messages in the MessageQueue).
sem_post increases the semaphore value, and notifies threads of a new available message.
sem_wait pauses the thread when there are no more available messages in the queue.


Room::add_member() and Room::remove_member()
We use a mutex in these 2 critical sections because we want to make sure that users aren't being added or removed
from the room at the same time (race condition). The shared data structure is a Set, and with the help of mutexes
in the Guard object, we can ensure that add_member(), remove_member(), and broadcast_message() are "mutually exclusive"
and won't interfere with each other. 


Room::broadcast_message()
We also use a mutex in this critical section.
Since the users are contained in a set, and sets are non-atomic, it means that while we are looping through the set
of users, this may be interrupted by adding or removing users in the room, which may cause race conditions. Using a
mutex in this case prevents race conditions from happening.

find_or_create_room()
We use a mutex in this critical section.
Without a mutex, we could create the same room twice, or we could have unsafe access to the map. With the mutex lock
in place, we would be able to avoid such issues since we would only be searching/creating rooms one at a time.

Therefore, with the usage of mutexes and semaphores, race conditions are avoided because operations on shared data 
structures within each of the critical sections will be "locked" and so there won't be data corruption caused by 
other threads trying to simultaneously access or make modifications to the data structure. Deadlocks are also prevented
by the usage of the Guard objects, which automatically unlock themselves once the block goes out of scope, so other 
operations may access the shared data structure and won't be left waiting. 

