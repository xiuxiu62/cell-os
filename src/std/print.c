#include "print.h"

#include <stddef.h>
#include <stdint.h>

uint16_t *video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

uint16_t terminal_make_char(char c, enum Color color) {
  return (color << 8) | c;
}

void terminal_putchar(int x, int y, char c, enum Color color) {
  video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, color);
}

static inline void terminal_put_newline() {
  terminal_col = 0;
  terminal_row += 1;
}

void terminal_writechar(char c, enum Color color) {
  if (c == '\n') {
    terminal_put_newline();
    return;
  }

  terminal_putchar(terminal_col, terminal_row, c, color);
  terminal_col += 1;

  if (terminal_col > VGA_WIDTH) {
    terminal_put_newline();
  }
}

void terminal_init() {
  video_mem = (uint16_t *)(0xb8000);
  for (int y = 0; y < VGA_HEIGHT; y++)
    for (int x = 0; x < VGA_WIDTH; x++)
      terminal_putchar(x, y, ' ', 0);
}

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len]) {
    len++;
  }

  return len;
}

void print(const char *str) {
  size_t len = strlen(str);
  for (int i = 0; i < len; i++)
    terminal_writechar(str[i], White);
}

void println(const char *str) {
  size_t len = strlen(str);
  for (int i = 0; i < len; i++)
    terminal_writechar(str[i], White);

  terminal_writechar('\n', White);
}

void print_color(const char *str, enum Color color) {
  size_t len = strlen(str);
  for (int i = 0; i < len; i++)
    terminal_writechar(str[i], color);
}

void println_color(const char *str, enum Color color) {
  size_t len = strlen(str);
  for (int i = 0; i < len; i++)
    terminal_writechar(str[i], color);

  terminal_writechar('\n', color);
}
