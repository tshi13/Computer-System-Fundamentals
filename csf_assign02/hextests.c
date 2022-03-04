// Unit tests for hex functions
// These tests should work for both your C implementations and your
// assembly language implementations
//Taiming Shi tshi13
//Ian Zheng yzheng67

#include <stdio.h>
#include <stdlib.h>
#include "tctest.h"
#include "hexfuncs.h"

// test fixture object
typedef struct {
  char test_data_1[16];
  char test_data_symbol[16];
  char test_data_nothing[16];
  char test_data_letters[16];
} TestObjs;

// setup function (to create the test fixture)
TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));
  strcpy(objs->test_data_1, "Hello, world!\n");
  strcpy(objs->test_data_symbol, "~!$^*(+}'>?\n");
  strcpy(objs->test_data_nothing, "");
  strcpy(objs->test_data_letters, "aghmzAGHMZ\n");
  
  return objs;
}

// cleanup function (to destroy the test fixture)
void cleanup(TestObjs *objs) {
  free(objs);
}

// Prototypes for test functions
void testFormatOffset(TestObjs *objs);
void testFormatByteAsHex(TestObjs *objs);
void testHexToPrintable(TestObjs *objs);

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(testFormatOffset);
  TEST(testFormatByteAsHex);
  TEST(testHexToPrintable);

  TEST_FINI();

  return 0;
}

void testFormatOffset(TestObjs *objs) {
  (void) objs; // suppress warning about unused parameter
  char buf[16];
  
  //zero
  hex_format_offset(0L, buf);
  ASSERT(0 == strcmp(buf, "00000000"));
  //unsigned small
  hex_format_offset(1L, buf);
  ASSERT(0 == strcmp(buf, "00000001"));

  //unsigned medium
  hex_format_offset(716L, buf);
  ASSERT(0 == strcmp(buf, "000002cc"));

  hex_format_offset(43981L, buf);
  ASSERT(0 == strcmp(buf, "0000abcd"));

  hex_format_offset(16436428L, buf);
  ASSERT(0 == strcmp(buf, "00facccc"));

  //unsigned large
  hex_format_offset(3910912371L, buf);
  ASSERT(0 == strcmp(buf, "e91bc973"));

  //unsigned max
  hex_format_offset(4294967295L, buf);
  ASSERT(0 == strcmp(buf, "ffffffff"));
}

void testFormatByteAsHex(TestObjs *objs) {
  char buf[16];
  //Test each in test_data_1
  hex_format_byte_as_hex(objs->test_data_1[0], buf); //H
  ASSERT(0 == strcmp(buf, "48"));

  hex_format_byte_as_hex(objs->test_data_1[1], buf); //e
  ASSERT(0 == strcmp(buf, "65"));

  hex_format_byte_as_hex(objs->test_data_1[3], buf); //l
  ASSERT(0 == strcmp(buf, "6c"));

  hex_format_byte_as_hex(objs->test_data_1[4], buf); //o
  ASSERT(0 == strcmp(buf, "6f"));

  hex_format_byte_as_hex(objs->test_data_1[5], buf); //,
  ASSERT(0 == strcmp(buf, "2c"));

  hex_format_byte_as_hex(objs->test_data_1[6], buf); //(space)
  ASSERT(0 == strcmp(buf, "20"));

  hex_format_byte_as_hex(objs->test_data_1[7], buf); //w
  ASSERT(0 == strcmp(buf, "77"));


  hex_format_byte_as_hex(objs->test_data_1[9], buf); //r
  ASSERT(0 == strcmp(buf, "72"));

  hex_format_byte_as_hex(objs->test_data_1[11], buf); //d
  ASSERT(0 == strcmp(buf, "64"));

  hex_format_byte_as_hex(objs->test_data_1[13], buf); //\n
  ASSERT(0 == strcmp(buf, "0a"));

  //Test each in test_data_symbol ~!$^*(+}'>?
  hex_format_byte_as_hex(objs->test_data_symbol[0], buf); //~
  ASSERT(0 == strcmp(buf, "7e"));

  hex_format_byte_as_hex(objs->test_data_symbol[1], buf); //!
  ASSERT(0 == strcmp(buf, "21"));

  hex_format_byte_as_hex(objs->test_data_symbol[2], buf); //$
  ASSERT(0 == strcmp(buf, "24"));

  hex_format_byte_as_hex(objs->test_data_symbol[3], buf); //^
  ASSERT(0 == strcmp(buf, "5e"));

  hex_format_byte_as_hex(objs->test_data_symbol[4], buf); //*
  ASSERT(0 == strcmp(buf, "2a"));

  hex_format_byte_as_hex(objs->test_data_symbol[5], buf); //(
  ASSERT(0 == strcmp(buf, "28"));

  hex_format_byte_as_hex(objs->test_data_symbol[6], buf); //+
  ASSERT(0 == strcmp(buf, "2b"));

  hex_format_byte_as_hex(objs->test_data_symbol[7], buf); //}
  ASSERT(0 == strcmp(buf, "7d"));

  hex_format_byte_as_hex(objs->test_data_symbol[8], buf); //'
  ASSERT(0 == strcmp(buf, "27"));

  hex_format_byte_as_hex(objs->test_data_symbol[9], buf); //>
  ASSERT(0 == strcmp(buf, "3e"));

  hex_format_byte_as_hex(objs->test_data_symbol[10], buf); //?
  ASSERT(0 == strcmp(buf, "3f"));

  //Test test_data_nothing
  hex_format_byte_as_hex(objs->test_data_nothing[0], buf); //NULL
  ASSERT(0 == strcmp(buf, "00"));

  //Test test_data_letters aghmzAGHMZ
  hex_format_byte_as_hex(objs->test_data_letters[0], buf); //a
  ASSERT(0 == strcmp(buf, "61"));

  hex_format_byte_as_hex(objs->test_data_letters[1], buf); //g
  ASSERT(0 == strcmp(buf, "67"));

  hex_format_byte_as_hex(objs->test_data_letters[2], buf); //h
  ASSERT(0 == strcmp(buf, "68"));

  hex_format_byte_as_hex(objs->test_data_letters[3], buf); //m
  ASSERT(0 == strcmp(buf, "6d"));

  hex_format_byte_as_hex(objs->test_data_letters[4], buf); //z
  ASSERT(0 == strcmp(buf, "7a"));

  hex_format_byte_as_hex(objs->test_data_letters[5], buf); //A
  ASSERT(0 == strcmp(buf, "41"));

  hex_format_byte_as_hex(objs->test_data_letters[6], buf); //G
  ASSERT(0 == strcmp(buf, "47"));

  hex_format_byte_as_hex(objs->test_data_letters[7], buf); //H
  ASSERT(0 == strcmp(buf, "48"));

  hex_format_byte_as_hex(objs->test_data_letters[8], buf); //M
  ASSERT(0 == strcmp(buf, "4d"));

  hex_format_byte_as_hex(objs->test_data_letters[9], buf); //Z
  ASSERT(0 == strcmp(buf, "5a"));
}

