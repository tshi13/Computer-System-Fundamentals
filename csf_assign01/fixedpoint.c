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
  // not sure if this is right
  char* whole_part = malloc(sizeof(char)*(index+1));
  char* fraction_part = malloc(sizeof(char) * (length - index));
  printf("length: %llu \n",length);
  uint64_t whole_index = 0;
  uint64_t frac_index = 0;
  for (uint64_t i = 0; i<index; i++) {
    //printf("%c",hex[i]);
    whole_part[whole_index++] = hex[i];
  }
  //printf("%s",whole_part);

  printf("\n");
  for (uint64_t i = index+1 ; i<length; i++) {
    //printf("%c",hex[i]);
    fraction_part[frac_index++] = hex[i];
  }
  //printf("%s",fraction_part);

  // printf("this is the fraction part string: %s",fraction_part);
  char* ptr1 = malloc(1);
  temp.whole = strtoul(whole_part,&ptr1,16);
  printf("whole part is: %x\n",temp.whole);
  temp.fractional = strtoul(fraction_part,&ptr1,16);
  printf("fractional part is: %x",temp.fractional);

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
  // TODO: implement
  assert(0);
  return 0;
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

int main(){
   char* ptr1 = malloc(1);
uint64_t temp= strtoul("0.00f2",&ptr1,16);
printf("answer is: %llu", temp);
}
