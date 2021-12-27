#include "paging.h"
#include "../status.h"
#include "kheap.h"

static uint32_t *current_directory = 0;

extern void paging_load_directory(uint32_t *directory);

uint32_t *create_entry(int size, uint8_t flags, int offset) {
  uint32_t *entry = kzalloc(size);
  for (int b = 0; b < PAGING_TOTAL_ENTRIES_PER_TABLE; b++)
    entry[b] = (offset + (b * PAGING_PAGE_SIZE)) | flags;

  return entry;
}

uint32_t *create_directory(int entry_size, uint8_t flags) {
  uint32_t *directory = kzalloc(entry_size);
  int offset = 0;
  for (int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; i++) {
    uint32_t *entry = create_entry(entry_size, flags, offset);
    offset += (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE);
    directory[i] = (uint32_t)entry | flags | PAGING_IS_WRITABLE;
  }

  return directory;
}

struct paging_chunk *create_paging_chunk(uint8_t flags) {
  struct paging_chunk *chunk = kzalloc(sizeof(struct paging_chunk));
  int entry_size = sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE;
  chunk->directory_entry = create_directory(entry_size, flags);

  return chunk;
}

void paging_switch(uint32_t *directory) {
  paging_load_directory(directory);
  current_directory = directory;
  //
}

uint32_t *paging_chunk_get_directory(struct paging_chunk *chunk) {
  return chunk->directory_entry;
}

bool paging_is_aligned(void *addr) {
  return (((uint32_t)addr % PAGING_PAGE_SIZE) == 0);
}

int paging_get_indexes(void *virtual_addr, uint32_t *directory_index_out,
                       uint32_t *table_index_out) {
  if (!paging_is_aligned(virtual_addr))
    return -EINVARG;

  uint32_t next_block_addr = PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE;
  *directory_index_out = ((uint32_t)virtual_addr / (next_block_addr));
  *table_index_out =
      ((uint32_t)virtual_addr % next_block_addr / PAGING_PAGE_SIZE);

  return OK;
}

int paging_set(uint32_t *directory, void *virtual_addr, uint32_t val) {
  if (!paging_is_aligned(virtual_addr))
    return -EINVARG;

  uint32_t directory_index = 0;
  uint32_t table_index = 0;
  int result = paging_get_indexes(virtual_addr, &directory_index, &table_index);

  if (result)
    return result;

  uint32_t entry = directory[directory_index];
  uint32_t *table = (uint32_t *)(entry & 0xfffff000);
  table[table_index] = val;

  return OK;
}
