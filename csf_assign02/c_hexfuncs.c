// C implementation of hexdump functions

#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"

unsigned hex_read(char data_buf[]) {
    ssize_t read_result = read(0, data_buf, 16);
    return read_result;
}

void hex_write_string(const char s[]) {
    unsigned length = 16;
    for (int i = 0; i < 16; i++) {
        if(s[i] == '\0') {
            length = i;
            break;
        }
    }
    write (1, s, length);
}

void hex_format_offset(unsigned offset, char sbuf[]) {
    for (int i=0;i<8; i++){
        sbuf[i] = '0';
    }
    sbuf[8] = '\0';
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
    char map[] = {"0123456789abcdef"};
    sbuf[2] = '\0';
    sbuf[1] = map[byteval & 0xf];
    byteval >>= 4;
    sbuf[0] = map[byteval & 0xf];


    // sbuf[0] = '0';
    // sbuf[1] = '0';
    // sbuf[2] = '\0';
    // int index = 1;
    // while(byteval != 0){
    //     if (byteval % 16 > 9) {
    //         sbuf[index--] = 'a' + (byteval % 16) - 10;
    //     } else {
    //         sbuf[index --] = '0' + (byteval % 16);
    //     }
    //     byteval /= 16;
    // }
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


