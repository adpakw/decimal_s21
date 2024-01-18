#ifndef S21_DECIMAL_COMPARISON
#define S21_DECIMAL_COMPARISON

#include <stdio.h>

#include "s21_decimal.h"
#include "s21_decimal_helper.h"

#define FALSE 0
#define TRUE 1

int s21_is_less(s21_decimal a, s21_decimal b);
int s21_is_less_or_equal(s21_decimal a, s21_decimal b);
int s21_is_greater(s21_decimal a, s21_decimal b);
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);
int s21_is_equal(s21_decimal a, s21_decimal b);
int s21_is_not_equal(s21_decimal a, s21_decimal b);
#endif