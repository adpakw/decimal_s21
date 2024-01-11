#ifndef S21_DECIMAL_ARITHMETICS
#define S21_DECIMAL_ARITHMETICS

#include <stdio.h>

#include "s21_decimal.h"
#include "s21_decimal_helper.h"

void to_additional_code(s21_decimal *decimal);
int add_mantisa(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

#endif