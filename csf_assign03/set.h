/*
 * Header file for set class, cache simulator program
 * CSF Assignment 3
 * Taiming Shi, Ian Zheng
 * tshi13@jh.edu yzheng67@jh.edu
 */

#ifndef SET_H
#define SET_H

#include <map>
#include <string>
#include "slot.h"

using std::map;
using namespace std;
using std::string;


class Set {

private:
  map<unsigned, slot> set; //set representation, key: tag, value: slot object
  void add_all_sequence() { // add 1 to all the usage sequence for slots in the set, for LRU operation
    std::map<unsigned, slot>::iterator it;
    for (it = set.begin(); it != set.end(); it++) {
      it->second.usage_sequence++;
    }
  }

public:
  Set(unsigned set_size) {
    this->set_size = set_size;
    block_num = 0;
  }
  unsigned set_size; //max size of the set
  unsigned block_num; // number of blocks in set

  /*
  * searches if a tag is present in the set
  * params: tag that we want to search for
  * returns true if tag is found in set, false otherwise
  */
  bool find(unsigned tag); 

  /*
  * update all usage sequences for slots in set when accessing a slot
  * params: tag that we have accessed/used
  * returns void
  */
  void mark_slot_as_used(unsigned tag);

  /*
  * evict a block from set when it is full, using LRU
  * params: null
  * returns true if the block we evicted was dirty, false otherwise
  */
  bool lru_evict(); 

  bool fifo_evict();



  /*
  * store tag into set
  * params: tag (tag we want to store), dirty (whether the block is dirty or not)
  * returns void
  */
  void store(unsigned tag, bool dirty, string eviction, unsigned load_save); 

  /*
  * set the slot containing tag to dirty
  * params: tag (tag we want to mark as dirty)
  * returns void
  */
  void mark_slot_dirty(unsigned tag);
};
#endif
