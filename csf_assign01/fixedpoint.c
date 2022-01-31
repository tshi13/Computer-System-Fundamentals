#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fixedpoint.h"

// You can remove this once all of the functions are fully implemented
static Fixedpoint DUMMY;

Fixedpoint fixedpoint_create(uint64_t whole) {
  Fixedpoint temp;
  temp.whole = whole;
  temp.fractional = 0;
  temp.tag = 0;
  return temp;
}

Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac) {
  Fixedpoint temp;
  temp.whole = whole;
  temp.fractional = frac;
  temp.tag = 0;
  return temp;
}

Fixedpoint fixedpoint_create_from_hex(const char *hex) {
  Fixedpoint temp;
  uint64_t length = strlen(hex);
  uint64_t index = 0;
  for (; index<length; index++) {
    if(hex[index] == '.')
      break;
  }

  char* whole_part = malloc(sizeof(char)*(index+1)); //malloc1
  char* fraction_part = malloc(sizeof(char) * (length - index)); //malloc2
  uint64_t whole_index = 0;
  uint64_t frac_index = 0;
  for (uint64_t i = 0; i<index; i++) {
    whole_part[whole_index++] = hex[i];
  }
  for (uint64_t i = index+1 ; i<length; i++) {
    fraction_part[frac_index++] = hex[i];
  }
  uint64_t frac_length = strlen(fraction_part);
  
  char* ptr1 = malloc(1); //malloc3
  temp.whole = strtoul(whole_part,&ptr1,16);
 

  char* padding = malloc(16 - frac_length); //malloc4
  for (uint64_t i = 0; i< 16-frac_length; i++){
    padding[i] = '0';
  }

  strcat(fraction_part,padding);

  temp.fractional= strtoul(fraction_part,&ptr1,16);

  //consider different tags?
  temp.tag = 0;
  return temp;

}

uint64_t fixedpoint_whole_part(Fixedpoint val) {
  return val.whole;

}

uint64_t fixedpoint_frac_part(Fixedpoint val) {
  return val.fractional;
}

Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

Fixedpoint fixedpoint_negate(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

Fixedpoint fixedpoint_halve(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_zero(Fixedpoint val) {
  if(val.whole == 0 && val.fractional == 0) {
    return 1;
  } else {
    return 0;
  }
}

int fixedpoint_is_err(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_neg(Fixedpoint val) {
  uint64_t tag = val.tag;
  if (tag == 1 || tag == 4 || tag == 6) {
    return 1;
  } else {
    return 0;
  }
}

int fixedpoint_is_overflow_neg(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_overflow_pos(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_underflow_neg(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_underflow_pos(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_valid(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
  // TODO: implement
  assert(0);
  // char *s = malloc(20);
  // strcpy(s, "<invalid>");
  // return s;
  return 0;
}

// int main(){
//    char* ptr1 = malloc(1);
// //uint64_t fractional= strtoul("f200000000000000",&ptr1,16);
// uint64_t temp = 0b11110010000000000000000000000000000000000000000000000000;
// printf("answer is: %llx", temp);
// }
