// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!

int main(int argc, char*argv[]) {
  
  //Read user input up to 16 bytes and store it in a string
  char userinput[] = "";
  unsigned num_elements = hex_read(userinput);
  hex_write_string(userinput);//TODO: this is a trst, DELTE IT

  //The output string
  char output = "";

  //Write the position to output
  hex_format_offset(num_elements, output);
  hex_write_string(output);
  hex_write_string(": ");
  //Write the hex number of each char to output
  for(int i; i < num_elements; i++) {
    char cur[] = "";
    hex_format_byte_as_hex(userinput[i], cur);
    hex_write_string(cur);
  }
  hex_write_string("  ");
  //Write the string to output
  for(int i; i < num_elements; i++) {
    hex_write_string(hex_to_printable(userinput[i]));
  }
  hex_write_string("\n");
}
