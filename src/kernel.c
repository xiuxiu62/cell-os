#include "kernel.h"
#include "idt/idt.h"
#include "std/io.h"
#include "std/mem/kheap.h"
#include "std/print.h"

void kernel_init() {
  terminal_init();
  println("Initialize VGA adapter: [ok]");
  // Initialize kernel heap
  kheap_init();
  println("Initialize kernel heap: [ok]");
  // Initialize interrupt descriptor table
  idt_init();
  println("Initialize interrupt descriptor table: [ok]");

  println("Kernel successfully loaded\n");
}

void kernel_main() {
  kernel_init();

  println("Hello world");

  void *ptr = kmalloc(50);
  void *ptr_2 = kmalloc(1000);

  kfree(ptr);
  kfree(ptr_2);
}
