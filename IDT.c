#include "IDT.h"
#include "pic.h"
#define high16(address) (uint16_t)(((address) >> 16) & 0xFFFF)
#define low16(address) (uint16_t)((address) & 0xFFFF)
//The IDTR
static idtr_32_t idtr;

//Array of 32-Bit IDT Entries, aligned for performance
__attribute__((aligned(0x10)))
static idt_entry_32_t idt[256];

void idt_set_descriptor(uint8_t vector, uint32_t isr, uint8_t flags) {
  idt_entry_32_t *descriptor = &idt[vector];

  descriptor->isr_lo      = low16(isr);
  descriptor->kernel_cs   = 0x08;
  descriptor->attributes  = flags;
  descriptor->isr_hi      = high16(isr);
  descriptor->reserverd   = 0;
}

void load_idt() {
  idtr.base  = (uint32_t) &idt;
  idtr.limit = (uint16_t)  IDT_MAX_ENTRIES * sizeof(idt_entry_32_t) - 1;
  __asm__ volatile("lidt (%0)" : : "r" (&idtr)); //Load IDT
  __asm__ volatile ("sti"); //Enable interrupts
}
