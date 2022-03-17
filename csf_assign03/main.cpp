#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::stoi;



int main(int argc, char *argv[]){
  //read in command line arguments
  if (argc != 7) {
    cout << "invalid number of arguments"<<endl;
  }
  int set_count = stoi(argv[1]);
  int block_count = stoi(argv[2]);
  int block_size = stoi(argv[3]);
  string write_allocate = argv[4];
  string write_through = argv[5];
  string eviction = argv[6];
  bool isLoad;

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
      cin >> temp; //skip last number
    }
  }

}