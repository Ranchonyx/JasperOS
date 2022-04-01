#ifndef SYS_H
#define SYS_H
#include "jaspdef.h"
#include "stddef.h"
#include "string.h"
#include "vga.h"

#define ASMROUTINE extern void
#define PANIC( msg ) printf("%s occured at %s:%u\n", msg, __FILE__, __LINE__); HLT();
#define HLT( void )        __asm__ volatile("cli; hlt");

uint32_t read_cr0(void);
uint32_t read_cr3(void);

//void native_cpuid(uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t * edx);
//void hang(char *cause, registers_t *regs);
void *memcpy(void* dest, const void* src, size_t n);
void *memmove(void* dest, const void* src, size_t n);
void *memset(void* dest, const unsigned char v, size_t n);
void *memcpy32(uint32_t* dest, const uint32_t* src, uint32_t n);
void *memmove32(uint32_t* dest, const uint32_t* src, uint32_t n);
void *memset32(uint32_t* dest, const uint32_t v, uint32_t n);
char *register_to_string(uint32_t reg);
//void get_cpu_vendor_string(char dest[]);
//void get_cpu_brand_string(char dest[]);
//bool extended_cpuid_available();
//char **argparse(char *cmd);
//int  cmd(char* command);
//void delay(uint32_t millis);

#endif /* end of include guard: SYS_H */
