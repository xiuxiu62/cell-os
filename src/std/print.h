#ifndef PRINT_H
#define PRINT_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

enum Color {
  Black = 0,
  Blue = 1,
  Green = 2,
  Cyan = 3,
  Red = 4,
  Magenta = 5,
  Brown = 6,
  LightGray = 7,
  DarkGray = 8,
  LightBlue = 9,
  LightGreen = 10,
  LightCyan = 11,
  LightRed = 12,
  Pink = 13,
  Yellow = 14,
  White = 15,
};

void terminal_init();

void print(const char *str);
void println(const char *str);
void print_color(const char *str, enum Color color);
void println_color(const char *str, enum Color color);

#endif
