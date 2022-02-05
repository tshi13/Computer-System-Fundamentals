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
  temp.tag = 0; //giving valid/non-negative tag
  return temp;
}

Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac) {
  Fixedpoint temp;
  temp.whole = whole;
  temp.fractional = frac;
  temp.tag = 0; //valid/non-negative
  return temp;
}

Fixedpoint fixedpoint_create_from_hex(const char *hex) {
  Fixedpoint temp;
  uint64_t length = strlen(hex);

//check invalid string
for (uint64_t i = 0; i<length; i++) {
    char c = hex[i];
    if(!((c >= 'a' && c<='f')||(c>='A' && c<='F')||(c>='0' && c<='9') || c=='.' || c=='-')){
      temp.tag = 2;
      temp.whole = 0;
      temp.fractional = 0;
      return temp;
    }
  }


  uint64_t index = 0;
  uint64_t start_index = 0;
  if (hex[0] == '-') {
    index = 1;
    temp.tag == 1;
    start_index = 1;
  } else {
    temp.tag = 0;
  }
  
  for (; index<length; index++) {
    if(hex[index] == '.')
      break;
  }

  char whole_part[index+1]; //malloc1
  char fraction_part[length - index]; //malloc2
  uint64_t whole_index = 0;
  uint64_t frac_index = 0;
  for (uint64_t i = start_index; i<index; i++) {
    whole_part[whole_index++] = hex[i];
  }
  for (uint64_t i = index+1 ; i<length; i++) {
    fraction_part[frac_index++] = hex[i];
  }
  uint64_t frac_length = strlen(fraction_part);
  
  char* ptr1; //malloc3
  temp.whole = strtoul(whole_part,&ptr1,16);
 

  char padding [16-frac_length]; //malloc4
  for (uint64_t i = 0; i< 16-frac_length; i++){
    padding[i] = '0';
  }

  strcat(fraction_part,padding);

  temp.fractional= strtoul(fraction_part,&ptr1,16);

  return temp;

}

uint64_t fixedpoint_whole_part(Fixedpoint val) {
  return val.whole;
}

uint64_t fixedpoint_frac_part(Fixedpoint val) {
  return val.fractional;
}

Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
  Fixedpoint result;

  if(left.tag == right.tag) {
      result.whole = left.whole + right.whole;
      result.fractional = right.fractional + left.fractional;
      result.tag = left.tag;
  }

  return result;
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

//Return 1 is fixedpoint is zero, 0 otherwise
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
  if (val.tag == 4) {
    return 1;
  } else {
    return 0;
  }
}

int fixedpoint_is_overflow_pos(Fixedpoint val) {
  if (val.tag == 3){
    return 1;
  } else {
    return 0;
  }
}

int fixedpoint_is_underflow_neg(Fixedpoint val) {
  if (val.tag == 6) {
    return 1;
  } else{
    return 0;
  }
}

int fixedpoint_is_underflow_pos(Fixedpoint val) {
  if (val.tag == 5) {
    return 1;
  } else{
    return 0;
  }
}

int fixedpoint_is_valid(Fixedpoint val) {
  if (val.tag == 0 || val.tag == 1) {
    return 1;
  } else{
    return 0;
  }
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