void testHexToPrintable(TestObjs *objs) {
  //test_data_1
  ASSERT('H' == hex_to_printable(objs->test_data_1[0]));
  ASSERT('e' == hex_to_printable(objs->test_data_1[1]));
  ASSERT('l' == hex_to_printable(objs->test_data_1[2]));
  ASSERT('o' == hex_to_printable(objs->test_data_1[4]));
  ASSERT(',' == hex_to_printable(objs->test_data_1[5]));
  ASSERT(' ' == hex_to_printable(objs->test_data_1[6]));
  ASSERT('w' == hex_to_printable(objs->test_data_1[7]));
  ASSERT('r' == hex_to_printable(objs->test_data_1[9]));
  ASSERT('d' == hex_to_printable(objs->test_data_1[11]));
  ASSERT('!' == hex_to_printable(objs->test_data_1[12]));
  ASSERT('.' == hex_to_printable(objs->test_data_1[13]));

  //test_data_symbol
  ASSERT('~' == hex_to_printable(objs->test_data_symbol[0]));
  ASSERT('!' == hex_to_printable(objs->test_data_symbol[1]));
  ASSERT('$' == hex_to_printable(objs->test_data_symbol[2]));
  ASSERT('^' == hex_to_printable(objs->test_data_symbol[3]));
  ASSERT('*' == hex_to_printable(objs->test_data_symbol[4]));
  ASSERT('(' == hex_to_printable(objs->test_data_symbol[5]));
  ASSERT('+' == hex_to_printable(objs->test_data_symbol[6]));
  ASSERT('}' == hex_to_printable(objs->test_data_symbol[7]));
  ASSERT('\'' == hex_to_printable(objs->test_data_symbol[8]));
  ASSERT('>' == hex_to_printable(objs->test_data_symbol[9]));
  ASSERT('?' == hex_to_printable(objs->test_data_symbol[10]));
  ASSERT('.' == hex_to_printable(objs->test_data_symbol[11]));
  //test_data_nothing
  ASSERT('.' == hex_to_printable(objs->test_data_nothing[0]));
  ASSERT('.' == hex_to_printable(objs->test_data_nothing[1]));

  //test_data_letters
  ASSERT('a' == hex_to_printable(objs->test_data_letters[0]));
  ASSERT('g' == hex_to_printable(objs->test_data_letters[1]));
  ASSERT('h' == hex_to_printable(objs->test_data_letters[2]));
  ASSERT('m' == hex_to_printable(objs->test_data_letters[3]));
  ASSERT('z' == hex_to_printable(objs->test_data_letters[4]));
  ASSERT('A' == hex_to_printable(objs->test_data_letters[5]));
  ASSERT('G' == hex_to_printable(objs->test_data_letters[6]));
  ASSERT('H' == hex_to_printable(objs->test_data_letters[7]));
  ASSERT('M' == hex_to_printable(objs->test_data_letters[8]));
  ASSERT('Z' == hex_to_printable(objs->test_data_letters[9]));
 
}
