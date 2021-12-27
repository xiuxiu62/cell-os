#include "kernel.h"
#include "idt/idt.h"
#include "std/io.h"
#include "std/mem/kheap.h"
#include "std/print.h"

void kernel_init() {
  // Initialize kernel heap
  kheap_init();
  // Initialize interrupt descriptor table
  idt_init();
  // Initialize VGA terminal
  terminal_init();
}

void kernel_main() {
  kernel_init();

  void *ptr = kmalloc(50);
  void *ptr_2 = kmalloc(1000);

  kfree(ptr);
  kfree(ptr_2);

  println("Hello world");
  println("This should be on a new line");
}
