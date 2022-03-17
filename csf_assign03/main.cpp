#include <string>
#include <iostream>
#include <map>
#include <vector>

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::stoi;
using std::map;
using std::vector;
using std::cerr;
using std::getline;


struct slot {
  unsigned sequence; //For LRU
  bool dirty = false;
};

int check_power_of_two(int parameter) {
  if(parameter % 2 != 0) {
    cerr << "set count or block size is not power of 2" << endl;
    return -1;
  }
}

int check_block_count(int parameter) {
  if(parameter <= 0) {
    cerr << "block count is not a positive number" << endl;
    return -1;
  }
}
/*
* Params: command stores the command for future opeartion, [0]: 0 is "s", 1 is "l", [1]: tag, [2]: index
* line: a line from trace file
* 
* Given these param, get the save and load command, breakdown the address into tag and index based on set count and block size
*/
void parse_line(unsigned* command, string line, int set_count, int block_size) {
  command[0] = line[0];

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

  vector<slot> set; //Representation of slot, an array of slots
  map<unsigned, slot*> cache; //Representation of a cache, mapping index to sets


  string line;
  while (scanf(" %[^\n]", line)== 1){ //reading from input file
    unsigned command[3];
    parse_line(command, line, set_count, block_size);
    unsigned load_save = command[0];
    unsigned tag = command[1];
    unsigned index = command[2];

}