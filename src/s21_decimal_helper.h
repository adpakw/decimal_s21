#ifndef S21_DECIMAL_HELPER_H
#define S21_DECIMAL_HELPER_H

#include <stdio.h>

#include "s21_decimal.h"

int get_part(int bit);
uint get_bit(s21_decimal dnum, int bit);
void set_bit(s21_decimal *dnum, int bit, int value);
uint int_to_bin(uint k);
int get_exp_in_bin(s21_decimal dnum);
int get_exp(s21_decimal dnum);
int set_exp(s21_decimal *num, int exp);
int compare(uint a, uint b);
int get_sign(s21_decimal dnum);
void set_sign(s21_decimal *dnum, int sign);

void s21_shift_left(s21_decimal *number);
uint s21_get_highestbit(s21_decimal dnum);
void s21_shift_right(s21_decimal *number);

int get_bit_big(s21_big_decimal *num, int cur_bit);
void set_bit_big(s21_big_decimal *num, int bit, int result);
#endif