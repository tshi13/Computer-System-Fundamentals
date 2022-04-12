#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cerrno>
#include <cstdint>
#include <elf.h>
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

  Elf64_Ehdr *elf_header = (Elf64_Ehdr *) data;
  Elf64_Off sh_location = elf_header->e_shoff;
  uint16_t sh_number = elf_header->e_shnum;
  uint16_t idx_sec_w_stringTable = elf_header->e_shstrndx;
  uint16_t entry_size = elf_header->e_shentsize;
  unsigned char endian = elf_header->e_ident[EI_DATA];// 1 is little, 2 is big
  printf("section location is %lu\nsection number is %u\n", sh_location, sh_number);
  printf(".shstrtab section index is %u\n", idx_sec_w_stringTable);

  printf("Object file type: ");
  printf(get_type_name(elf_header->e_type));
  printf("\n");
  
  printf("Instruction set: ");
  printf(get_machine_name(elf_header->e_machine));
  printf("\n");

  if(endian == 1) printf("Endianness: Little endian\n");
  else printf("Endianness: Big endian\n"); 

  printf("section entry size: %u\n", entry_size);

  Elf64_Shdr *section_header = ((Elf64_Shdr *) elf_header) + sh_location;
  for(uint16_t index = 0; index < sh_number; index++) {
    section_header += entry_size;
    printf("Section # %d", index);
  }
  


}
