#include <stdio.h>
#include <stdlib.h>
#include "fixedpoint.h"
#include "tctest.h"

// Test fixture object, has some useful values for testing
typedef struct {
  Fixedpoint zero;
  Fixedpoint one;
  Fixedpoint one_half;
  Fixedpoint one_fourth;
  Fixedpoint large1;
  Fixedpoint large2;
  Fixedpoint max;
  Fixedpoint neg_max;

  Fixedpoint neg_one;
  Fixedpoint neg_one_half;
  Fixedpoint neg_one_fourth;

  Fixedpoint one_and_three_fourth; //1.75
  Fixedpoint neg_one_and_three_fourth;

  Fixedpoint one_and_one_fourth; //1.25
  Fixedpoint neg_one_and_one_fourth;

  Fixedpoint three_fourth;
  Fixedpoint neg_three_fourth;

  Fixedpoint max_empty_frac;
  Fixedpoint neg_max_empty_frac;
  Fixedpoint neg_zero;

  // TODO: add more objects to the test fixture
} TestObjs;

// functions to create and destroy the test fixture
TestObjs *setup(void);
void cleanup(TestObjs *objs);

// test functions
void test_whole_part(TestObjs *objs);
void test_frac_part(TestObjs *objs);
void test_create_from_hex(TestObjs *objs);
void test_format_as_hex(TestObjs *objs);
void test_negate(TestObjs *objs);
void test_add(TestObjs *objs);
void test_sub(TestObjs *objs);
void test_is_overflow_pos(TestObjs *objs);
void test_is_err(TestObjs *objs);
// TODO: add more test functions

void test_create_all_IntObjs(TestObjs *objs);
void test_create2_all_FracObjs(TestObjs *objs);
void test_is_zero(TestObjs *objs);
void test_is_zero_uninitialized();
//addition
void addition_two_normal_positive(TestObjs *objs);
void addition_two_normal_negative(TestObjs *objs);
void addition_normal_pos_add_neg(TestObjs *objs);
void pos_add_pos_frac_overflow(TestObjs *objs);
void neg_add_neg_frac_overflow(TestObjs *objs);
void positive_add_zero(TestObjs *objs);
void negative_add_zero(TestObjs *objs);
void neg_add_neg_frac_overflow(TestObjs *objs);
void pos_add_neg_frac_overflow(TestObjs *objs);
void pos_add_pos_whole_overflow_pos(TestObjs *objs);
void neg_add_neg_whole_overflow_pos(TestObjs *objs);
void pos_add_pos_whole_and_frac_overflow(TestObjs *objs);
void neg_add_neg_whole_and_frac_overflow(TestObjs *objs);
void pos_add_pos_frac_overflow_lead_to_whole_overflow(TestObjs *objs);
void neg_add_neg_frac_overflow_lead_to_whole_overflow(TestObjs *objs);
void add_large(TestObjs *objs);
void add_same_mag_diff_sign(TestObjs *objs);
void add_result_is_zero(TestObjs *objs);

//Subtraction


int main(int argc, char **argv) {
  // if a testname was specified on the command line, only that
  // test function will be executed
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(test_whole_part);
  TEST(test_frac_part);
  TEST(test_create_from_hex);
  TEST(test_format_as_hex);
  /*
  TEST(test_create_from_hex);
  TEST(test_format_as_hex);
  TEST(test_negate);
   */

  TEST(test_add);
    /*
    TEST(test_sub);
    TEST(test_is_overflow_pos);
    TEST(test_is_err);
     */
  TEST(test_create_all_IntObjs);
  TEST(test_create2_all_FracObjs);
  TEST(test_is_zero);
  TEST(test_is_zero_uninitialized);
  TEST(addition_two_normal_positive);
  TEST(addition_two_normal_negative);
  TEST(addition_normal_pos_add_neg);
  TEST(pos_add_pos_frac_overflow);
  TEST(neg_add_neg_frac_overflow);
  TEST(positive_add_zero);
  TEST(negative_add_zero);
  TEST(neg_add_neg_frac_overflow);
  TEST(pos_add_neg_frac_overflow);
  TEST(pos_add_pos_whole_overflow_pos);
  TEST(neg_add_neg_whole_overflow_pos);
  TEST(pos_add_pos_whole_and_frac_overflow);
  TEST(neg_add_neg_whole_and_frac_overflow);
  TEST(pos_add_pos_frac_overflow_lead_to_whole_overflow);
  TEST(neg_add_neg_frac_overflow_lead_to_whole_overflow);
  TEST(add_large);
  TEST(add_same_mag_diff_sign);
  TEST(add_result_is_zero);

  // IMPORTANT: if you add additional test functions (which you should!),
  // make sure they are included here.  E.g., if you add a test function
  // "my_awesome_tests", you should add
  //
  //   TEST(my_awesome_tests);
  //
  // here. This ensures that your test function will actually be executed.

  TEST_FINI();
}

TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));

  objs->zero = fixedpoint_create(0UL);
  objs->one = fixedpoint_create(1UL);
  objs->one_half = fixedpoint_create2(0UL, 0x8000000000000000UL);
  objs->one_fourth = fixedpoint_create2(0UL, 0x4000000000000000UL);
  objs->large1 = fixedpoint_create2(0x4b19efceaUL, 0xec9a1e2418UL);
  objs->large2 = fixedpoint_create2(0xfcbf3d5UL, 0x4d1a23c24fafUL);
  objs->max = fixedpoint_create2(0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL);

  objs->neg_one = fixedpoint_negate(objs->one);
  objs->neg_one_half = fixedpoint_negate(objs->one_half);
  objs->neg_one_fourth = fixedpoint_negate(objs->one_fourth);

  objs->one_and_three_fourth = fixedpoint_create2(1UL, 0x8000000000000000UL + 0x4000000000000000UL);
  objs->neg_one_and_three_fourth = fixedpoint_negate(objs->one_and_three_fourth);
  objs->one_and_one_fourth = fixedpoint_create2(1UL, 0x4000000000000000UL);
  objs->neg_one_and_one_fourth = fixedpoint_negate(objs->one_and_one_fourth);
  objs->neg_max = fixedpoint_negate(objs->max);
  objs->three_fourth = fixedpoint_create2(0UL, 0x8000000000000000UL + 0x4000000000000000UL);
  objs->neg_three_fourth = fixedpoint_negate(objs->three_fourth);
  objs->max_empty_frac = fixedpoint_create2(0xFFFFFFFFFFFFFFFFUL, 0UL);
  objs->neg_max_empty_frac = fixedpoint_negate(objs->max_empty_frac);
  objs->neg_zero = fixedpoint_negate(objs->zero);

  return objs;
}

void cleanup(TestObjs *objs) {
  free(objs);
}

void test_whole_part(TestObjs *objs) {
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));
}

void test_frac_part(TestObjs *objs) {
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));
}

void test_create_from_hex(TestObjs *objs) {
  (void) objs;

  Fixedpoint val1 = fixedpoint_create_from_hex("f6a5865.00f2");
  ASSERT(fixedpoint_is_valid(val1));
  ASSERT(0xf6a5865UL == fixedpoint_whole_part(val1));
  //printf("this is my frac part: %lx",fixedpoint_frac_part(val1));
  ASSERT(0x00f2000000000000UL == fixedpoint_frac_part(val1));
  ASSERT(!fixedpoint_is_err(val1));


  Fixedpoint val2 = fixedpoint_create_from_hex("-f6a5865.00f2");
  ASSERT(fixedpoint_is_valid(val2));
  ASSERT(!fixedpoint_is_err(val2));
  ASSERT(0xf6a5865UL == fixedpoint_whole_part(val2));
  ASSERT(0x00f2000000000000UL == fixedpoint_frac_part(val2));
  ASSERT(fixedpoint_is_neg(val2));

  Fixedpoint val3 = fixedpoint_create_from_hex(".00f2");
  ASSERT(fixedpoint_is_valid(val3));
  ASSERT(!fixedpoint_is_err(val3));
  ASSERT(!fixedpoint_is_neg(val3));
  ASSERT(0x000UL == fixedpoint_whole_part(val3));
  ASSERT(0x00f2000000000000UL == fixedpoint_frac_part(val3));

  Fixedpoint val4 = fixedpoint_create_from_hex("xxx");
  ASSERT(!fixedpoint_is_valid(val4));
  ASSERT(fixedpoint_is_err(val4));

  Fixedpoint val5 = fixedpoint_create_from_hex("x-xx");
  ASSERT(!fixedpoint_is_valid(val5));
  ASSERT(fixedpoint_is_err(val5));


  Fixedpoint val6 = fixedpoint_create_from_hex("-f6a5865.");
  ASSERT(fixedpoint_is_valid(val6));
  ASSERT(!fixedpoint_is_err(val6));
  ASSERT(0xf6a5865UL == fixedpoint_whole_part(val6));
  ASSERT(0x00000000000000UL == fixedpoint_frac_part(val6));
  ASSERT(fixedpoint_is_neg(val6));

  Fixedpoint val7 = fixedpoint_create_from_hex(".");
  ASSERT(fixedpoint_is_valid(val7));
  ASSERT(!fixedpoint_is_err(val7));

  Fixedpoint val8 = fixedpoint_create_from_hex("f6a5865.");
  ASSERT(fixedpoint_is_valid(val8));
  ASSERT(!fixedpoint_is_err(val8));
  ASSERT(0xf6a5865UL == fixedpoint_whole_part(val8));
  ASSERT(0x00000000000000UL == fixedpoint_frac_part(val8));
  ASSERT(!fixedpoint_is_neg(val8));

  Fixedpoint val = fixedpoint_create_from_hex("-1111");
  ASSERT(fixedpoint_is_valid(val));
  ASSERT(fixedpoint_is_neg(val));
  ASSERT(0x1111UL == fixedpoint_whole_part(val));
  ASSERT(0UL == fixedpoint_frac_part(val));


}

