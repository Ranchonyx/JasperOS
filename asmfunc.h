#ifndef ASMFUNC_H
#define ASMFUNC_H
#include "jaspdef.h"

extern void load_page_directory(uint32_t*);
extern void flush_tlb_entry(uint32_t*);
extern uint32_t get_pdbr();
#endif
