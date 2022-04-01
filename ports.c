#include "ports.h"

inline unsigned char inb(uint16_t port)
{
	unsigned char result;
	__asm__("in %%dx, %%al" : "=a"(result) : "d" (port));
	return result;
}

inline void outb(uint16_t port, uint8_t data)
{
	__asm__("out %%al, %%dx" : : "a"(data), "d" (port));
}

inline unsigned short inw(uint16_t port)
{
	unsigned short result;
	__asm__("in %%dx, %%al" : "=a"(result) : "d" (port));
	return result;
}

inline void outw(uint16_t port, uint16_t data)
{
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

inline void io_wait()
{
  outb(0x80, 0);
}
