// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!

int main(int argc, char*argv[]) {
  
  //Read user input up to 16 bytes and store it in a string
  char userinput[16];
  unsigned num_elements = hex_read(userinput);
  unsigned count = 0;

  while(num_elements == 16) {
      //The output string
      char output[9];

      //Write the position to output
      hex_format_offset(count, output);
      hex_write_string(output);
      hex_write_string(": \0");

      //Write the hex number of each char to output
      for(int i = 0; i < num_elements; i++) {
          char cur[3];
          hex_format_byte_as_hex(userinput[i], cur);
          hex_write_string(cur);
          hex_write_string(" \0");
      }

      for(int i = num_elements; i < 16; i++) {
          hex_write_string("   \0");
      }

      hex_write_string("  \0");

      //Write the string to output
      hex_write_string(userinput);
      for(int i = num_elements; i < 16; i++) {
          hex_write_string(" \0");
      }

      hex_write_string("\n\0");
      num_elements = hex_read(userinput);
      count += 16;
  }

}
