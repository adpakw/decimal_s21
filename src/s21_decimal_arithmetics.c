#include "s21_decimal_arithmetics.h"
#include <check.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#define SUITE_NAME "Unit"
#define TCASE_NAME "Tests"
#define S21_OK 0
#define S21_ERROR 1

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;
  if (result) {
    int equal_signs = 0;
    zero_s21_decimal(result);

    s21_big_decimal bvalue_1 = {0};
    init_big(value_1, &bvalue_1);
    s21_big_decimal bvalue_2 = {0};
    init_big(value_2, &bvalue_2);
    s21_big_decimal result_b = {0};

    normalize_big(&bvalue_1, &bvalue_2);

    if (get_sign(value_1) == get_sign(value_2))
      equal_signs = 1;

    if (equal_signs) {
      sum_mantissa(&bvalue_1, &bvalue_2, &result_b);
      result_b.sign = bvalue_1.sign;
    } else {
      int compare = compare_mantis_big(&bvalue_1, &bvalue_2);
      if (compare > 0) {
        sub_mantis_big(bvalue_1, bvalue_2, &result_b);
        result_b.sign = bvalue_1.sign;
      } else if (compare < 0) {
        sub_mantis_big(bvalue_2, bvalue_1, &result_b);
        result_b.sign = bvalue_2.sign;
      }
    }

    result_b.exponenta = bvalue_1.exponenta;
    status = big_to_s21decimal(result, &result_b);

  } else {
    status = 1;
  }
  return status;
}

int big_to_s21decimal(s21_decimal *result, s21_big_decimal *result_big) {
  int status = 0;
  zeroes_left_big(result_big);
  s21_big_decimal ten = {0};
  ten.bits[0] = 10;
  int diff = 0;
  s21_big_decimal copy_result_big_x = *result_big;
  while (result_big->exponenta > 28) {
    if (compare_mantis_big(result_big, &ten) >= 0) {
      division_with_rest_for10(*result_big, result_big);
      diff++;
    } else {
      status = 2;
      zero_s21_decimal(result);
      break;
    }
  }
  if (!status) {
    if (diff > 0)
      bank_round(copy_result_big_x, result_big, diff);

    diff = 0;
    if (go_beyond_big_decimal_s21(result_big)) {
      if (result_big->exponenta < 1) {
        status = 1;
      } else {
        copy_result_big_x = *result_big;
        while (go_beyond_big_decimal_s21(result_big) &&
               (result_big->exponenta > 0)) {
          division_with_rest_for10(*result_big, result_big);
          diff++;
        }
        if (diff > 0)
          bank_round(copy_result_big_x, result_big, diff);
      }
    }

    if (go_beyond_big_decimal_s21(result_big))
      status = 1;

    if ((status == 1) && result_big->sign)
      status = 2;

    if (!status) {
      if (result_big->sign)
        set_sign(result, 1);
      big_to_s21decimal_95(result_big, result);
      set_exp(result, result_big->exponenta);
    }
  }
  return status;
}

void bank_round(s21_big_decimal copy_result_big_x, s21_big_decimal *result_big,
                int exp_diff) {
  s21_big_decimal one = {0};
  one.bits[0] = 1;
  s21_big_decimal ostatok_xy = {0};
  s21_big_decimal half = {0};
  half.bits[0] = 5;
  half.exponenta = 1;

  if (exp_diff > 0)
    exp_diff--;
  s21_big_decimal copy_result_big_y = *result_big;
  normalize_big(&copy_result_big_x, &copy_result_big_y);

  sub_mantis_big(copy_result_big_x, copy_result_big_y, &ostatok_xy);
  ostatok_xy.sign = 0;
  multiply_10_mantis_big(&half, exp_diff);

  int compare_res = compare_mantis_big(&ostatok_xy, &half);
  if (compare_res == 1) {
    sum_mantissa(result_big, &one, result_big);
  } else if (compare_res == 0) {
    if (get_bit_big(result_big, 0) == 1)
      sum_mantissa(result_big, &one, result_big);
  }
}

