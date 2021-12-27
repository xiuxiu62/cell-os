#include "kernel.h"
#include "idt/idt.h"
#include "io/io.h"
#include "mem/conv.h"
#include "mem/kheap.h"
#include "mem/paging.h"
#include "print.h"
#include "storage/disk.h"

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

  char *buf = kmalloc(512);
  disk_read_sector(0, 1, buf);
  for (int i = 0; i < 512; i++)
    buf[i] = (unsigned char)(buf[i]);

  println(buf);
  kfree(buf);
}
