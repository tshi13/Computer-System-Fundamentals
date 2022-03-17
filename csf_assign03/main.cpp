#include <string>
#include <iostream>
#include <map>

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::stoi;
using std::map;

//map index -> set, set is a vector/array (stores tag)

struct slot {
  bool valid = false;
  bool dirty = false;
};



int main(int argc, char *argv[]){
  //read in command line arguments
  int set_count = stoi(argv[1]);
  int block_count = stoi(argv[2]);
  int block_size = stoi(argv[3]);
  string write_allocate = argv[4];
  string write_through = argv[5];
  string eviction = argv[6];
  bool isLoad;

  map<unsigned, slot> set; //Representation of a set, mapping index sets
  map<unsigned, map<unsigned, slot> > cache; //Representation of a cache, mapping tag to sets


  string temp;
  while (cin >> temp){ //reading from input file
    if (temp == "l"){ //loading
      isLoad = true;
      cout << "loading" << endl;
    } else if (temp == "s") { //saving
      isLoad = false;
      cout << "saving" << endl;
    } else {
      cout << "read address" << endl;
    }
  }
}