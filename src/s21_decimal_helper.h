#ifndef S21_DECIMAL_HELPER_H
#define S21_DECIMAL_HELPER_H

#include <stdio.h>

#include "s21_decimal.h"

int get_part(int bit);
uint get_bit(s21_decimal dnum, int bit);
void set_bit(s21_decimal *dnum, int bit, int value);
uint int_to_bin(uint k);
int get_exp(s21_decimal dnum);
int compare(uint a, uint b);
int get_sign(s21_decimal dnum);
void set_sign(s21_decimal dnum, int sign);
#endif