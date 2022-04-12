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
  uint16_t sh_number = elf_header->e_shnum;
  uint16_t idx_sec_w_stringTable = elf_header->e_shstrndx;
  unsigned char endian = elf_header->e_ident[EI_DATA];// 1 is little, 2 is big

  printf("Object file type: ");
  printf(get_type_name(elf_header->e_type));
  printf("\n");
  
  printf("Instruction set: ");
  printf(get_machine_name(elf_header->e_machine));
  printf("\n");

  if(endian == 1) printf("Endianness: Little endian\n");
  else printf("Endianness: Big endian\n"); 

  unsigned char* initial_location = (unsigned char*) elf_header;
  unsigned char* section_location = initial_location + (elf_header->e_shoff);
  Elf64_Shdr *section_header = (Elf64_Shdr *) section_location;

  Elf64_Shdr *cur_header = section_header;
  unsigned char* current_loc = (unsigned char*) cur_header;
  unsigned char* name_table = section_header[elf_header->e_shstrndx].sh_offset + initial_location;
  uint16_t symbol_index = 0;
  uint16_t symbol_name_index = 0;

  for(uint16_t i; i < sh_number;i++) {
    cur_header = (Elf64_Shdr *) current_loc;
    char* name = (char*) (cur_header->sh_name + name_table);
    printf("Section header %d: name=%s, type=%lx, offset=%lx, size=%lx\n", i, cur_header->sh_name + name_table, cur_header->sh_type, cur_header->sh_offset, cur_header->sh_size);
    current_loc = current_loc + (elf_header->e_shentsize);
    if(string(name) == ".symtab") symbol_index = i;
    if(string(name) == ".strtab") symbol_name_index = i;
  }

  Elf64_Sym* sym_table = (Elf64_Sym*) (section_header[symbol_index].sh_offset + initial_location);
  uint64_t number_of_symbols = section_header[symbol_index].sh_size / section_header[symbol_index].sh_entsize;

  unsigned char* symbol_name_table = section_header[symbol_name_index].sh_offset + initial_location;
  for(uint64_t i = 0; i < number_of_symbols; i++) {
    printf("Symbol %d: name=%s, size=%lx, info=%lx, other=%lx\n", i, symbol_name_table + sym_table[i].st_name, sym_table[i].st_size, sym_table[i].st_info, sym_table[i].st_other);
  }


}
