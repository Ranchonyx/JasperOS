//#include "timer.h"
#include "sys.h"
#include "util.h"
#include "strings.h"
//#include "g_vga.h"
#include "ports.h"
//#include "dmm.h"
#include "util.h"
#include "vga.h"
//#include "colors.h"
//#include "math.h"
//#include "IDT.h"
//#include "mouse.h"
//#include "serial.h"
//#include "timer.h"
//#include "keyboard.h"
#include "string.h"
//#include "speaker.h"

uint32_t read_cr0() {
  uint32_t val;
  asm volatile ( "mov %%cr0, %0" : "=r"(val) );
  return val;
}

uint32_t read_cr3() {
  uint32_t val;
  asm volatile ( "mov %%cr3, %0" : "=r"(val) );
  return val;
}

//Special thanks to Fast-Nop for the char extraction help and patience with me
char *register_to_string(uint32_t reg)
{

	char regtext[4];
	char* regtext_ptr = regtext;
	char c1 = (char) (reg >> 24);
	char c2 = (char) ((reg >> 16) & 0xFF);
	char c3 = (char) ((reg >> 8) & 0xFF);
	char c4 = (char) reg;

	regtext[0] = c1;
	regtext[1] = c2;
	regtext[2] = c3;
	regtext[3] = c4;
	regtext[4] = '\0';


	reverse(regtext);
	return regtext_ptr;
}

void* memcpy(void* dest, const void* src, size_t n)
{
  char* cdest = (char*)dest;
  const char* csrc = (const char*)src;
  for (size_t i = 0; i < n; i++) cdest[i] = csrc[i];
  return dest;
}

void* memmove (void* dest, const void* src, size_t n)
{
  if (src == dest) return dest;

  char* cdest = (char*)dest;
  const char* csrc = (const char*)src;

  if (cdest > csrc) while (n--) cdest[n] = csrc[n];
  else for (size_t i = 0; i < n; i++) cdest[i] = csrc[i];

  return dest;
}

void *memset(void* dest, const unsigned char v, size_t n)
{
  unsigned char* cdest = (unsigned char*) dest;
  for(size_t i = 0; i < n; i++) cdest[i] = v;
  return dest;
}

void *memcpy32(uint32_t* dest, const uint32_t* src, uint32_t n)
{
  uint32_t* cdest = (uint32_t*)dest;
  const uint32_t* csrc = (const uint32_t*)src;
  for (uint32_t i = 0; i < n; i++) cdest[i] = csrc[i];
  return dest;
}

void *memmove32(uint32_t* dest, const uint32_t* src, uint32_t n)
{
  if (src == dest) return dest;

  uint32_t* cdest = (uint32_t*)dest;
  const uint32_t* csrc = (const uint32_t*)src;

  if (cdest > csrc) while (n--) cdest[n] = csrc[n];
  else for (uint32_t i = 0; i < n; i++) cdest[i] = csrc[i];

  return dest;
}

void *memset32(uint32_t* dest, const uint32_t v, uint32_t n)
{
  uint32_t* cdest = (uint32_t*) dest;
  for(uint32_t i = 0; i < n; i++) cdest[i] = v;
  return dest;
}
