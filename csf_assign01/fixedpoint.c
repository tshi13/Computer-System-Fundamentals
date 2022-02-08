#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fixedpoint.h"


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


uint64_t hexstring_is_valid(const char *hex){
  uint64_t length = strlen(hex);
  if(length == 0){ //null string
    return 0;
  }
  uint64_t minus_position = 0; //negative sign position
  uint64_t dot_count = 0; // count how many dots are in the string
  for (uint64_t i = 0; i<length; i++) {
      char c = hex[i];
      if(!((c >= 'a' && c<='f')||(c>='A' && c<='F')||(c>='0' && c<='9') || c=='.' || c=='-')){ //check all characters in string
        return 0;
      }
      if (c == '-') minus_position = i;
      if (c == '.') dot_count++;
  }
  if(dot_count > 1 || (minus_position > 0)){ //when there are multiple dots/negative sign is not at the start
    return 0;
  }
  return 1;
}



Fixedpoint fixedpoint_create_from_hex(const char *hex) {
  Fixedpoint temp;
  uint64_t length = strlen(hex);
  if(hexstring_is_valid(hex) == 0){ //if string is valid, update error tag and return
    temp.tag = 2;
    temp.whole = 0;
    temp.fractional = 0;
    return temp;
  }
  uint64_t index = 0;//index of dot
  uint64_t start_index = 0;//start index where we begin loops later
  if (hex[0] == '-') { //when we have a negative fixedpoint, set negative tag
    index = 1;
    temp.tag = 1;
    start_index = 1;//when we want to start from index = 1, because of negative sign
  } else {
    temp.tag = 0; //set positive tag
  }
  for (; index<length; index++) { //find index of the dot
    if(hex[index] == '.')
      break;
  }
  if (index - start_index > 16 || (length - index > 0 && length - index - 1 > 16)){ //if whole part/frac string is too long, return early with tag
    temp.tag = 2;
    temp.whole = 0;
    temp.fractional = 0;
    return temp;
  }
  char whole_part[index - start_index + 2]; //create 2 strings for storage
  char fraction_part[length - index + 1];
  uint64_t whole_index = 0; //index for whole_part
  for (uint64_t i = start_index; i<index; i++) { //initialize whole part string
    whole_part[whole_index++] = hex[i];
  }
  whole_part[whole_index] = '\0';
  temp.whole = strtoul(whole_part,NULL,16);//convert whole string to uint64_t
  if (index == length -1){ //when there is no fractional part, end early
    temp.fractional = 0;
    return temp;
  }
  temp.fractional= create_frac_with_padding(fraction_part,hex,index);//convert frac string to uint64_t
  return temp;
}



uint64_t create_frac_with_padding(char* fraction_part, const char* hex, uint64_t index){
  uint64_t frac_index = 0;
  uint64_t length = strlen(hex);
  for (uint64_t i = index+1 ; i<=length; i++) { //initialize frac part string
    fraction_part[frac_index++] = hex[i];
  }
  fraction_part[frac_index] = '\0';
  uint64_t frac_length = strlen(fraction_part); //add corresponding padding zeros at the end of frac string
  char padding [16-frac_length + 1];
  uint64_t i = 0;
  for (; i< 16-frac_length; i++){ //create padding string
    padding[i] = '0';
  }
  padding[i] = '\0';
  strcat(fraction_part,padding); //add padding to frac string
  return strtoul(fraction_part,NULL,16);
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

    if(left.whole == right.whole && left.fractional == right.fractional) {
        result.whole = 0;
        result.fractional = 0;
        result.tag = 0;
        return result;
    }
    /*
    if(fixedpoint_compare(left, right) == 1) {
        bigger = left;
        smaller = right;
    } else if(fixedpoint_compare(left, right) == -1) {
        bigger = right;
        smaller = left;
    }*/

    //Make the number with bigger absolute number bigger
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
    if(result.fractional > bigger.fractional){ //We need to borrow 1 from whole
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
      if (fixedpoint_is_zero(left) && fixedpoint_is_zero(right)) {
          result.tag = 0;
      }
      return result;
  }
  // Only changing magnitude for addition with same signs or adding zero
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
      if(result.fractional < left.fractional && result.fractional < right.fractional) {
          if(UINT64_MAX == result.whole && result.tag == 0) {
              result.tag = 3;
          } else if(UINT64_MAX == result.whole && result.tag == 1) {
              result.tag = 4;
          }
          result.whole += 1;
      }
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

  if (val.tag == 1) {val.tag = 0;}
  else val.tag = 1;

  return val;
}


