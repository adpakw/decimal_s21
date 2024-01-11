#include "s21_decimal_comparison.h"

int s21_is_less(s21_decimal a, s21_decimal b) {
  switch (compare(get_sign(a), get_sign(b))) {
    case 0:
      break;
    case 1:
      return FALSE;
      break;
    case -1:
      return TRUE;
      break;
  }
  switch (compare(get_exp(a), get_exp(b))) {
    case 0:
      break;
    case 1:
      return FALSE;
      break;
    case -1:
      return TRUE;
      break;
  }
  for (int i = 3; i > -1; i--) {
    switch (compare(a.bits[i], b.bits[i])) {
      case 0:
        break;
      case 1:
        return FALSE;
        break;
      case -1:
        return TRUE;
        break;
    }
  }
  return FALSE;
}

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
  switch (compare(get_sign(a), get_sign(b))) {
    case 0:
      break;
    case 1:
      return FALSE;
      break;
    case -1:
      return TRUE;
      break;
  }
  switch (compare(get_exp(a), get_exp(b))) {
    case 0:
      break;
    case 1:
      return FALSE;
      break;
    case -1:
      return TRUE;
      break;
  }
  for (int i = 3; i > -1; i--) {
    switch (compare(a.bits[i], b.bits[i])) {
      case 0:
        break;
      case 1:
        return FALSE;
        break;
      case -1:
        return TRUE;
        break;
    }
  }
  return TRUE;
}

int s21_is_greater(s21_decimal a, s21_decimal b) {
  switch (compare(get_sign(a), get_sign(b))) {
    case 0:
      break;
    case 1:
      return TRUE;
      break;
    case -1:
      return FALSE;
      break;
  }
  switch (compare(get_exp(a), get_exp(b))) {
    case 0:
      break;
    case 1:
      return TRUE;
      break;
    case -1:
      return FALSE;
      break;
  }
  for (int i = 3; i > -1; i--) {
    switch (compare(a.bits[i], b.bits[i])) {
      case 0:
        break;
      case 1:
        return TRUE;
        break;
      case -1:
        return FALSE;
        break;
    }
  }
  return FALSE;
}

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  switch (compare(get_sign(a), get_sign(b))) {
    case 0:
      break;
    case 1:
      return TRUE;
      break;
    case -1:
      return FALSE;
      break;
  }
  switch (compare(get_exp(a), get_exp(b))) {
    case 0:
      break;
    case 1:
      return TRUE;
      break;
    case -1:
      return FALSE;
      break;
  }
  for (int i = 3; i > -1; i--) {
    switch (compare(a.bits[i], b.bits[i])) {
      case 0:
        break;
      case 1:
        return TRUE;
        break;
      case -1:
        return FALSE;
        break;
    }
  }
  return TRUE;
}

int s21_is_equal(s21_decimal a, s21_decimal b) {
  if (s21_is_greater_or_equal(a, b) & !s21_is_greater(a, b))
    return TRUE;
  else
    return FALSE;
}
int s21_is_not_equal(s21_decimal a, s21_decimal b) {
  if (!s21_is_equal(a, b))
    return TRUE;
  else
    return FALSE;
}

int main() {
  s21_decimal decimal = {
      {0b10101010101010101010101010101010, 0b10101010101010101010101010101011,
       0b10101010101010101010101010101010, 0b10101010101010101010101010101010}};
  s21_decimal decimal_2 = {
      {0b11111111111111111111111111111111, 0b11111111111111111111111111111111,
       0b11111111111111111111111111111111, 0b10101010101010101010101010101010}};
  printf("%d\n", s21_is_less(decimal_2, decimal));
  printf("%d\n", s21_is_less_or_equal(decimal_2, decimal_2));
  printf("%d\n", s21_is_greater(decimal_2, decimal));
  printf("%d\n", s21_is_greater_or_equal(decimal_2, decimal_2));
  printf("%d\n", s21_is_equal(decimal_2, decimal));
  printf("%d\n", s21_is_not_equal(decimal_2, decimal_2));
}