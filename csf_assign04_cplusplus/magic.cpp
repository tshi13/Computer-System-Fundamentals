/*
 * main file for reading ELF hw
 * CSF Assignment 4
 * Taiming Shi, Ian Zheng
 * tshi13@jh.edu yzheng67@jh.edu
 */
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

  //Get the start location and read helf header
  void *data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
  Elf64_Ehdr *elf_header = (Elf64_Ehdr *) data;
  unsigned char endian = elf_header->e_ident[EI_DATA];// 1 is little, 2 is big

  //Check if the file is ELF file
  if(elf_header->e_ident[EI_MAG0] != 0x7f || elf_header->e_ident[EI_MAG1] != 'E'
    || elf_header->e_ident[EI_MAG2] != 'L' || elf_header->e_ident[EI_MAG3] !='F') {
      cerr << "Not an ELF file" << endl;
      return -1;
    }

  //Print out the ELF header info
  cout << "Object file type: " << get_type_name(elf_header->e_type) << endl;
  cout << "Instruction set: " << get_machine_name(elf_header->e_machine) << endl;
  if(endian == 1) printf("Endianness: Little endian\n");
  else printf("Endianness: Big endian\n"); 

  //Get the locations of the start and the section
  unsigned char* initial_location = (unsigned char*) elf_header;
  unsigned char* section_location = initial_location + (elf_header->e_shoff);
  Elf64_Shdr *section_header = (Elf64_Shdr *) section_location;

  //Get the table for the section name
  unsigned char* name_table = section_header[elf_header->e_shstrndx].sh_offset + initial_location;
  uint16_t symbol_index = 0;
  uint16_t symbol_name_index = 0;

  //Iterate through section headers and output the information
  for(uint16_t i; i < elf_header->e_shnum; i++) {
    char* name = (char*) (section_header[i].sh_name + name_table);
    printf("Section header %d: name=%s, type=%lx, offset=%lx, size=%lx\n", i, section_header[i].sh_name + name_table, section_header[i].sh_type, section_header[i].sh_offset, section_header[i].sh_size);
    if(string(name) == ".symtab") symbol_index = i;
    if(string(name) == ".strtab") symbol_name_index = i;
  }

  //Get the location of the symbol table and the table of names
  Elf64_Sym* sym_table = (Elf64_Sym*) (section_header[symbol_index].sh_offset + initial_location);
  uint64_t number_of_symbols = section_header[symbol_index].sh_size / section_header[symbol_index].sh_entsize;
  unsigned char* symbol_name_table = section_header[symbol_name_index].sh_offset + initial_location;

  //Iterate through symbol table and output the information
  for(uint64_t i = 0; i < number_of_symbols; i++) {
    printf("Symbol %d: name=%s, size=%lx, info=%lx, other=%lx\n", i, symbol_name_table + sym_table[i].st_name, sym_table[i].st_size, sym_table[i].st_info, sym_table[i].st_other);
  }
}
