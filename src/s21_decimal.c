#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {}
int s21_from_float_to_decimal(float src, s21_decimal *dst) {}
int s21_from_decimal_to_int(s21_decimal src, int *dst) {}
int s21_from_decimal_to_float(s21_decimal src, float *dst) {}

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {}
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {}
int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {}
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {}
int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {}
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {}