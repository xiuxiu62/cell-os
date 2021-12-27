#include "kernel.h"
#include "idt/idt.h"
#include "std/io.h"
#include "std/mem/kheap.h"
#include "std/mem/paging.h"
#include "std/print.h"

static struct paging_chunk *kernel_chunk = 0;

void kernel_init() {
  // Initialize VGA adapter
  terminal_init();
  println("VGA adapter: [ok]");

  // Initialize kernel heap
  kheap_init();
  println("Kernel heap: [ok]");

  // Initialize the interrupt descriptor table
  idt_init();
  println("Interrupt descriptor table: [ok]");

  // Setup paging
  uint_fast8_t paging_flags =
      (PAGING_IS_WRITABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
  kernel_chunk = create_paging_chunk(paging_flags);
  println("Initialize paging: [ok]");

  // Enable Interrupts
  enable_interrupts();
  println("Enable interrupts: [ok]");

  println("Kernel loaded successfully\n");
}

void kernel_main() {
  kernel_init();

  println("Hello world");

  void *ptr = kmalloc(50);
  void *ptr_2 = kmalloc(1000);

  kfree(ptr);
  kfree(ptr_2);
}
