#include "str.h"
#include "../status.h"
#include <stdbool.h>

int strlen(const char *ptr) {
  int len = 0;
  while (ptr[len])
    len++;

  return len;
}

bool is_digit(char c) { return (c >= 48 && c <= 57); }

// Returns -2 if invalid, else returns the int repr of an ascii char
int int_from_char(char c) { return is_digit(c) ? (c - 48) : -EINVARG; }
