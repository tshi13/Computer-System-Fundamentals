// C implementation of hexdump functions

#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"

unsigned hex_read(char data_buf[]) {
    unsigned count = 0;
    ssize_t read_result = read(0, data_buf, 16);

    //Return number of bytes read if there is input
    if(read_result > 0) {
        count = read_result;
        return count;
    }
    return 0; //Return 0 if there is not input or there is error
}

void hex_write_string(const char s[]) {
    write (1, s, 16);
}
