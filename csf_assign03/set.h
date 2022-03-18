#include <map>
#include "slot.h"

using std::map;


class Set {
    map<unsigned, slot> set;


public:
    unsigned set_size;
    unsigned block_num;

    Set(unsigned set_size) {
       this->set_size = set_size;
       block_num = 0;
    }

    bool find(unsigned tag) {
        if (set.find(tag) == set.end()) return false;
        return true;
    }

    void mark_slot_as_used(unsigned tag) {
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

    void mark_as_dirty(unsigned tag) {
        std::map<unsigned, slot>::iterator curr_slot;
        curr_slot = set.find(tag);
        curr_slot->second.dirty = true;
    }

    int lru_evict() {
        std::map<unsigned, slot>::iterator it;
        int cycle_number = 0;
        for(it = set.begin(); it != set.end(); it++) {
            if(it->second.usage_sequence == set_size - 1) {
                if(it->second.dirty) cycle_number = 100;
                set.erase(it);
                break;
            }
        }
        return cycle_number;
    }

    void add_all_sequence() {
        std::map<unsigned, slot>::iterator it;
        for(it = set.begin(); it != set.end(); it++) {
            it->second.usage_sequence++;
        }
    }
    void store(unsigned tag) {
        add_all_sequence();
        slot new_slot;
        new_slot.is_dirty();
        set.insert({tag, new_slot});
        block_num++;
    }
};