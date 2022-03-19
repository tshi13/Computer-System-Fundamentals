#include "set.h"

#include <map>

using std::map;
using namespace std;

bool Set::find(unsigned tag) { //searches if a tag is present in the set
    std::map<unsigned, slot>::iterator it;
    it = set.find(tag);
    if (it == set.end()) return false;
    return true;
}

void Set::mark_slot_as_used(unsigned tag) { ////update all usage sequences for slots in set when accessing a slot
    std::map<unsigned, slot>::iterator curr_slot;
    std::map<unsigned, slot>::iterator it;
    curr_slot = set.find(tag);
    unsigned curr_usage_sequence = (curr_slot->second).usage_sequence;
    //If the sequence is the hit slot, update it to 0. For older slots, they remain unchanged.
    //For younger slots, they increment 1. At the end, we still have from 0 to the oldest sequence
    for(it = set.begin(); it != set.end(); it++) {
        if((it->second).usage_sequence < curr_usage_sequence) (it->second).usage_sequence++;
        else if((it->second).usage_sequence == curr_usage_sequence) (it->second).usage_sequence = 0;
    }
}

bool Set::lru_evict() { //evict a block from set when it is full, using LRU
    std::map<unsigned, slot>::iterator it;
    bool evict_hit = false; //whether the slot we removed is dirty
    for(it = set.begin(); it != set.end(); it++) { //iterate through all the slots of the set
        if(it->second.usage_sequence == block_num - 1) { // find the slot with highest usage sequence (least used)
            if(it->second.dirty) evict_hit = true ; // if it's dirty, we want to store that back to main memory
            set.erase(it->first); //remove slot
            block_num--; //decrease block number count
            break;
        }
    }
    return evict_hit;
}

void Set::store(unsigned tag, bool dirty) { //store new slot with tag into set
    add_all_sequence(); //add one to usage sequences for all the other slots in set
    slot new_slot;
    if(dirty) new_slot.is_dirty(); //checks if new slot should be set to dirty
    set.insert(pair<unsigned, slot>(tag, new_slot)); //insert new tag-slot pair into set
    block_num++;
}

void Set::mark_slot_dirty(unsigned int tag) { //set the slot containing tag to dirty
    std::map<unsigned, slot>::iterator it;
    it = set.find(tag); //find slot, set to dirty
    it->second.dirty = true;
}
