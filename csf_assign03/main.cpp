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

/*
* params: parameter: 
* function that checks if parameter is a power of two
* returns: -1 if not a power of 2, returns parameter otherwise
*/
int check_power_of_two(int parameter) {
  if(parameter != 1 && parameter % 2 != 0) {
    cerr << "set count or block size is not power of 2" << endl;
    return -1;
  }
  return parameter;
}

/*
* Params: parameter to be checked
*
* function that checks if parameter is a valid block count
* returns: -1 if non positive, returns parameter otherwise
*/
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
    command[0] = 1; //set 0/1 for command[0] based on save or load
    if(line[0] == 's') command[0] = 0;
    bool is_fully_associative = false; //if cache is fully_associative
    int offset_bits = log2(block_size); //num of bits for offset and index
    int index_bits = log2(set_count);
    if (index_bits == 0){ // sets cache to fully_associative if index_bits == 0
      is_fully_associative = true;
    }

    char address[9]; //contains 8 hex characters
    int i = 4;
    for (; i < 12; i++) { //extract address part in the input trace file
        if (line[i] != ' '){
        address[i - 4] = line[i];
        } else {
          break;
        }
    }
    address[i-4] = '\0';

    unsigned decimal = stoul(address, 0, 16); //convert hexstring to decimal
    decimal = decimal >> offset_bits; //removes offset bits from address

    int index = 0;
    if (!is_fully_associative){ //if not fully associative, we do bit shifting to extract index part 
      int helper_number = pow(2,index_bits) - 1;
      index = decimal & helper_number;
      decimal = decimal >> index_bits;
    }
    
    int tag = decimal; // remaining part of decimal is now the tag part
    command[1] = tag; //put tag and index in correct positions in command
    command[2] = index;

}

int main(int argc, char *argv[]){
  //Check command line arguments
  if(argc != 7) {
    cerr << "wrong command line input" << endl;
    return -1;
  }

  //read in command line arguments
  int set_count = check_power_of_two(stoi(argv[1])); //number of sets in cache
  int block_count = check_block_count(stoi(argv[2])); // number of blocks in each set
  int block_size = check_power_of_two(stoi(argv[3])); // size of each block
  if (block_size < 4) block_size = -1;
  string write_allocate = argv[4]; //set remaining command input arguments
  string write_through = argv[5];
  string eviction = argv[6];

  //check input correctness
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

  // initialize all profiling data
  unsigned loads = 0;
  unsigned stores = 0;
  unsigned load_hits = 0;
  unsigned load_misses = 0;
  unsigned store_hits = 0;
  unsigned store_misses = 0;
  unsigned total_cycles = 0;

  vector <Set> cache; //Representation of a cache, mapping index to sets

  //initializing the cache
  for(int i = 0; i < set_count; i++) {
    cache.push_back(Set(block_count));
  }


  char line[15];
  while (scanf(" %[^\n]", line) == 1){ //reading from input file
    unsigned command[3];
    parse_line(command, line, set_count, block_size); // send information from each line in file to command 
    unsigned load_save = command[0];
    unsigned tag = command[1];
    unsigned index = command[2];

    //STORING
    if(load_save == 0) { 
      bool hit = false;
      stores++;
      if(cache[index].find(tag)) hit = true; // see if we can find a slot in set with same tag
      //STORE HIT
      if(hit) {
          store_hits++;
          cache[index].mark_slot_as_used(tag); //update respective slot in set as used
          if(write_through == "write-through") { //write directly to memory
              total_cycles += 101;
          } else { //write-back
              total_cycles++;
              cache[index].mark_slot_dirty(tag); //if it's write-back we need to set dirty
          }
      } else { //STORE MISS
          store_misses++;
          //Write allocate
          if(write_allocate == "write-allocate") { //when we are using write-allocate
              total_cycles += ((block_size / 4) * 100); //Load happens anyway
              if(cache[index].set_size == cache[index].block_num) { //set is full, start eviction
                  if(cache[index].lru_evict()){ //evict slot, if slot is dirty, will need extra total cycles
                    total_cycles += (block_size / 4) * 100;
                  }
              }
              cache[index].store(tag, false); //store information
              if(write_through == "write-through") { //write-through, update total cycles
              total_cycles += 100;
              total_cycles++;
              } else { // write-back, update total cycles
              total_cycles++;
              cache[index].mark_slot_dirty(tag);
              }
          } else { // no write-allocate, update total cycles
            total_cycles += 100;
          }

          
      }
    } else if(load_save == 1) { //LOADING
        loads++;
        bool hit = false;
        if(cache[index].find(tag)) hit = true;
        if(hit) { //LOAD HIT, update total cycles
            load_hits++;
            total_cycles++;
            cache[index].mark_slot_as_used(tag); //accessed slot, update all usage sequences in set
        } else {
            load_misses++;
            total_cycles += (block_size / 4) * 100;
            total_cycles += 1;
            if(cache[index].set_size == cache[index].block_num) { //when set is full, need to evict
                if(cache[index].lru_evict()){ // if the block we evicted was dirty, we need to update total_cycle
                  total_cycles += (block_size / 4) * 100;
                }
            }
            cache[index].store(tag, false); // store information in slot
        }
    }
  }
  //output cache simulation results
  cout << "Total loads:" << loads << endl;
  cout << "Total stores:" << stores << endl;
  cout << "Load hits:" << load_hits << endl;
  cout << "Load misses:" << load_misses << endl;
  cout << "Store hits:" << store_hits << endl;
  cout << "Store misses:" << store_misses << endl;
  cout << "Total cycles:" << total_cycles << endl;
}
