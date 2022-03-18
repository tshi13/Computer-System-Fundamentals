#include "slot.h"
#include "set.h"

#include <map>

using std::map;
using namespace std;

bool Set::find(unsigned tag) {
    std::map<unsigned, slot>::iterator it;
    it = set.find(tag);
    if (it == set.end()) return false;
    return true;
}

void Set::mark_slot_as_used(unsigned tag) {
    std::map<unsigned, slot>::iterator curr_slot;
    std::map<unsigned, slot>::iterator it;
    curr_slot = set.find(tag);
    unsigned curr_usage_sequence = (curr_slot->second).usage_sequence;
    //If the sequence is the hit slot, update it to 0. For older slots, they temain unchanged.
    //For younger slots, they increment 1. At the end, we still have from 0 to the oldest sequence
    for(it = set.begin(); it != set.end(); it++) {
        if((it->second).usage_sequence < curr_usage_sequence) (it->second).usage_sequence++;
        if((it->second).usage_sequence == curr_usage_sequence) (it->second).usage_sequence = 0;
    }
}

unsigned Set::lru_evict(unsigned block_size) {
    std::map<unsigned, slot>::iterator it;
    unsigned cycle_inc = 0;
    for(it = set.begin(); it != set.end(); it++) {
        if(it->second.usage_sequence == block_num - 1) {
            if(it->second.dirty) cycle_inc = (block_size / 4) * 100;
            set.erase(it);
            break;
        }
    }
    block_num--;
    return cycle_inc;
}

void Set::store(unsigned tag, bool dirty) {
    add_all_sequence();
    slot new_slot;
    if(dirty) new_slot.is_dirty();
    set.insert(pair<unsigned, slot>(tag, new_slot));
    block_num++;
}
