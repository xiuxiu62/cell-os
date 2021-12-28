#include "disk.h"
#include "../config.h"
#include "../io/io.h"
#include "../mem/memory.h"
#include "../status.h"

struct disk disk;

void disk_search_init() {
  memset(&disk, 0, sizeof(disk));
  disk.type = CELLOS_DISK_TYPE_REAL;
  disk.sector_size = CELLOS_SECTOR_SIZE;
};

struct disk *disk_get(int index) {
  // Temporary static return for non-main disk
  if (index != CELLOS_DISK_TYPE_REAL)
    return 0;

  return &disk;
}

int disk_read_sector(int lba, int total, void *buf) {
  // Prepare disk read
  outb(0x1f6, (lba >> 24) | 0xe0);
  outb(0x1f2, total);
  outb(0x1f3, (unsigned char)(lba & 0xff));
  outb(0x1f4, (unsigned char)(lba >> 8));
  outb(0x1f5, (unsigned char)(lba >> 16));
  outb(0x1f7, 0x20);

  unsigned short *ptr = (unsigned short *)buf;
  for (int b = 0; b < total; b++) {
    // Wait for buffer to be ready
    char c = insb(0x1f7);
    while (!(c & 0x08))
      c = insb(0x1f7);

    for (int i = 0; i < 256; i++) {
      *ptr = insw(0x1f0);
      ptr++;
    }
  }

  return OK;
}

int disk_read_block(struct disk *idisk, unsigned int lba, int total,
                    void *buf) {
  if (idisk != CELLOS_DISK_TYPE_REAL)
    return -EIO;

  return disk_read_sector(lba, total, buf);
}
