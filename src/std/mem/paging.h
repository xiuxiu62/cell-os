#ifndef PAGING_H
#define PAGING_H

#include "conv.h"
#include <stddef.h>
#include <stdint.h>

#define PAGING_CACHE_DISABLED 0x10
#define PAGING_WRITE_THROUGH 0x08
#define PAGING_ACCESS_FROM_ALL 0x04
#define PAGING_IS_WRITABLE 0x02
#define PAGING_IS_PRESENT 0x01

#define PAGING_TOTAL_ENTRIES_PER_TABLE KB(1)
#define PAGING_PAGE_SIZE KB(4)

struct paging_chunk {
  uint32_t *directory_entry;
};

struct paging_chunk *create_paging_chunk(uint8_t flags);
void paging_switch(uint32_t *directory);
void enable_paging();

#endif
