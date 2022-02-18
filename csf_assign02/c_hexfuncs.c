// C implementation of hexdump functions

#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"

unsigned hex_read(char data_buf[]) {
    ssize_t read_result = read(0, data_buf, 16);
    return read_result;
}

void hex_write_string(const char s[]) {
    write (1, s, 16);
}

void hex_format_offset(unsigned offset, char sbuf[]) {
    int index = 7;
    while(offset != 0){
        if (offset % 16 > 9) {
            sbuf[index--] = 'a' + (offset % 16) - 10;
        } else {
            sbuf[index --] = '0' + (offset % 16);
        }
        offset /= 16;
    }
}

void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]) {
    int index = 1;
    while(byteval != 0){
        if (byteval % 16 > 9) {
            sbuf[index--] = 'a' + (byteval % 16) - 10;
        } else {
            sbuf[index --] = '0' + (byteval % 16);
        }
        byteval /= 16;
    }
}

char hex_to_printable(unsigned char byteval) {
    char result;
    if(byteval > 31) {
        result = byteval;
    } else {
        result = 46;
    }
    return result;
}


