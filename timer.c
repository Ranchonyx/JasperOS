#include "timer.h"
#include "ISR.h"
#include "jaspdef.h"
#include "ports.h"
#include "vga.h"

static uint32_t ticks = 0;

void delay(uint32_t millis)
{
  uint32_t t = timer_getticks()+(millis / 10);

  while(timer_getticks()<t);
}

void timer_callback(cpu_state_t *state)
{
  ticks++;
}

void init_timer(uint32_t hz)
{
  register_interrupt_handler(IRQ0, &timer_callback);

  uint32_t divisor = 1193182 / hz;
  uint8_t h = (uint8_t)(divisor & 0xFF);
  uint8_t l = (uint8_t)((divisor >> 8) & 0xFF);

  outb(0x43, 0x36);
  outb(0x40, l);
  outb(0x40, h);

}

uint32_t timer_getticks()
{
  return ticks;
}
