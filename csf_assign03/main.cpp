#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>
#include "set.h"

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::stoi;
using std::stoul;
using std::map;
using std::vector;
using std::cerr;
using std::getline;


int check_power_of_two(int parameter) {
  if(parameter % 2 != 0) {
    cerr << "set count or block size is not power of 2" << endl;
    return -1;
  }
  return parameter;
}

int check_block_count(int parameter) {
  if(parameter <= 0) {
    cerr << "block count is not a positive number" << endl;
    return -1;
  }
  return parameter;
}
/*
* Params: command stores the command for future opeartion, [0]: 0 is "s", 1 is "l", [1]: tag, [2]: index
* line: a line from trace file
* 
* Given these param, get the save and load command, breakdown the address into tag and index based on set count and block size
*/
void parse_line(unsigned* command, char* line, int set_count, int block_size) {
    command[0] = 1;
    if(line[0] = 's') command[0] = 0;
    int offset_bits = log2(block_size);
    int index_bits = log2(set_count);
    int tag_bits = 32 - offset_bits - index_bits;

    char address[9]; //contains 8 hex characters
    for (int i = 4; i < 12; i++) {
        address[i - 4] = line[i];
    }
    address[8] = '\0';

    unsigned decimal = stoul(address, 0, 16); //convert hexstring to decimal
    string binary_address = std::bitset<32>(decimal).to_string(); //this the binary string format of the address

    char tag[tag_bits + 1];
    tag[tag_bits] = '\0';

    char index[index_bits + 1];
    index[8] = '\0';

    for (int i = 0; i < tag_bits + index_bits; i++) { //store appropriate portions of binary_address to tag, index, offset strings
        if (i < tag_bits) {
            tag[i] = binary_address[i];
        } else {
            index[i - tag_bits] = binary_address[i];
        }
    }
  
        unsigned tag_value = std::bitset<32>(tag).to_ulong();
        unsigned index_value = std::bitset<32>(index).to_ulong();

        command[1] = tag_value;
        command[2] = index_value;
    
}

int main(int argc, char *argv[]){
  //Check command line arguments
  if(argc != 7) {
    cerr << "wrong command line input" << endl;
    return -1;
  }

  //read in command line arguments
  int set_count = check_power_of_two(stoi(argv[1]));
  int block_count = check_block_count(stoi(argv[2]));
  int block_size = check_power_of_two(stoi(argv[3]));
  if (block_size < 4) block_size = -1;
  string write_allocate = argv[4];
  string write_through = argv[5];
  string eviction = argv[6];

  //check if input correctness
  if(set_count == -1 || block_count == -1 || block_size == -1) {
    cerr << "incorrect set_count, block_count, or block_size" << endl;
    return -1;
  }
  if(write_allocate == "no-write-allocate" && write_through == "write-back") {
    cerr << "write-back and no-write-allocate were both specified" << endl;
    return -1;
  }
  if(eviction != "lru" && eviction != "fifo") {
    cerr << "wrong evction command" << endl;
    return -1;
  }


  unsigned loads = 0;
  unsigned stores = 0;
  unsigned load_hits = 0;
  unsigned load_misses = 0;
  unsigned store_hits = 0;
  unsigned store_misses = 0;
  unsigned total_cycles = 0;
  bool isLoad;

  vector <Set> cache; //Representation of a cache, mapping index to sets

  //initializing the cache
  for(int i = 0; i < set_count; i++) {
    cache.push_back(block_count);
  }


  char line[15];
  while (scanf(" %[^\n]", line) == 1){ //reading from input file
    unsigned command[3];
    parse_line(command, line, set_count, block_size);
    unsigned load_save = command[0];
    unsigned tag = command[1];
    unsigned index = command[2];
    if(load_save == 0) {
      bool hit = false;
      Set curr_set = cache[index];
      if(curr_set.find(tag)) hit = true;

      if(hit) {
          store_hits++;
          curr_set.mark_slot_as_used(tag);
          if(write_through == "write-through") {
              total_cycles += (block_size / 4) * 100; //write directly to memory
          }
      } else {
          store_misses++;
          if(curr_set.set_size < curr_set.block_num) {
              total_cycles += curr_set.lru_evict(block_size);
          }
          if(write_allocate == "no-write-allocate") {
              total_cycles += (block_size /4) * 100;
          } else {
              total_cycles++;
              curr_set.store(tag);
          }
      }
    } else if(load_save == 1) {

    }
  }

  cout << "Total cycles:" << total_cycles << endl;
  cout << "Total store hits:" << store_hits << endl;
  cout << "Total store misses:" << store_misses << endl;
}