unsigned int division_with_rest_for10(s21_big_decimal val1,
                                      s21_big_decimal *res) {
  s21_big_decimal val2 = {0};
  val2.bits[0] = 10;
  zeroes_left_big(&val2);
  int q = 0;
  s21_big_decimal part = {0};
  s21_big_decimal part_next = {0};
  s21_big_decimal sum = {0};
  s21_big_decimal before_sum = {0};
  while (is_zero_big_decimal(val1) &&
         is_greater_or_equal_big_decimal(val1, val2)) {
    q = 0;
    zero_big_decimal(&part);
    zero_big_decimal(&before_sum);
    part_next = val2;
    part = val2;
    zeroes_left_big(&val1);
    while (is_greater_or_equal_big_decimal(val1, part_next)) {
      if (q == 0) {
        int difference_elder_bit =
            val1.one_position_left - val2.one_position_left;
        if (difference_elder_bit > 2) {
          q = difference_elder_bit - 1;
          shift_left_big(&part_next, q);
        }
      }
      part = part_next;
      shift_left_big(&part_next, 1);
      q++;
    }
    q--;
    set_bit_big(&before_sum, q, 1);
    sum_mantissa(&sum, &before_sum, &sum);
    if (is_greater_or_equal_big_decimal(val1, part))
      sub_mantis_big(val1, part, &val1);
  }

  sum.exponenta = val1.exponenta - 1;
  sum.sign = val1.sign;
  *res = sum;
  return val1.bits[0];
}

int go_beyond_big_decimal_s21(s21_big_decimal *big) {
  int result = 0;
  for (int i = 3; i < 8; i++) {
    if (big->bits[i] != 0) {
      result = 1;
      break;
    }
  }

  return result;
}

void zero_s21_decimal(s21_decimal *value) {
  value->bits[0] = value->bits[1] = value->bits[2] = value->bits[3] = 0;
}

int is_greater_or_equal_big_decimal(s21_big_decimal value_1,
                                    s21_big_decimal value_2) {
  int result = 1;
  for (int i = 7; i >= 0; i--) {
    if (value_1.bits[i] > value_2.bits[i]) {
      result = 1;
      break;
    } else if (value_1.bits[i] < value_2.bits[i]) {
      result = 0;
      break;
    }
  }
  return result;
}

int is_greater_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2) {
  int result = 0, out = 0;
  for (int i = 7; i >= 0 && !result && !out; i--) {
    if (value_1.bits[i] || value_2.bits[i]) {
      if (value_1.bits[i] > value_2.bits[i]) {
        result = 1;
      }
      if (value_1.bits[i] != value_2.bits[i])
        out = 1;
    }
  }
  return result;
}

int is_zero_big_decimal(s21_big_decimal big) {
  int result = 0;
  for (int i = 7; i >= 0; i--) {
    if (big.bits[i] != 0) {
      result = 1;
      break;
    }
  }
  return result;
}

int is_zero_s21_decimal(s21_decimal value) {
  int res = 0;
  for (int i = 2; i >= 0; i--) {
    if (value.bits[i] != 0) {
      res = 1;
      break;
    }
  }
  return res;
}

void sub_mantis_big(s21_big_decimal value_1, s21_big_decimal value_2,
                    s21_big_decimal *result) {
  int tmp = 0, res = 0;
  for (int i = 0; i <= 255; i++) {
    res = get_bit_big(&value_1, i) - get_bit_big(&value_2, i) - tmp;
    tmp = res < 0;
    res = abs(res);
    set_bit_big(result, i, res % 2);
  }
}

int multiply_10_mantis_big(s21_big_decimal *bvalue, int def) {
  int status = 0;

  s21_decimal ten_s = {0};
  s21_from_int_to_decimal(10, &ten_s);
  s21_big_decimal ten = {0};
  init_big(ten_s, &ten);

  for (int i = 1; i <= def; i++) {
    if (multiply_mantis_big(*bvalue, &ten, bvalue))
      status = 1;
  }
  bvalue->exponenta += def;
  return status;
}

int compare_mantis_big(s21_big_decimal *bvalue1, s21_big_decimal *bvalue2) {
  int result = 0;
  for (int i = 255; i >= 0; i--) {
    int rvalue1 = 0, rvalue2 = 0;
    rvalue1 = get_bit_big(bvalue1, i);
    rvalue2 = get_bit_big(bvalue2, i);
    if (rvalue1 != rvalue2) {
      result = rvalue1 - rvalue2;
      break;
    }
  }
  return result;
}

