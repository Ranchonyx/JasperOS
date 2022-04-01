#ifndef PAGING_H
#define PAGING_H
#include "jaspdef.h"
#include "ISR.h"

typedef struct page
{
  uint32_t present    : 1;   // Page present in memory
  uint32_t rw         : 1;   // Read-only if clear, readwrite if set
  uint32_t user       : 1;   // Supervisor level only if clear
  uint32_t accessed   : 1;   // Has the page been accessed since last refresh?
  uint32_t dirty      : 1;   // Has the page been written to since last refresh?
  uint32_t unused     : 7;   // Amalgamation of unused and reserved bits
  uint32_t frame      : 20;  // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table {
  page_t pages[1024];
} page_table_t;

typedef struct page_directory {
  page_table_t *tables[1024];

  uint32_t tables_physical[1024];

  uint32_t physical_addr;
} page_directory_t;

void initialise_paging(uint32_t addr, uint32_t mem_end_page);

void switch_page_directory(page_directory_t *new);

page_t *get_page(uint32_t address, int make, page_directory_t *dir);

void page_fault(cpu_state_t *regs);

void identity_map_page(uint32_t from, uint32_t to, int make, int writable, int kernel, page_directory_t *directory);

void map_page(void *phys, void *virt, uint32_t flags);

void enable_paging();

#endif