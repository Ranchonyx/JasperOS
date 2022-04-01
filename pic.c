#include "pic.h"
#include "ports.h"
#include "jaspdef.h"

/*
PIC1      0x20
PIC1_CMD  PIC1
PIC1_DAT  (PIC1+1)
PIC2      0xA0
PIC2_CMD  PIC2
PIC2_DAT  (PIC2+1)
PIC_EOI   0x20
ICW1_ICW4	0x01		ICW4 (not) needed
ICW1_SINGLE	0x02		Single (cascade) mode
ICW1_INTERVAL4	0x04	Call address interval 4 (8)
ICW1_LEVEL      0x08	Level triggered (edge) mode
ICW1_INIT       0x10	Initialization - required!
ICW4_8086       0x01	8086/88 (MCS-80/85) mode
ICW4_AUTO       0x02	Auto (normal) EOI
ICW4_BUF_SLAVE	0x08	Buffered mode/slave
ICW4_BUF_MASTER	0x0C	Buffered mode/master
ICW4_SFNM	    0x10	Special fully nested (not)
PIC_READ_IRR    0x0a    OCW3 irq ready next CMD read
PIC_READ_ISR    0x0b    OCW3 irq service next CMD read
*/

void pic_eoi (unsigned char irq)
{
  if(irq >= 8) {
    outb(PIC2_CMD, PIC_EOI);
  }
  outb(PIC1_CMD, PIC_EOI);
}

void pic_remap (int offset1, int offset2)
{
  unsigned char a1, a2;

  a1 = inb(PIC1_DAT);
  a2 = inb(PIC2_DAT);

  outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
  io_wait();
  outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
  io_wait();
  outb(PIC1_DAT, offset1);                 // ICW2: Master PIC vector offset
  io_wait();
  outb(PIC2_DAT, offset2);                 // ICW2: Slave PIC vector offset
  io_wait();
  outb(PIC1_DAT, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
  io_wait();
  outb(PIC2_DAT, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
  io_wait();

  outb(PIC1_DAT, ICW4_8086);
  io_wait();
  outb(PIC2_DAT, ICW4_8086);
  io_wait();

  outb(PIC1_DAT, a1);   // restore saved masks.
  outb(PIC2_DAT, a2);
}

void irq_set_mask(unsigned char irqline)
{
  uint16_t port;
  uint8_t  value;

  if(irqline < 8) {
    port = PIC1_DAT;
  } else {
    port = PIC2_DAT;
    irqline -= 8;
  }

  value = inb(port) | (1 << irqline);
  outb(port, value);
}

void irq_clear_mask(unsigned char irqline)
{
  uint16_t port;
  uint8_t  value;

  if(irqline < 8) {
    port = PIC1_DAT;
  } else {
    port = PIC2_DAT;
    irqline -= 8;
  }

  value = inb(port) & ~(1 << irqline);
  outb(port, value);
}

uint16_t __pic_get_irq_reg (int ocw3)
{
  outb(PIC1_CMD, ocw3);
  outb(PIC2_CMD, ocw3);

  return (inb(PIC2_CMD) << 8) | inb(PIC1_CMD);
}

uint16_t pic_get_irr ()
{
  return __pic_get_irq_reg(PIC_READ_IRR);
}

uint16_t pic_get_isr ()
{
  return __pic_get_irq_reg(PIC_READ_ISR);
}

