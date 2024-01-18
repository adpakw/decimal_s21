#include "s21_cast.h"
#include <stdlib.h>
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  zero_s21_decimal(dst);
  dst->bits[0] = abs(src);
  if (src < 0) {
    set_sign(dst, 1);
  }
  return 0;
}

// int s21_from_float_to_decimal(float src, s21_decimal *dst) {}
// int s21_from_decimal_to_int(s21_decimal src, int *dst) {
//   *dst = 0;
//   uint exp = get_exp(src);
//   s21_decimal buffer =
//       src; //тут неуверен может всё поломаться, но тесты для нубов
//   while (exp > 0) {
//     s21_shift_right(&buffer);
//     exp--;
//   }
//   if (s21_get_highestbit(buffer) > 31 ||
//       (s21_get_highestbit(buffer) == 31 && get_sign(buffer) == 1))
//     return FAIL;
//   else {
//     int sign = get_sign(buffer) ? 1 : -1;
//     *dst = (int)buffer.bits[0] * sign;
//   }
//   return SUCCESS;
// }
// int s21_from_decimal_to_float(s21_decimal src, float *dst) {}
