#include "idt.h"
#include "../config.h"
#include "../std/io.h"
#include "../std/mem/memory.h"
#include "../std/print.h"

struct idt_desc idt_descriptors[CELLOS_TOTAL_INTERUPTS];
struct idtr_desc idtr_descriptor;

extern void idt_load(struct idtr_desc *ptr);

extern void enable_interrupts();
extern void disable_interrupts();

extern void no_interrupt();
extern void int21h();

void int0h() { println("Divided by zero"); }

void no_interrupt_handler() { outb(0x20, 0x20); }

void int21h_handler() {
  println("Keyboard pressed");
  outb(0x20, 0x20);
}

void idt_set(int interrupt_no, void *address) {
  struct idt_desc *desc = &idt_descriptors[interrupt_no];
  desc->offset_1 = (uint32_t)address & 0x0000ffff;
  desc->selector = KERNEL_CODE_SELECTOR;
  desc->zero = 0x00;
  desc->type_attr = 0xee;
  desc->offset_2 = (uint32_t)address >> 16;
}

void idt_init() {
  memset(idt_descriptors, 0, sizeof(idt_descriptors));
  idtr_descriptor.limit = sizeof(idt_descriptors) - 1;
  idtr_descriptor.base = (uint32_t)idt_descriptors;

  for (int i = 0; i < CELLOS_TOTAL_INTERUPTS; i++)
    idt_set(i, no_interrupt);

  idt_set(0x0, int0h);
  idt_set(0x21, int21h);

  idt_load(&idtr_descriptor);
  enable_interrupts();
}
