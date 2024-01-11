#include "s21_decimal_helper.h"

#include <math.h>
uint int_to_bin(uint k) {
  return (k == 0 || k == 1 ? k : ((k % 2) + 10 * int_to_bin(k / 2)));
}

int get_part(int bit) { return (int)(bit / 32); }

uint get_bit(s21_decimal dnum, int bit) {
  uint working_bits;
  working_bits = dnum.bits[get_part(bit)];
  bit = bit - get_part(bit) * 32;
  return ((working_bits & (1 << bit)) >> bit);
}

void set_bit(s21_decimal *dnum, int bit, int value) {
  int index = get_part(bit);
  int bit_position = bit % 32;
  if (value == 1) {
    dnum->bits[index] |= (1 << bit_position);
  } else {
    dnum->bits[index] &= ~(1 << bit_position);
  }
}

int get_exp(s21_decimal dnum) {
  uint buffer = 0b11111111;
  return int_to_bin(((buffer << 15) & dnum.bits[3]) >> 15);
}
int get_sign(s21_decimal dnum) { return get_bit(dnum, 127); }
void set_sign(s21_decimal dnum, int sign) { set_bit(&dnum, 127, sign); }

int compare(uint a, uint b) {
  uint fb = 1 << (sizeof(a) * 8 - 1);

  while (!((a ^ b) & fb)) {
    a <<= 1;
    b <<= 1;
    if (!a && !b) return 0;
  }
  if (a & fb)
    return 1;
  else
    return -1;
}

// int main() {
//   s21_decimal decimal = {
//       {0b10101010101010101010101010101010,
//       0b10101010101010101010101010101011,
//        0b10101010101010101010101010101010,
//        0b10101010101010101010101010101010}};
//   printf("%u\n", get_exp(decimal));
//   printf("%d", compare(0, 1));
//   return 0;
// }
