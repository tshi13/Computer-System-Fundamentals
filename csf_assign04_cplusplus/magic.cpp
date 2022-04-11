#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cerrno>
#include <cstdint>
//#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <fcntl.h>
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::cerr;
using std::endl;


#include "elf_names.h"

int main(int argc, char **argv) {
  char* elf_filename = argv[1];
  cout << "start" << endl;
  size_t file_size;
  int fd = open(elf_filename, O_RDONLY);
  if(fd == -1) {
    cerr << "File cannot be opened" << endl; 
    return -1;
  }

  //Reading bytes from the file
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
    cerr << "Error while reading bytes" << endl;
    return -1;
  } else {
    file_size = statbuf.st_size;
  }
  void *data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
  cout << "done" << endl;


}
