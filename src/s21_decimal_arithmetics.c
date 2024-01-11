#include "s21_decimal_arithmetics.h"

void to_additional_code(s21_decimal *decimal_) {
  s21_decimal buf = *decimal_;
  for (int i = 0; i < 96; i++) {
    if (get_bit(buf, i) == 1) {
      set_bit(&buf, i, 0);
    } else {
      set_bit(&buf, i, 1);
    }
  }
  s21_decimal addition = {{0b0, 0b0, 0b00000000000000000000000000000001, 0b0}};
  add_mantisa(buf, addition, decimal_);
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_decimal val_1 = value_1;
  s21_decimal val_2 = value_2;
  int sign1 = get_sign(value_1);
  int sign2 = get_sign(value_2);
  if (sign1 == 0 && sign2 == 0) {
    add_mantisa(val_1, val_2, result);
  } else if (sign1 == 1 && sign2 == 1) {
    add_mantisa(val_1, val_2, result);
    set_sign(*result, 1);
  } else if (sign1 == 1 && sign2 == 0) {
    to_additional_code(&val_1);
    add_mantisa(val_1, val_2, result);
  } else if (sign1 == 0 && sign2 == 1) {
    to_additional_code(&val_2);
    add_mantisa(val_1, val_2, result);
  }
  return 0;
}

int add_mantisa(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int overflow = 0;
  for (int i = 0; i < 96; i++) {
    int bit_val_1 = get_bit(value_1, i);
    int bit_val_2 = get_bit(value_2, i);
    int res = bit_val_1 + bit_val_2 + overflow;
    switch (res) {
      case (1):
        set_bit(result, i, res);
        overflow = 0;
        break;
      case (2):
        set_bit(result, i, 0);
        overflow = 1;
        break;
      case (3):
        set_bit(result, i, 1);
        overflow = 1;
        break;
      default:
        set_bit(result, i, 0);
    }
  }
  return overflow;
}

// int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {}
// int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {}
// int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {}

int main() {
  s21_decimal decimal1 = {{0b00000000000000000000000000011111,
                           0b01010101010101010101010101010101,
                           0b10101010101010101010101010101010, 0b0110}};
  s21_decimal decimal2 = {{0b00000000000000000000000000011111,
                           0b10101010101010101010101010101010,
                           0b01010101010101010101010101010101, 0b0110}};

  s21_decimal res;

  add_mantisa(decimal1, decimal2, &res);
  // for (int i = 0; i < 96; i++) {
  //   printf("%u", get_bit(res, i));
  // }
  // printf("\n%d\n", res_overflow);

  to_additional_code(&res);

  for (int i = 95; i > -1; i--) {
    if (i == 31 || i == 63) {
      printf(" ");
    }
    printf("%u", get_bit(res, i));
  }
  printf("\n");

  // s21_decimal decimal = {{0b10101010101010101010101010101010,
  //                         0b10101010101010101010101010101011,
  //                         0b10101010101010101010101010101010, 0b0110}};
  // s21_decimal decimal1 = {{0b10101010101010101010101010101010,
  //                          0b01010101010101010101010101010101,
  //                          0b10101010101010101010101010101010, 0b0110}};
  // s21_decimal decimal2 = {{0b01010101010101010101010101010101,
  //                          0b10101010101010101010101010101010,
  //                          0b01010101010101010101010101010101, 0b0110}};

  // s21_decimal res;

  // s21_add(decimal1, decimal2, &res);
  // for (int i = 0; i < 96; i++) {
  //   printf("%u", get_bit(res, i));
  // }

  // s21_decimal decimal = {{0b11111111111111111111111111111110,
  //                         0b10101010101010101010101010101011,
  //                         0b10101010101010101010101010101010, 0b0110}};
  // s21_decimal decimal1 = {{0b10101010101010101010101010101010,
  //                          0b01010101010101010101010101010101,
  //                          0b10101010101010101010101010101010, 0b0110}};
  // s21_decimal decimal2 = {{0b01010101010101010101010101010101,
  //                          0b10101010101010101010101010101010,
  //                          0b01010101010101010101010101010101, 0b0110}};

  // // s21_decimal res;
  // printf("%u\n", get_bit(decimal, 0));
  // for (int i = 0; i < 96; i++) {
  //   printf("%u", get_bit(decimal, i));
  // }

  // printf("%d\n%d\n%d", int_to_bin(res.bits[0]), int_to_bin(res.bits[1]),
  // int_to_bin(res.bits[2])); printf("%u\n", get_bit(decimal, 32));
  // set_bit(&decimal, 32, 0);
  // printf("%u\n", get_bit(decimal, 32));
  return 0;
}