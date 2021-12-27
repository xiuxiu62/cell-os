#include "kheap.h"
#include "../../config.h"
#include "../print.h"
#include "heap.h"
#include "memory.h"

struct heap kernel_heap;
struct heap_table kernel_heap_table;

void kheap_init() {
  int total_table_entries = CELLOS_HEAP_SIZE_BYTES / CELLOS_HEAP_BLOCK_SIZE;

  kernel_heap_table.entries =
      (HEAP_BLOCK_TABLE_ENTRY *)(CELLOS_HEAP_TABLE_ADDRESS);
  kernel_heap_table.total = total_table_entries;

  void *end = (void *)(CELLOS_HEAP_ADDRESS + CELLOS_HEAP_SIZE_BYTES);
  int result = heap_create(&kernel_heap, (void *)(CELLOS_HEAP_ADDRESS), end,
                           &kernel_heap_table);
  if (result < 0) {
    println("Failed to create heap");
  }
}

void *kmalloc(size_t size) { return heap_malloc(&kernel_heap, size); }

void *kzalloc(size_t size) {
  void *ptr = kmalloc(size);
  if (!ptr)
    return 0;

  memset(ptr, 0x00, size);
  return ptr;
}

void kfree(void *ptr) { heap_free(&kernel_heap, ptr); }
