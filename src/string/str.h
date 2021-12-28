#ifndef STR_H
#define STR_H

#include <stddef.h>
#include <stdint.h>

// struct string {
//   int len;
//   size_t capacity;
//   char *inner;
// };

// struct string string_from_str(char *str) {
//   struct string string;
//   string.inner = str;

//   return string;
// }

int strlen(const char *ptr);

#endif
