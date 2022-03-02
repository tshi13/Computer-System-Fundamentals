// C implementation of hexdump functions

#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"

unsigned hex_read(char data_buf[]) {
    ssize_t read_result = read(0, data_buf, 16);
    return read_result;
}

void hex_write_string(const char s[]) {
    unsigned length = 16;
    //write till null terminator
    for (int i = 0; i < 16; i++) {
        if(s[i] == '\0') {
            length = i;
            break;
        }
    }
    write (1, s, length);
}


void hex_format_offset(unsigned offset, char sbuf[]) {
    char map[] = {"0123456789abcdef"};//mapping to hex characters
    for (int i=0;i<8; i++){ //initialze with zeros
      sbuf[i] = '0';
    }
    sbuf[8] = '\0';
    int index = 7;
    while(offset != 0){
      sbuf[index] = map[offset & 0xf];
      offset >>= 4; //shift 4 bits
      index--;
    }
}



void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]) {
    char map[] = {"0123456789abcdef"};//mapping to hex characters
    sbuf[2] = '\0';
    sbuf[1] = map[byteval & 0xf];//AND operation
    byteval >>= 4; //shift 4 bits
    sbuf[0] = map[byteval & 0xf]; //another AND operation

}

char hex_to_printable(unsigned char byteval) {
    char result;
    if(byteval > 31 && byteval < 127) { //check range
        result = byteval;
    } else {
        result = 46; //unprintable is .
    }
    return result;
}


