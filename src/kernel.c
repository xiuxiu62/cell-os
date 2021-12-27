#include "kernel.h"
#include "idt/idt.h"
#include "std/io.h"
#include "std/mem/conv.h"
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

  // Switch to kernel paging
  uint32_t *directory = paging_chunk_get_directory(kernel_chunk);
  paging_switch(directory);
  println("Kernel paging: [ok]");

  char *ptr = kzalloc(KB(4));
  uint32_t *chunk_directory = paging_chunk_get_directory(kernel_chunk);
  uint32_t entry_val = ((uint32_t)ptr | PAGING_ACCESS_FROM_ALL |
                        PAGING_IS_PRESENT | PAGING_IS_WRITABLE);
  paging_set(chunk_directory, (void *)0x1000, entry_val);

  // Enable paging
  enable_paging();
  println("Enable paging: [ok]");

  // Enable Interrupts
  enable_interrupts();
  println("Enable interrupts: [ok]");

  println("Kernel loaded successfully\n");
}

void kernel_main() {
  kernel_init();

  void *ptr = kmalloc(50);
  ptr = "Hello world :)";

  println(ptr);
  kfree(ptr);
}
