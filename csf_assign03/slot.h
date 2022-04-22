/*
 * Header file for slot class, cache simulator program
 * CSF Assignment 3
 * Taiming Shi, Ian Zheng
 * tshi13@jh.edu yzheng67@jh.edu
 */

class slot {
public:
  unsigned usage_sequence; //usage ranking amongst all other slots in set, used for LRU
  bool dirty; // if slot is dirty

  slot() {
    usage_sequence = 0; //For LRU
    dirty = false;
  }

  void is_dirty() { //set dirty to true
    dirty = true;
  }
};
