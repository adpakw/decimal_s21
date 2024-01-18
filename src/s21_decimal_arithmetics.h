#ifndef S21_DECIMAL_ARITHMETICS
#define S21_DECIMAL_ARITHMETICS

#include <stdio.h>

#include "s21_cast.h"
#include "s21_decimal.h"
#include "s21_decimal_helper.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// заполнить все нулями
void zero_s21_decimal(s21_decimal *value);

void init_big(s21_decimal value, s21_big_decimal *big);
int get_bit_big(s21_big_decimal *num, int cur_bit);
void normalize_big(s21_big_decimal *bvalue_1, s21_big_decimal *bvalue_2);
int multiply_10_mantis_big(s21_big_decimal *bvalue, int def);
int multiply_mantis_big(s21_big_decimal bvalue_1, s21_big_decimal *bvalue_2,
                        s21_big_decimal *result);
void zero_big_decimal(s21_big_decimal *result);
void zero_mantisa_big(s21_big_decimal *result);
int shift_left_big(s21_big_decimal *bvalue, int def);
int sum_mantissa(s21_big_decimal *bvalue_1, s21_big_decimal *bvalue_2,
                 s21_big_decimal *result);
void sub_mantis_big(s21_big_decimal value_1, s21_big_decimal value_2,
                    s21_big_decimal *result);
int compare_mantis_big(s21_big_decimal *bvalue1, s21_big_decimal *bvalue2);
int big_to_s21decimal(s21_decimal *result, s21_big_decimal *result_big);
void big_to_s21decimal_95(s21_big_decimal *result_big, s21_decimal *result);
void zeroes_left_big(s21_big_decimal *bvalue);
unsigned int division_with_rest_for10(s21_big_decimal val1,
                                      s21_big_decimal *res);
int is_zero_big_decimal(s21_big_decimal big);
int is_greater_or_equal_big_decimal(s21_big_decimal value_1,
                                    s21_big_decimal value_2);
void bank_round(s21_big_decimal copy_result_big_x, s21_big_decimal *result_big,
                int exp_diff);
int go_beyond_big_decimal_s21(s21_big_decimal *big);
#endif