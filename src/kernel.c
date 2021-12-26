#include "kernel.h"
#include "idt/idt.h"
#include "std/io.h"
#include "std/print.h"

void kernel_main() {
  idt_init();
  terminal_initialize();

  println("Hello world");
  println("This should be on a new line");

  outb(0x60, 0xff);
}
