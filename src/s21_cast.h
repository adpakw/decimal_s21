#ifndef S21_CAST_H
#define S21_CAST_H
#include "s21_decimal.h"
#include "s21_decimal_arithmetics.h"
#include "s21_decimal_helper.h"

#define SUCCESS 0
#define FAIL 1

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

#endif