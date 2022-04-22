/*
* Header file for messages
* CSF assignment 5
* Yixin Zheng yzheng67
* Taiming Shi tshi13
*/

#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>
#include <string>
#include<iostream>
#include <sstream>
using namespace std;

struct Message {
  // An encoded message may have at most this many characters,
  // including the trailing newline ('\n'). Note that this does
  // *not* include a NUL terminator (if one is needed to
  // temporarily store the encoded message.)
  static const unsigned MAX_LEN = 255;

  std::string tag;
  std::string data;

  Message() { }

  Message(const std::string &tag, const std::string &data)
    : tag(tag), data(data) { }

  // returns the payload (data) as a vector of strings,
  // split using ':' as the separator
  std::vector<std::string> split_payload() const {
    std::vector<std::string> result;
    // TODO: split the message data into fields separated by ':', add them
    //       to result vector

    // int left_index = 0; 
    // int right_index = -1;
    // for (int i = 0; i<data.length(); i++) {
    //   if (data[i] == ':') {
    //     right_index = i;
    //     left_index = right_index + 1;
    //     std::string temp = data.substr(left_index,right_index);
    //     result.push_back(temp);
    //   }
    // }
    // std::string temp = data.substr(right_index + 1,data.length());

    std::stringstream ss(data);
    string item;

    while (getline (ss, item, ':')) {
        result.push_back(item);
    }
    // cout << "data length: " << data.length() <<"\n";
    // cout << result[0] << "\n";
    // cout << result[1] << "\n";
    // cout << result[2] << "\n";

    return result;
  }
};

// standard message tags (note that you don't need to worry about
// "senduser" or "empty" messages)
#define TAG_ERR       "err"       // protocol error
#define TAG_OK        "ok"        // success response
#define TAG_SLOGIN    "slogin"    // register as specific user for sending
#define TAG_RLOGIN    "rlogin"    // register as specific user for receiving
#define TAG_JOIN      "join"      // join a chat room
#define TAG_LEAVE     "leave"     // leave a chat room
#define TAG_SENDALL   "sendall"   // send message to all users in chat room
#define TAG_SENDUSER  "senduser"  // send message to specific user in chat room
#define TAG_QUIT      "quit"      // quit
#define TAG_DELIVERY  "delivery"  // message delivered by server to receiving client
#define TAG_EMPTY     "empty"     // sent by server to receiving client to indicate no msgs available

#endif // MESSAGE_H
