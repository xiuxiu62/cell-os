#ifndef KERNEL_H
#define KERNEL_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

void terminal_initialize();
void print(const char *str);
void println(const char *str);
void print_color(const char *str, char color);
void println_color(const char *str, char color);

#endif
