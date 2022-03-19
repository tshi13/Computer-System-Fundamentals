#ifndef SET_H
#define SET_H

#include <map>
#include "slot.h"

using std::map;
using namespace std;

class Set {

private:
    map<unsigned, slot> set;
    void add_all_sequence() {
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
    unsigned set_size;
    unsigned block_num;
    bool find(unsigned tag);
    void mark_slot_as_used(unsigned tag);
    unsigned lru_evict();
    void store(unsigned tag, bool dirty);
    void mark_slot_dirty(unsigned tag);

};
#endif
