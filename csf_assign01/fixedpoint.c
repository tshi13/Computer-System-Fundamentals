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

uint64_t hexstring_is_valid(char *hex){
  uint64_t length = strlen(hex);
  uint64_t valid = 1;

  if(length == 0){
    valid = 0;
  }

  uint64_t minus_position = 0;
  uint64_t dot_count = 0;

  for (uint64_t i = 0; i<length; i++) {
      char c = hex[i];
      if(!((c >= 'a' && c<='f')||(c>='A' && c<='F')||(c>='0' && c<='9') || c=='.' || c=='-')){
        valid = 0;
        // printf("character outof bounds error");
      }
      if (c == '-') minus_position = i;
      if (c == '.') dot_count++;
  }

  if(dot_count > 1 || (minus_position > 0)){
    // printf("dot count: %lu \n",dot_count);
    // printf("minus position: %lu",minus_position);
    valid = 0;
  }

  return valid;
}



Fixedpoint fixedpoint_create_from_hex(const char *hex) {
  Fixedpoint temp;
  uint64_t length = strlen(hex);

  if(hexstring_is_valid(hex) == 0){
    temp.tag = 2;
    temp.whole = 0;
    temp.fractional = 0;
    return temp;
  }


  uint64_t index = 0;
  uint64_t start_index = 0;
  if (hex[0] == '-') {
    index = 1;
    temp.tag = 1;
    start_index = 1;
  } else {
    temp.tag = 0;
  }
  
  for (; index<length; index++) {
    if(hex[index] == '.')
      break;
  }

  if (index - start_index > 16 || length - index - 1 > 16){
    temp.tag = 2;
    temp.whole = 0;
    temp.fractional = 0;
    return temp;
  }
  char whole_part[index - start_index +1];
  char fraction_part[length - index];
  uint64_t whole_index = 0;
  uint64_t frac_index = 0;
  for (uint64_t i = start_index; i<index; i++) {
    whole_part[whole_index++] = hex[i];
  }
  temp.whole = strtoul(whole_part,NULL,16);
  if (index == length){
    temp.fractional = 0;
    return temp;
  }

  for (uint64_t i = index+1 ; i<=length; i++) {
    fraction_part[frac_index++] = hex[i];
  }
  uint64_t frac_length = strlen(fraction_part);
  
  char padding [16-frac_length];
  for (uint64_t i = 0; i< 16-frac_length; i++){
    padding[i] = '0';
  }
  strcat(fraction_part,padding);

  temp.fractional= strtoul(fraction_part,NULL,16);

  return temp;

}

uint64_t fixedpoint_whole_part(Fixedpoint val) {
  return val.whole;
}

uint64_t fixedpoint_frac_part(Fixedpoint val) {
  return val.fractional;
}

Fixedpoint diff_sign_addition(Fixedpoint left, Fixedpoint right) {
    Fixedpoint bigger;
    Fixedpoint smaller;
    Fixedpoint result;
    //Determine the bigger fixedpoint value
    if (fixedpoint_whole_part(left) > fixedpoint_whole_part(right)){
        bigger = left;
        smaller = right;
    } else if(fixedpoint_whole_part(left) < fixedpoint_whole_part(right)) {
        bigger = right;
        smaller = left;
    } else {
        if (fixedpoint_frac_part(left) > fixedpoint_frac_part(right)){
            bigger = left;
            smaller = right;
        }
        else{
            bigger = right;
            smaller = left;
        }
    }
    result.whole = bigger.whole - smaller.whole;
    result.tag = bigger.tag;
    result.fractional = bigger.fractional - smaller.fractional;
    if(result.fractional >= bigger.fractional){ //We need to borrow 1 from whole
        result.whole--;
    }
    return result;
}

Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
  Fixedpoint result;

  if(fixedpoint_is_zero(left) || fixedpoint_is_zero(right)) {
      result.fractional = right.fractional + left.fractional;
      if (fixedpoint_is_zero(left)){
          result.tag = right.tag;
          result.whole = right.whole;
      }
      else{
          result.tag = left.tag;
          result.whole = left.whole;
      }

  }
  // Only changing magnitude for addition with same signs or adding zeroi
  else if(left.tag == right.tag) {
      result.whole = left.whole + right.whole;
      result.fractional = right.fractional + left.fractional;
      result.tag = left.tag;
      //Check for whole_part overflow before frac overflow
      if(result.whole < left.whole && result.whole < right.whole) {
          if(result.tag == 0) {
              result.tag = 3; //positive overflow occured
          } else {
              result.tag = 4; //negative overflow occured
          }
      }
      //Check for frac_part overflow
      if(result.fractional < left.fractional && result.fractional < right.fractional) result.whole += 1;

  } else {//Addition with different signs
      result = diff_sign_addition(left,right);
  }

  return result;
}

Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  return fixedpoint_add(left, fixedpoint_negate(right));
}

Fixedpoint fixedpoint_negate(Fixedpoint val) {
  if (fixedpoint_is_zero(val)) return val;

  if (val.tag == 1) val.tag = 0;
  else val.tag = 1;

  return val;
}

Fixedpoint fixedpoint_halve(Fixedpoint val) {
  uint64_t whole_copy = val.whole;
  uint64_t frac_copy = val.fractional;
  // check if frac will underflow after division
  if (frac_copy & 1 == 1){
    if (val.tag == 0) {val.tag = 5;} //positive underflow
    if (val.tag == 1) {val.tag = 6;} //negative underflow
    return val;
  }
  frac_copy >>= 1;
  //check if whole will lose a bit after division
  if (whole_copy & 1 == 1){
      frac_copy += 0x8000000000000000;
  }
  whole_copy >>= 1;
  val.fractional = frac_copy;
  val.whole = whole_copy;
  return val;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
  uint64_t whole_copy = val.whole;
  uint64_t frac_copy = val.fractional;

  // check if whole will overflow
  if ((whole_copy & (1<<(64-1))) == 1) {
    if (val.tag == 0) val.tag = 3;
    if (val.tag == 1) val.tag = 4;
    return val;
  }

  whole_copy <<= 1;
  if (frac_copy & (1<<(64-1))==1){
    whole_copy += 1;
  }
  frac_copy <<= 1;
  
  
  
  if (whole_copy < val.whole){
    if (val.tag == 0) val.tag = 3;
    if (val.tag == 1) val.tag = 4;
    return val;
  }

  val.whole = whole_copy;
  val.fractional = frac_copy;
  return val; 

}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  if(left.tag == 0 && right.tag == 1){
    return 1;
  }
  if(left.tag == 1 && right.tag == 0){
    return -1;
  }

  // now left and right have the same sign
  if(left.whole > right.whole){
    if (left.tag == 0) return 1;
    else return -1;
  }


  if(left.whole < right.whole){
    if (left.tag == 0)return -1;
    else return 1;
  }

  if(left.fractional > right.fractional){
    if (left.tag == 0) return 1;
    else return -1;
  }

  if(left.fractional < right.fractional){
    if (left.tag == 0) return -1;
    else return 1;
  }

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
  if(val.tag == 2){
    return 1;
  } else{
    return 0;
  }
}

int fixedpoint_is_neg(Fixedpoint val) {
  uint64_t tag = val.tag;
  //printf("tag is : %lu",tag);
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
  printf("invalid tag is: %lu \n",val.tag);
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
