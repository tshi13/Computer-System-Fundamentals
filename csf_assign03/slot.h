class slot {
public:
    unsigned usage_sequence;
    bool dirty;

    slot() {
        usage_sequence = 0; //For LRU
        dirty = false;
    }

    void is_dirty() {
        dirty = true;
    }
};