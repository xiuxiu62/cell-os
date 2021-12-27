#ifndef PAGING_H
#define PAGING_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PAGING_CACHE_DISABLED 0x10
#define PAGING_WRITE_THROUGH 0x08
#define PAGING_ACCESS_FROM_ALL 0x04
#define PAGING_IS_WRITABLE 0x02
#define PAGING_IS_PRESENT 0x01

#define PAGING_TOTAL_ENTRIES_PER_TABLE 1024
#define PAGING_PAGE_SIZE 4096

struct paging_chunk {
  uint32_t *directory_entry;
};

struct paging_chunk *create_paging_chunk(uint8_t flags);
uint32_t *paging_chunk_get_directory(struct paging_chunk *chunk);

void paging_switch(uint32_t *directory);
void enable_paging();

int paging_set(uint32_t *directory, void *virtual_addr, uint32_t val);
bool paging_is_aligned(void *addr);

#endif
