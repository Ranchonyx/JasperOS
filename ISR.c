#include "ISR.h"
#include "IDT.h"
#include "vga.h"
#include "sys.h"
#include "pic.h"
#include "ports.h"

static const char *irq_messages[] = {
  "PIT Timer",
  "Keyboard",
  "PIT Cascade",
  "COM2",
  "COM1",
  "LPT2",
  "Floppy Disk",
  "LPT1 / Spurious",
  "CMOS RTC",
  "Peripheral / Free",
  "Peripheral / Free",
  "Peripheral / Free",
  "PS/2 Mouse",
  "FPU / Coprocessor / Inter-processor",
  "Primary ATA Disk",
  "Secondary ATA Disk"
};

static const char *exception_messages[] = {
  "Division By Zero",
  "Debug",
  "Non Maskable Interrupt",
  "Breakpoint",
  "Overflow",
  "Bound Range Exceeded",
  "Invalid Opcode",
  "Device Not Available",
  "Double Fault",
  "Coprocessor Segment Overrun",
  "Invalid TSS",
  "Segment Not Present",
  "Stack-Segment Fault",
  "General Protection Fault",
  "Page Fault",
  "x87 Floating-Point Exception",
  "Alignment Check",
  "Machine Check",
  "SIMD Floating-Point Exception",
  "Virtualization Exception",
  "Control Protection Exception",
  "Reserved / Unknown Exception",
  "Reserved / Unknown Exception",
  "Reserved / Unknown Exception",
  "Reserved / Unknown Exception",
  "Reserved / Unknown Exception",
  "Hypervisor Injection Exception",
  "VMM Communication Exception",
  "Security Exception",
  "Reserved / Unknown Exception",
  "Reserved / Unknown Exception"
};

isr_t interrupt_handlers[256];

void isr_install()
{
  idt_set_descriptor( 0, (uint32_t)isr0 , 0x8E);
  idt_set_descriptor( 1, (uint32_t)isr1 , 0x8E);
  idt_set_descriptor( 2, (uint32_t)isr2 , 0x8E);
  idt_set_descriptor( 3, (uint32_t)isr3 , 0x8E);
  idt_set_descriptor( 4, (uint32_t)isr4 , 0x8E);
  idt_set_descriptor( 5, (uint32_t)isr5 , 0x8E);
  idt_set_descriptor( 6, (uint32_t)isr6 , 0x8E);
  idt_set_descriptor( 7, (uint32_t)isr7 , 0x8E);
  idt_set_descriptor( 8, (uint32_t)isr8 , 0x8E);
  idt_set_descriptor( 9, (uint32_t)isr9 , 0x8E);
  idt_set_descriptor(10, (uint32_t)isr10, 0x8E);
  idt_set_descriptor(11, (uint32_t)isr11, 0x8E);
  idt_set_descriptor(12, (uint32_t)isr12, 0x8E);
  idt_set_descriptor(13, (uint32_t)isr13, 0x8E);
  idt_set_descriptor(14, (uint32_t)isr14, 0x8E);
  idt_set_descriptor(15, (uint32_t)isr15, 0x8E);
  idt_set_descriptor(16, (uint32_t)isr16, 0x8E);
  idt_set_descriptor(17, (uint32_t)isr17, 0x8E);
  idt_set_descriptor(18, (uint32_t)isr18, 0x8E);
  idt_set_descriptor(19, (uint32_t)isr19, 0x8E);
  idt_set_descriptor(20, (uint32_t)isr20, 0x8E);
  idt_set_descriptor(21, (uint32_t)isr21, 0x8E);
  idt_set_descriptor(22, (uint32_t)isr22, 0x8E);
  idt_set_descriptor(23, (uint32_t)isr23, 0x8E);
  idt_set_descriptor(24, (uint32_t)isr24, 0x8E);
  idt_set_descriptor(25, (uint32_t)isr25, 0x8E);
  idt_set_descriptor(26, (uint32_t)isr26, 0x8E);
  idt_set_descriptor(27, (uint32_t)isr27, 0x8E);
  idt_set_descriptor(28, (uint32_t)isr28, 0x8E);
  idt_set_descriptor(29, (uint32_t)isr29, 0x8E);
  idt_set_descriptor(30, (uint32_t)isr30, 0x8E);
  idt_set_descriptor(31, (uint32_t)isr31, 0x8E);

  //pic_remap(0x20, 0x28);
  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  outb(0x21, 0x20);
  outb(0xA1, 0x28);
  outb(0x21, 0x04);
  outb(0xA1, 0x02);
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
  outb(0x21, 0x00);
  outb(0xA1, 0x00);

  idt_set_descriptor(32, (uint32_t)irq0, 0x8E);
  idt_set_descriptor(33, (uint32_t)irq1, 0x8E);
  idt_set_descriptor(34, (uint32_t)irq2, 0x8E);
  idt_set_descriptor(35, (uint32_t)irq3, 0x8E);
  idt_set_descriptor(36, (uint32_t)irq4, 0x8E);
  idt_set_descriptor(37, (uint32_t)irq5, 0x8E);
  idt_set_descriptor(38, (uint32_t)irq6, 0x8E);
  idt_set_descriptor(39, (uint32_t)irq7, 0x8E);
  idt_set_descriptor(40, (uint32_t)irq8, 0x8E);
  idt_set_descriptor(41, (uint32_t)irq9, 0x8E);
  idt_set_descriptor(42, (uint32_t)irq10, 0x8E);
  idt_set_descriptor(43, (uint32_t)irq11, 0x8E);
  idt_set_descriptor(44, (uint32_t)irq12, 0x8E);
  idt_set_descriptor(45, (uint32_t)irq13, 0x8E);
  idt_set_descriptor(46, (uint32_t)irq14, 0x8E);
  idt_set_descriptor(47, (uint32_t)irq15, 0x8E);

  load_idt();
}

void register_interrupt_handler(uint8_t vector, isr_t handler)
{
  interrupt_handlers[vector] = handler;
  if(vector >= IRQ0) {
    printf("[IRQ] Registered handler for interrupt request %u (%s)\n", vector-32, irq_messages[vector-32]);
  } else {
    printf("[ISR] Registered handler for interrupt %u (%s)\n", vector, exception_messages[vector]);
  }
}

void isr_handler(cpu_state_t *state)
{

  if(interrupt_handlers[state->int_no] != 0) {
    isr_t handler = interrupt_handlers[state->int_no];
    handler(state);
  }

  clrprintf("Exception handler fired.\nSystem Halted.\nINTNO = %u, ERR = 0x%x\n", LRED_ON_BLACK, state->int_no, state->err_code);
  clrprintf("[%s]\n",LRED_ON_BLACK, exception_messages[state->int_no]);
  HLT();
}

void irq_handler(cpu_state_t *state)
{

  if(interrupt_handlers[state->int_no] != 0) {
    isr_t handler = interrupt_handlers[state->int_no];
    handler(state);
  }

 pic_eoi(state->int_no);

}
