// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!

int main(int argc, char*argv[]) {
  
  //Read user input up to 16 bytes and store it in a string
  char userinput[16];
  int num_elements = 16;
  unsigned count = 0;

  while(num_elements == 16) {
      num_elements = hex_read(userinput);
      if(num_elements == 0 || num_elements == -1) break;

      //Write the position to output
      char position[9];
      hex_format_offset(count, position);
      hex_write_string(position);
      hex_write_string(": ");

      //Write the hex number of each char to output
      for(int i = 0; i < num_elements; i++) {
          char cur[3];
          hex_format_byte_as_hex(userinput[i], cur);
          hex_write_string(cur);
          hex_write_string(" ");
      }
      for(int i = num_elements; i < 16; i++) {
          hex_write_string("   ");
      }

      //write each char of the sentence to output
      hex_write_string(" ");
      char sentence[num_elements + 1];
      for(int i = 0; i < num_elements; i++) {
        sentence[i] = hex_to_printable(userinput[i]);
      }
      sentence[num_elements] = '\0';
      hex_write_string(sentence);
      hex_write_string("\n");

      count += 16;//update the position 
  }
}
