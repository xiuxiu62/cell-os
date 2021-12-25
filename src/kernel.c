#include "kernel.h"
#include "std/print.h"

void kernel_main() {
  terminal_initialize();

  println("Hello world");
  println("This should be on a new line");
}
