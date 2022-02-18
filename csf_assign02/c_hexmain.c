// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!

int main(int argc, char*argv[]) {
  
  //Read user input up to 16 bytes and store it in a string
  char userinput[16];
  unsigned num_elements = hex_read(userinput);

  while(num_elements != - 1) {
      //The output string
      char output[9];

      //Write the position to output
      hex_format_offset(num_elements, output);
      hex_write_string(output);
      hex_write_string(": \0");
      /*
      //Write the hex number of each char to output
      for(int i = 0; i < num_elements; i++) {
          char cur[] = "";
          hex_format_byte_as_hex(userinput[i], cur);
          hex_write_string(cur);
      }

      hex_write_string("  ");
      //Write the string to output
      for(int i = 0; i < num_elements; i++) {
          //hex_write_string("Loop");
          //hex_write_string((char*) hex_to_printable(output[i]));
      }
       */
      num_elements = hex_read(userinput);
      hex_write_string(userinput);
  }
}