void big_to_s21decimal_95(s21_big_decimal *result_big, s21_decimal *result) {
  for (int i = 0; i < 3; i++) {
    result->bits[i] = result_big->bits[i];
  }
}

int sum_mantissa(s21_big_decimal *bvalue_1, s21_big_decimal *bvalue_2,
                 s21_big_decimal *result) {
  int status = 0;
  int tmp = 0;
  int var = 0;

  for (int i = 0; i <= 255; i++) {
    var = (get_bit_big(bvalue_1, i) + get_bit_big(bvalue_2, i) + tmp);
    if (var == 3) {
      tmp = 1;
      set_bit_big(result, i, 1);
    } else if (var == 2) {
      tmp = 1;
      set_bit_big(result, i, 0);
    } else if (var == 1) {
      set_bit_big(result, i, 1);
      tmp = 0;
    } else if (var == 0) {
      set_bit_big(result, i, 0);
      tmp = 0;
    }
  }
  if (tmp == 1)
    status = 1;
  return status;
}

void normalize_big(s21_big_decimal *bvalue_1, s21_big_decimal *bvalue_2) {
  int def = bvalue_1->exponenta - bvalue_2->exponenta;
  if (def > 0) {
    multiply_10_mantis_big(bvalue_2, def);
    zeroes_left_big(bvalue_2);
  } else if (def < 0) {
    multiply_10_mantis_big(bvalue_1, -def);
    zeroes_left_big(bvalue_1);
  }
}

int multiply_mantis_big(s21_big_decimal bvalue_1, s21_big_decimal *bvalue_2,
                        s21_big_decimal *result) {
  int status = 0;

  zero_big_decimal(result);

  zeroes_left_big(bvalue_2);
  zeroes_left_big(&bvalue_1);

  for (int i = 0; i <= bvalue_2->one_position_left; i++) {
    if (i != 0)
      if (shift_left_big(&bvalue_1, 1) == 1) {
        status = 1;
        break;
      }
    if (get_bit_big(bvalue_2, i))
      if (sum_mantissa(result, &bvalue_1, result))
        status = 1;
  }

  int equal_znak = (bvalue_1.sign == bvalue_2->sign);
  if (!equal_znak)
    result->sign = 1;

  result->exponenta = bvalue_1.exponenta + bvalue_2->exponenta;

  return status;
}

void zero_mantisa_big(s21_big_decimal *result) {
  for (int i = 0; i < 8; i++) {
    result->bits[i] = 0;
  }
}

void zero_big_decimal(s21_big_decimal *result) {
  zero_mantisa_big(result);
  result->exponenta = 0;
  result->sign = 0;
  result->one_position_left = 0;
  result->one_right = 0;
}

int shift_left_big(s21_big_decimal *bvalue, int def) {
  int status = 0;

  zeroes_left_big(bvalue);

  if ((255 - bvalue->one_position_left) < def)
    status = 1;

  for (int i = bvalue->one_position_left; i >= 0; i--) {
    if ((i + def) <= 255) {
      set_bit_big(bvalue, i + def, get_bit_big(bvalue, i));
    }
  }
  for (int i = 0; i < def; i++) {
    set_bit_big(bvalue, i, 0);
  }
  zeroes_left_big(bvalue);

  return status;
}

void zeroes_left_big(s21_big_decimal *bvalue) {
  int i = 255;
  while (1) {
    if (get_bit_big(bvalue, i) != 0) {
      bvalue->one_position_left = i;
      break;
    }
    i--;
    if (!(i >= 0))
      break;
  }
  if (i == -1) {
    bvalue->one_position_left = -1;
  }
  for (int j = 0; j <= 255; j++) {
    if (get_bit_big(bvalue, j) != 0) {
      bvalue->one_right = j;
      break;
    }
  }
}

void init_big(s21_decimal value, s21_big_decimal *big) {
  big->exponenta = get_exp(value);
  big->sign = get_sign(value);
  big->bits[0] = value.bits[0];
  big->bits[1] = value.bits[1];
  big->bits[2] = value.bits[2];
  zeroes_left_big(big);
}