void test_format_as_hex(TestObjs *objs) {
  char *s;

  s = fixedpoint_format_as_hex(objs->zero);
  ASSERT(0 == strcmp(s, "0"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one);
  ASSERT(0 == strcmp(s, "1"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one_half);
  ASSERT(0 == strcmp(s, "0.8"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one_fourth);
  ASSERT(0 == strcmp(s, "0.4"));
  free(s);

  s = fixedpoint_format_as_hex(objs->large1);
  ASSERT(0 == strcmp(s, "4b19efcea.000000ec9a1e2418"));
  free(s);

  s = fixedpoint_format_as_hex(objs->large2);
  ASSERT(0 == strcmp(s, "fcbf3d5.00004d1a23c24faf"));
  free(s);

  Fixedpoint temp = fixedpoint_negate(objs->large2);
  s = fixedpoint_format_as_hex(temp);
  printf("\n tag??: %lu\n",temp.tag);
  printf("%s\n",s);
  ASSERT(0 == strcmp(s, "-fcbf3d5.00004d1a23c24faf"));
  free(s);
  
}

void test_negate(TestObjs *objs) {
  // none of the test fixture objects are negative
  ASSERT(!fixedpoint_is_neg(objs->zero));
  ASSERT(!fixedpoint_is_neg(objs->one));
  ASSERT(!fixedpoint_is_neg(objs->one_half));
  ASSERT(!fixedpoint_is_neg(objs->one_fourth));
  ASSERT(!fixedpoint_is_neg(objs->large1));
  ASSERT(!fixedpoint_is_neg(objs->large2));

  // negate the test fixture values
  Fixedpoint zero_neg = fixedpoint_negate(objs->zero);
  Fixedpoint one_neg = fixedpoint_negate(objs->one);
  Fixedpoint one_half_neg = fixedpoint_negate(objs->one_half);
  Fixedpoint one_fourth_neg = fixedpoint_negate(objs->one_fourth);
  Fixedpoint large1_neg = fixedpoint_negate(objs->large1);
  Fixedpoint large2_neg = fixedpoint_negate(objs->large2);

  // zero does not become negative when negated
  ASSERT(!fixedpoint_is_neg(zero_neg));

  // all of the other values should have become negative when negated
  ASSERT(fixedpoint_is_neg(one_neg));
  ASSERT(fixedpoint_is_neg(one_half_neg));
  ASSERT(fixedpoint_is_neg(one_fourth_neg));
  ASSERT(fixedpoint_is_neg(large1_neg));
  ASSERT(fixedpoint_is_neg(large2_neg));

  // magnitudes should stay the same
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));
}

void test_add(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create_from_hex("-c7252a193ae07.7a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("d09079.1e6d601");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0xc7252a0c31d8eUL == fixedpoint_whole_part(sum));
  ASSERT(0x5be47e8ea0538c50UL == fixedpoint_frac_part(sum));
}

void test_sub(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, diff;

  lhs = fixedpoint_create_from_hex("-ccf35aa3a04a3b.b105");
  rhs = fixedpoint_create_from_hex("f676e8.58");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0xccf35aa496c124UL == fixedpoint_whole_part(diff));
  ASSERT(0x0905000000000000UL == fixedpoint_frac_part(diff));
}

void test_is_overflow_pos(TestObjs *objs) {
  Fixedpoint sum;

  sum = fixedpoint_add(objs->max, objs->one);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  sum = fixedpoint_add(objs->one, objs->max);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  Fixedpoint negative_one = fixedpoint_negate(objs->one);

  sum = fixedpoint_sub(objs->max, negative_one);
  ASSERT(fixedpoint_is_overflow_pos(sum));
}

void test_is_err(TestObjs *objs) {
  (void) objs;

  // too many characters
  Fixedpoint err1 = fixedpoint_create_from_hex("88888888888888889.6666666666666666");
  ASSERT(fixedpoint_is_err(err1));

  // too many characters
  Fixedpoint err2 = fixedpoint_create_from_hex("6666666666666666.88888888888888889");
  ASSERT(fixedpoint_is_err(err2));

  // this one is actually fine
  Fixedpoint err3 = fixedpoint_create_from_hex("-6666666666666666.8888888888888888");
  ASSERT(fixedpoint_is_valid(err3));
  ASSERT(!fixedpoint_is_err(err3));

  // whole part is too large
  Fixedpoint err4 = fixedpoint_create_from_hex("88888888888888889");
  ASSERT(fixedpoint_is_err(err4));

  // fractional part is too large
  Fixedpoint err5 = fixedpoint_create_from_hex("7.88888888888888889");
  ASSERT(fixedpoint_is_err(err5));

  // invalid hex digits in whole part
  Fixedpoint err6 = fixedpoint_create_from_hex("123xabc.4");
  ASSERT(fixedpoint_is_err(err6));

  // invalid hex digits in fractional part
  Fixedpoint err7 = fixedpoint_create_from_hex("7.0?4");
  ASSERT(fixedpoint_is_err(err7));
}

// TODO: implement more test functions

//Assume whole_part and frac_part have been tested
void test_create_all_IntObjs(TestObjs *objs) {
    ASSERT(fixedpoint_whole_part(objs->zero) == 0UL);
    ASSERT(fixedpoint_frac_part(objs->zero) == 0UL);
    ASSERT(objs->zero.tag == 0);

    ASSERT(fixedpoint_whole_part(objs->one) == 1UL);
    ASSERT(fixedpoint_frac_part(objs->one) == 0UL);
    ASSERT(objs->one.tag == 0);
}

void test_create2_all_FracObjs(TestObjs *objs) {
    ASSERT(fixedpoint_whole_part(objs->one_half) == 0UL);
    ASSERT(fixedpoint_frac_part(objs->one_half) == 0x8000000000000000UL);
    ASSERT(objs->one_half.tag == 0);

    ASSERT(fixedpoint_whole_part(objs->one_fourth) == 0UL);
    ASSERT(fixedpoint_frac_part(objs->one_fourth) == 0x4000000000000000UL);
    ASSERT(objs->one_fourth.tag == 0);

    ASSERT(fixedpoint_whole_part(objs->large1) == 0x4b19efceaUL);
    ASSERT(fixedpoint_frac_part(objs->large1) == 0xec9a1e2418UL);
    ASSERT(objs->large1.tag == 0);

    ASSERT(fixedpoint_whole_part(objs->large2) == 0xfcbf3d5UL);
    ASSERT(fixedpoint_frac_part(objs->large2) == 0x4d1a23c24fafUL);
    ASSERT(objs->large2.tag == 0);
}

void test_is_zero(TestObjs *objs) {
    ASSERT(fixedpoint_is_zero(objs->zero) == 1);
    ASSERT(fixedpoint_is_zero(objs->one) == 0);
    ASSERT(fixedpoint_is_zero(objs->one_half) == 0);
    ASSERT(fixedpoint_is_zero(objs->one_fourth) == 0);
    ASSERT(fixedpoint_is_zero(objs->large1) == 0);
    ASSERT(fixedpoint_is_zero(objs->large2) == 0);
}

//Uninitialized value should not be zero
void test_is_zero_uninitialized() {
    Fixedpoint uninitialized;
    ASSERT(fixedpoint_is_zero(uninitialized) == 0);
}

//Tests for add
void addition_two_normal_positive(TestObjs *objs) {
    //1.0 + 0.0 = 1.0
    Fixedpoint result = fixedpoint_add(objs->zero, objs->one);
    ASSERT(fixedpoint_whole_part(result) == 1UL);
    ASSERT(fixedpoint_frac_part(result) == 0UL);
    ASSERT(result.tag == 0);

    //0.5 + 0.25 = 0.75
    result = fixedpoint_add(objs->one_half, objs->one_fourth);
    ASSERT(fixedpoint_whole_part(result) == 0UL);
    ASSERT(fixedpoint_frac_part(result) == (0x8000000000000000UL + 0x4000000000000000UL));
    ASSERT(result.tag == 0);

}

void addition_two_normal_negative(TestObjs *objs) {
    //-1.0 + 0.0 = -1.0
    Fixedpoint result = fixedpoint_add(objs->zero, objs->neg_one);
    ASSERT(fixedpoint_whole_part(result) == 1UL);
    ASSERT(fixedpoint_frac_part(result) == 0UL);
    ASSERT(result.tag == 1);

    //-0.5 + -0.25 = -0.75
    result = fixedpoint_add(objs->neg_one_half, objs->neg_one_fourth);
    ASSERT(fixedpoint_whole_part(result) == 0UL);
    ASSERT(fixedpoint_frac_part(result) == (0x8000000000000000UL + 0x4000000000000000UL));
    ASSERT(result.tag == 1);

}

void addition_normal_pos_add_neg(TestObjs *objs) {
    //1.0 + -0.5 = 0.5
    Fixedpoint result = fixedpoint_add(objs->one, objs->neg_one_half);
    ASSERT(fixedpoint_whole_part(result) == 0UL);
    ASSERT(fixedpoint_frac_part(result) == 0x8000000000000000UL);
    ASSERT(result.tag == 0);

    //-1.0 + 0.25 = -0.75
    result = fixedpoint_add(objs->neg_one, objs->one_fourth);
    ASSERT(fixedpoint_whole_part(result) == 0UL);
    ASSERT(fixedpoint_frac_part(result) == (0x8000000000000000UL + 0x4000000000000000UL));
    ASSERT(result.tag == 1);
}

//pos + pos add one to whole

void pos_add_pos_frac_overflow(TestObjs *objs) {
    Fixedpoint result;
    //1.75 + 0.5
    result = fixedpoint_add(objs->one_and_three_fourth, objs->one_half);
    ASSERT(fixedpoint_whole_part(result) == 2UL);
    ASSERT(fixedpoint_frac_part(result) == 0x4000000000000000UL);
    ASSERT(result.tag == 0);
}

//add zero value no change

void positive_add_zero(TestObjs *objs) {
    Fixedpoint result;
    result = fixedpoint_add(objs->zero, objs->one_half);
    ASSERT(fixedpoint_whole_part(result) == 0UL);
    ASSERT(fixedpoint_frac_part(result) == 0x8000000000000000UL);
    ASSERT(result.tag == 0);
}

void negative_add_zero(TestObjs *objs) {
    Fixedpoint result;
    result = fixedpoint_add(objs->zero, objs->neg_one_half);
    ASSERT(fixedpoint_whole_part(result) == 0UL);
    ASSERT(fixedpoint_frac_part(result) == 0x8000000000000000UL);
    ASSERT(result.tag == 1);
}

void neg_add_neg_frac_overflow(TestObjs *objs) {
    Fixedpoint result;
    //-1.75 + -0.5
    result = fixedpoint_add(objs->neg_one_and_three_fourth, objs->neg_one_half);
    ASSERT(fixedpoint_whole_part(result) == 2UL);
    ASSERT(fixedpoint_frac_part(result) == 0x4000000000000000UL);
    ASSERT(result.tag == 1);
}

void pos_add_neg_frac_overflow(TestObjs *objs) {
    Fixedpoint result;
    //positive result -0.5 + 1.25 = 0.75
    result = fixedpoint_add(objs->neg_one_half, objs->one_and_one_fourth);
    ASSERT(fixedpoint_whole_part(result) == 0UL);
    ASSERT(fixedpoint_frac_part(result) == 0x8000000000000000UL + 0x4000000000000000UL);
    ASSERT(result.tag == 0);

    //negative result 0.5 + -1.25 = -0.75
    result = fixedpoint_add(objs->one_half, objs -> neg_one_and_one_fourth);
    ASSERT(fixedpoint_whole_part(result) == 0UL);
    ASSERT(fixedpoint_frac_part(result) == 0x8000000000000000UL + 0x4000000000000000UL);
    ASSERT(result.tag == 1);

}


void pos_add_pos_whole_overflow_pos(TestObjs *objs) {
    Fixedpoint result;
    result = fixedpoint_add(objs->max, objs->one);
    ASSERT(fixedpoint_whole_part(result) == 0UL);
    ASSERT(fixedpoint_frac_part(result) == 0xFFFFFFFFFFFFFFFFUL);
    ASSERT(result.tag == 3);
}

void neg_add_neg_whole_overflow_pos(TestObjs *objs) {
    Fixedpoint result;
    result = fixedpoint_add(objs->neg_max, objs->neg_one);
    ASSERT(fixedpoint_whole_part(result) == 0UL);
    ASSERT(fixedpoint_frac_part(result) == 0xFFFFFFFFFFFFFFFFUL);
    ASSERT(result.tag == 4);
}

void pos_add_pos_whole_and_frac_overflow(TestObjs *objs) {
    Fixedpoint max_and_three_fourth = fixedpoint_add(objs->max_empty_frac, objs->three_fourth);
    Fixedpoint result;
    result = fixedpoint_add(max_and_three_fourth, objs->one_and_three_fourth);
    ASSERT(fixedpoint_whole_part(result) == 1UL);
    ASSERT(fixedpoint_frac_part(result) == 0x8000000000000000UL);
    ASSERT(result.tag == 3);
}

void neg_add_neg_whole_and_frac_overflow(TestObjs *objs) {
    Fixedpoint max_and_three_fourth = fixedpoint_add(objs->max_empty_frac, objs->three_fourth);
    Fixedpoint neg_max_and_three_fourth = fixedpoint_negate(max_and_three_fourth);
    Fixedpoint result;
    result = fixedpoint_add(neg_max_and_three_fourth, objs->neg_one_and_three_fourth);
    ASSERT(fixedpoint_whole_part(result) == 1UL);
    ASSERT(fixedpoint_frac_part(result) == 0x8000000000000000UL);
    ASSERT(result.tag == 4);
}

void pos_add_pos_frac_overflow_lead_to_whole_overflow(TestObjs *objs) {
    Fixedpoint max_and_three_fourth = fixedpoint_add(objs->max_empty_frac, objs->three_fourth);
    Fixedpoint result;
    result = fixedpoint_add(max_and_three_fourth, objs->one_half);
    ASSERT(fixedpoint_whole_part(result) == 0UL);
    ASSERT(fixedpoint_frac_part(result) == 0x4000000000000000UL);
    ASSERT(result.tag == 3);
}

void neg_add_neg_frac_overflow_lead_to_whole_overflow(TestObjs *objs) {
    Fixedpoint max_and_three_fourth = fixedpoint_add(objs->max_empty_frac, objs->three_fourth);
    Fixedpoint neg_max_and_three_fourth = fixedpoint_negate(max_and_three_fourth);
    Fixedpoint result;
    result = fixedpoint_add(neg_max_and_three_fourth, objs->neg_one_half);
    ASSERT(fixedpoint_whole_part(result) == 0UL);
    ASSERT(fixedpoint_frac_part(result) == 0x4000000000000000UL);
    ASSERT(result.tag == 4);
}

void add_large(TestObjs *objs) {
    Fixedpoint result;
    result = fixedpoint_add(objs->large1, objs->large2);
    ASSERT(fixedpoint_whole_part(result) == 0x4C16AF0BFUL);
    ASSERT(fixedpoint_frac_part(result) == 0x4E06BDE073C7UL);
    ASSERT(result.tag == 0);
}

void add_same_mag_diff_sign(TestObjs *objs) {
    Fixedpoint result = fixedpoint_add(objs->neg_one, objs->one);
    ASSERT(fixedpoint_whole_part(result) == 0);
    ASSERT(fixedpoint_frac_part(result) == 0);
    ASSERT(result.tag == 0);
}

void add_result_is_zero(TestObjs *objs) {
    Fixedpoint result = fixedpoint_add(objs->zero, objs->neg_zero);
    ASSERT(fixedpoint_whole_part(result) == 0);
    ASSERT(fixedpoint_frac_part(result) == 0);
    ASSERT(result.tag == 0);
}
//pos + pos = max + frac overflow lead to whole over flow

//neg + neg = max + frac lead to neg overflow