Fixedpoint fixedpoint_halve(Fixedpoint val) {
  uint64_t whole_copy = val.whole;
  uint64_t frac_copy = val.fractional;
  // check if frac will underflow after division
  if ((frac_copy & 1) == 1){
    if (val.tag == 0) {val.tag = 5;} //positive underflow
    if (val.tag == 1) {val.tag = 6;} //negative underflow
    return val;
  }
  frac_copy >>= 1; //shift frac_copy
  //check if whole will lose a bit after division
  if ((whole_copy & 1) == 1){
      frac_copy += 0x8000000000000000;
  }
  whole_copy >>= 1; //shift whole_copy
  val.fractional = frac_copy;
  val.whole = whole_copy;
  return val;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
  return fixedpoint_add(val,val);
}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  //compare signs
  if(left.tag == 0 && right.tag == 1){
    return 1;
  }
  if(left.tag == 1 && right.tag == 0){
    return -1;
  }

  // now left and right have the same sign, consider 4 possible situations
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
  return 0; //they are the same
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
  // printf("invalid tag is: %lu \n",val.tag);
  if (val.tag == 0 || val.tag == 1) {
    return 1;
  } else{
    return 0;
  }
}
char* format_as_hex_without_frac(char* whole_buffer,uint64_t tag){
  char* s = malloc((strlen(whole_buffer)+1+tag)*sizeof(char));//allocate enough memory
    if(tag == 1){ //when we need to add minus sign infront
      char temp [18] = {'-','\0'};//leave anough room to avoid errors when doing strcat
      strcat(temp,whole_buffer);//concatenate minus sign
      strcpy(s,temp);
      return s;
    }
    strcpy(s,whole_buffer); //copy whole buffer string
    return s;
}

char* format_as_hex_helper(char* whole_buffer, uint64_t tag){
  uint64_t length = strlen(whole_buffer);
  char* ptr = whole_buffer;
  uint64_t count = 0;//count number of useless zeros at the end of whole_buffer
  while(*ptr!='\0'){ //let ptr go to end
    ptr++;
  }
  ptr--;
  while(*ptr == '0'){ //while there are still useless trailing zeros
    ptr--;
    count++; //count trailing zeros
  }
  char* result = malloc(sizeof(char)*(length + 1 - count + tag)); //allocate enough memory
  char* copy = result; //keep track of the head pointer
  if (tag == 1){ //when we need to add minus sign 
    *result = '-';
    result ++;
  }
  for(uint64_t i = 0; i<length - count; i++){ //add string to result, ignoring trailing zeros
    *result = whole_buffer[i];
    result++; //update result pointer
  } 
  *result = '\0';
  return copy;
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
  uint64_t whole = val.whole; //initialize variables
  uint64_t frac = val.fractional;
  uint64_t tag = val.tag;  

  char whole_buffer [35]; //allocate enough space, to prevent issues when doing multiple strcat
  char frac_buffer [17];
  sprintf(whole_buffer,"%lx",whole); //convert whole and frac parts to string
  sprintf(frac_buffer,"%016lx",frac);
  if(frac == 0){
    return format_as_hex_without_frac(whole_buffer,tag); //when there isn't a frac part to consider
  }
  strcat(whole_buffer,"."); //add decimal point
  strcat(whole_buffer,frac_buffer); //add frac part to whole part
  return format_as_hex_helper(whole_buffer,tag); //return correctly formatted hexstring 
}


