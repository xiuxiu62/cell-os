#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idt_desc {
  uint16_t offset_1; // Offset bits 0-15
  uint16_t selector; // Selector in our GDT
  uint8_t zero;      // Unused
  uint8_t type_attr; // Descriptor type + attrs
  uint16_t offset_2; // Offset bits 16-31
} __attribute__((packed));

struct idtr_desc {
  uint16_t limit; // Size of descriptor table -1
  uint32_t base;  // Base address of the table
} __attribute__((packed));

void idt_init();

#endif