START_TEST(add_1_test) {
  s21_decimal num = {{1, 0, 0, 0}};
  s21_decimal num2 = {{1, 0, 0, 0}};
  s21_decimal res = {0};
  s21_decimal answer = {{2, 0, 0, 0}};

  s21_add(num, num2, &res);
  ck_assert_int_eq(res.bits[0], answer.bits[0]);
  ck_assert_int_eq(res.bits[1], answer.bits[1]);
  ck_assert_int_eq(res.bits[2], answer.bits[2]);
  ck_assert_int_eq(res.bits[3], answer.bits[3]);
}
END_TEST

START_TEST(add_2_test) {
  s21_decimal num1 = {{-1, -1, 56648, 0}};
  s21_decimal num2 = {{44, 369, 1687, 0}};
  s21_decimal res = {{0, 0, 0, 0}};

  int status = s21_add(num1, num2, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 43);
  ck_assert_int_eq(res.bits[1], 369);
  ck_assert_int_eq(res.bits[2], 58336);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(add_3_test) {
  s21_decimal num1 = {{-1, -1, -1688, 0}};
  s21_decimal num2 = {{44, 369, 1687, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_add(num1, num2, &res);
  ck_assert_int_eq(status, 1);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(add_4_test) {
  s21_decimal num1 = {
      {(unsigned int)-1, (unsigned int)-1, 56648, (unsigned int)-2147483648}};
  s21_decimal num2 = {{44, 369, 1687, (unsigned int)-2147483648}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_add(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 43);
  ck_assert_int_eq(res.bits[1], 369);
  ck_assert_int_eq(res.bits[2], 58336);
  ck_assert_int_eq(res.bits[3], (unsigned int)-2147483648);
}
END_TEST

START_TEST(add_5_test) {
  s21_decimal num1 = {{(unsigned int)-1, (unsigned int)-1, (unsigned int)-1688,
                       (unsigned int)-2147483648}};
  s21_decimal num2 = {{44, 369, 1687, (unsigned int)-2147483648}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_add(num1, num2, &res);
  ck_assert_int_eq(status, 2);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(add_6_test) {
  s21_decimal num1 = {{45, 0, 10, (unsigned int)-2147483648}};
  // print_decimal(num1);
  s21_decimal num2 = {{67, 0, 0, (unsigned int)-2147352576}};
  // print_decimal(num2);
  s21_decimal res = {{0, 0, 0, 0}};

  int status = s21_add(num1, num2, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 4567);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 1000);
  ck_assert_int_eq(res.bits[3], (unsigned int)-2147352576);
}
END_TEST

START_TEST(add_7_test) {
  s21_decimal num1 = {{45, 0, 10, 65536}};

  s21_decimal num2 = {{67, 0, 0, 131072}};

  s21_decimal res = {{0, 0, 0, 0}};

  int status = s21_add(num1, num2, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 517);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 100);
  ck_assert_int_eq(res.bits[3], 131072);
}
END_TEST

START_TEST(add_8_test) {
  s21_decimal num1 = {
      {(unsigned int)-2, (unsigned int)-1, (unsigned int)-1, 0}};
  s21_decimal num2 = {{5, 0, 0, 65536}};
  s21_decimal res = {{0, 0, 0, 0}};

  int status = s21_add(num1, num2, &res);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], (unsigned int)-2);
  ck_assert_int_eq(res.bits[1], (unsigned int)-1);
  ck_assert_int_eq(res.bits[2], (unsigned int)-1);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(add_9_test) {
  s21_decimal num1 = {{(unsigned int)-3, (unsigned int)-1, (unsigned int)-1,
                       (unsigned int)-2147483648}};
  s21_decimal num2 = {{7, 0, 0, (unsigned int)-2147418112}};
  s21_decimal res = {{2, 1, 4, 65536}};
  //   print_decimal(num1);
  // print_decimal(num2);
  // printf("\n!!!!!!!!\n");
  int status = s21_add(num1, num2, &res);
  // s21_decimal res2 = {{(unsigned int)-2, (unsigned int)-1, (unsigned
  // int)-1, (unsigned int)-2147483648}}; printf("\n!!!!!!!!\n");

  // print_decimal(res);
  // print_decimal(res2);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], (unsigned int)-2);
  ck_assert_int_eq(res.bits[1], (unsigned int)-1);
  ck_assert_int_eq(res.bits[2], (unsigned int)-1);
  ck_assert_int_eq(res.bits[3], (unsigned int)-2147483648);
}
END_TEST

START_TEST(add_10_test) {
  s21_decimal num1 = {{0, 0, 10, 0}};
  s21_decimal num2 = {{6, 0, 0, 589824}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_add(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 1);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 1000000000);
  ck_assert_int_eq(res.bits[3], 524288);
}
END_TEST

START_TEST(add_11_test) {
  s21_decimal num1 = {{0, 0, 13, (unsigned int)-2147483648}};
  s21_decimal num2 = {{4, 0, 0, (unsigned int)-2146893824}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_add(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 1300000000);
  ck_assert_int_eq(res.bits[3], (unsigned int)-2146959360);
}
END_TEST

START_TEST(add_12_test) {
  s21_decimal num1 = {{6, 7, 8, 0}};
  s21_decimal num2 = {{5, 4, 3, (unsigned int)-2147483648}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_add(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 1);
  ck_assert_int_eq(res.bits[1], 3);
  ck_assert_int_eq(res.bits[2], 5);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(add_13_test) {
  s21_decimal num1 = {{3, 1, 3, 0}};
  s21_decimal num2 = {{5, 4, 8, (unsigned int)-2147483648}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_add(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 2);
  ck_assert_int_eq(res.bits[1], 3);
  ck_assert_int_eq(res.bits[2], 5);
  ck_assert_int_eq(res.bits[3], (unsigned int)-2147483648);
}
END_TEST

START_TEST(add_14_test) {
  s21_decimal num1 = {{6, 7, 8, (unsigned int)-2147483648}};
  s21_decimal num2 = {{5, 4, 3, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_add(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 1);
  ck_assert_int_eq(res.bits[1], 3);
  ck_assert_int_eq(res.bits[2], 5);
  ck_assert_int_eq(res.bits[3], (unsigned int)-2147483648);
}
END_TEST

START_TEST(add_15_test) {
  s21_decimal num1 = {{3, 1, 3, (unsigned int)-2147483648}};
  s21_decimal num2 = {{5, 4, 8, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_add(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 2);
  ck_assert_int_eq(res.bits[1], 3);
  ck_assert_int_eq(res.bits[2], 5);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(add_16_test) {
  s21_decimal num1 = {{0, 0, 0, 0}};
  s21_decimal num2 = {{5, 0, 0, 65536}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_add(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 5);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 65536);
}
END_TEST

START_TEST(add_error) {
  s21_decimal src1 = {{0, 0, 0, 0}};
  s21_decimal src2 = {{0, 0, 0, 0}};
  s21_decimal *result_our = NULL;
  int val = s21_add(src1, src2, result_our);
  ck_assert_int_eq(val, 1);
}
END_TEST
int main() {
  Suite *suite = suite_create(SUITE_NAME);
  TCase *tcase = tcase_create(TCASE_NAME);
  SRunner *srunner = srunner_create(suite);
  int nf;
  suite_add_tcase(suite, tcase);

  tcase_add_test(tcase, add_1_test);
  tcase_add_test(tcase, add_2_test);
  tcase_add_test(tcase, add_3_test);
  tcase_add_test(tcase, add_4_test);
  tcase_add_test(tcase, add_5_test);
  tcase_add_test(tcase, add_6_test);
  tcase_add_test(tcase, add_7_test);
  tcase_add_test(tcase, add_8_test);
  tcase_add_test(tcase, add_9_test);
  tcase_add_test(tcase, add_10_test);
  tcase_add_test(tcase, add_11_test);
  tcase_add_test(tcase, add_12_test);
  tcase_add_test(tcase, add_13_test);
  tcase_add_test(tcase, add_14_test);
  tcase_add_test(tcase, add_15_test);
  tcase_add_test(tcase, add_16_test);
  tcase_add_test(tcase, add_error);
  srunner_run_all(srunner, CK_ENV);
  nf = srunner_ntests_failed(srunner);
  srunner_free(srunner);

  return nf == 0 ? 0 : 1;
}