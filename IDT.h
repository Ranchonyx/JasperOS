#ifndef IDT_H
#define IDT_H
#include "jaspdef.h"
#define IDT_MAX_ENTRIES 256

typedef struct {               // 32-Bit IDT Entry structure
  uint16_t isr_lo;             // Lower 16 Bits of the ISR address
  uint16_t kernel_cs;          // GDT Segment selector that the CPU will load into CS before calling the ISR
  uint8_t  reserverd;          // Must be zero iirc
  uint8_t  attributes;         // Type + Attributes afaik
  uint16_t isr_hi;             // Higher 16 Bits of the ISR address
} __attribute__((packed)) idt_entry_32_t;

typedef struct {               // 32-Bit IDT Register structure
  uint16_t limit;              // Size of the IDTR
  uint32_t base;               // Base Address of the IDTR
} __attribute__((packed)) idtr_32_t;

extern void* isr_stub_table[]; // The external isr_stub_table as defined in assembly code

void idt_set_descriptor(uint8_t vector, uint32_t isr, uint8_t flags);
void load_idt();

#endif
