#include "jaspdef.h"

#define PIC1      0x20
#define PIC1_CMD  PIC1
#define PIC1_DAT  (PIC1+1)

#define PIC2      0xA0
#define PIC2_CMD  PIC2
#define PIC2_DAT  (PIC2+1)

#define PIC_EOI   0x20

#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define PIC_READ_IRR                0x0a    /* OCW3 irq ready next CMD read */
#define PIC_READ_ISR                0x0b    /* OCW3 irq service next CMD read */

void pic_eoi          (unsigned char irq);
void pic_remap        (int offset1, int offset2);
void irq_set_mask     (unsigned char irqline);
void irq_clear_mask   (unsigned char irqline);

uint16_t __pic_get_irq_reg  (int ocw3);
uint16_t pic_get_irr        ();
uint16_t pic_get_isr        ();
