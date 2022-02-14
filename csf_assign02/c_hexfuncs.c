// C implementation of hexdump functions

#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"

unsigned hex_read(char data_buf[]) {
    ssize_t read_result = read(0, data_buf, 16);

    //Return number of bytes read if there is input
    if(read_result > 0) return read_result;
    return 0; //Return 0 if there is not input or there is error
}

void hex_write_string(const char s[]) {
    //Count how many chars there are in s
    int count = 0;
    char curr = s[0];
    while(curr != '\n') {
        count++;
        curr = s[count];
    }
    write (1, s, count);
}



