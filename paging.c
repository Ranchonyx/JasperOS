#include "paging.h"
#include "asmfunc.h"
#include "jaspdef.h"
#include "ISR.h"
#include "sys.h"
#include "vga.h"
#include "kheap.h"

uint32_t *frames;
uint32_t nframes;

page_directory_t *kernel_directory = 0;
page_directory_t *current_dir = 0;

// Macros used in the bitset algorithms.
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

// Static function to set a bit in the frames bitset
static void set_frame(uint32_t frame_addr)
{
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   frames[idx] |= (0x1 << off);
}

// Static function to clear a bit in the frames bitset
static void clear_frame(uint32_t frame_addr)
{
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   frames[idx] &= ~(0x1 << off);
}

// Static function to test if a bit is set.
static uint32_t test_frame(uint32_t frame_addr)
{
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   return (frames[idx] & (0x1 << off));
}

// Static function to find the first free frame.
static uint32_t first_frame()
{
   uint32_t i, j;
   for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
   {
       if (frames[i] != 0xFFFFFFFF) // nothing free, exit early.
       {
           // at least one bit is free here.
           for (j = 0; j < 32; j++)
           {
               uint32_t toTest = 0x1 << j;
               if ( !(frames[i]&toTest) )
               {
                   return i*4*8+j;
               }
           }
       }
   }
}



//Actual paging code and shit

void alloc_frame(page_t *page, int is_kernel, int is_writeable) {
  if(page->frame != 0) {
    return;
  } else {
    uint32_t index = first_frame();
    if(index == (uint32_t)-1) {
      printf("No free frames lmao\n");
    }
    set_frame(index*0x1000);
    page->present = 1;
    page->rw = (is_writeable) ? 1 : 0;
    page->user = (is_kernel) ? 0 : 1;
    page->frame = index;
  }
}

void free_frame(page_t *page) {
  uint32_t frame;
  if(!( frame = page->frame)) {
    return;
  } else {
    clear_frame(frame);
    page->frame = 0x0;
  }
}

page_t *get_page(uint32_t address, int make, page_directory_t *dir) {
  address /= 0x1000;

  uint32_t table_index = address / 1024;

  if(dir->tables[table_index]) {
    return &dir->tables[table_index]->pages[address % 1024];
  } else if(make) {
    uint32_t tmp;

    dir->tables[table_index] = (page_table_t*) kmalloc_ap(sizeof(page_table_t), &tmp);
    memset(dir->tables[table_index], 0, 0x1000);
    dir->tables_physical[table_index] = tmp | 0x7;
    return &dir->tables[table_index]->pages[address%1024];
  } else {
    return 0;
  }
}

void identity_map_page(uint32_t from, uint32_t to, int make, int writable, int kernel, page_directory_t *directory) {
  printf("[PAGING] Identity mapping region from 0x%x to 0x%x\n", from, to);
  for(; from < to; from += 0x1000) {
    alloc_frame(get_page(from, make, directory), kernel, writable);
  }
}

static inline void invlpg(void* m)
{
    /* Clobber memory to avoid optimizer re-ordering access before invlpg, which may cause nasty bugs. */
    asm volatile ( "invlpg (%0)" : : "b"(m) : "memory" );
}

void map_page(void *phys, void *virt, uint32_t flags) {
  uint32_t d_index = (uint32_t) virt >> 22;
  uint32_t t_index = (uint32_t) virt >> 12 & 0x03FF;

  uint32_t *dirent = (uint32_t*)0xFFFFF000;
  uint32_t *tabent = ((uint32_t*)0xFFC00000) + (0x400 * d_index);

  tabent[t_index] = ((uint32_t) phys) | (flags & 0xFFF) | 0x01;

  invlpg(tabent);
}

void initialise_paging(uint32_t addr, uint32_t mem_end_page) {

  nframes = mem_end_page / 0x1000;
  frames = (uint32_t*) kmalloc(INDEX_FROM_BIT(nframes));
  memset(frames, 0, INDEX_FROM_BIT(nframes));

  kernel_directory = (page_directory_t*) kmalloc_a(sizeof(page_directory_t));
  memset(kernel_directory, 0, sizeof(page_directory_t));
  current_dir = kernel_directory;

  // identity_map_page(0, addr+, 1, 0, 0, kernel_directory);
  identity_map_page(0, addr, 1, 0, 0, kernel_directory);

  register_interrupt_handler(14, &page_fault);

  switch_page_directory(kernel_directory);

  enable_paging();
}

void switch_page_directory(page_directory_t *dir) {
  current_dir = dir;

  asm volatile("mov %0, %%cr3":: "r"(&dir->tables_physical));
}

void enable_paging() {
  uint32_t cr0;
  asm volatile("mov %%cr0, %0": "=r"(cr0));
  cr0 |= 0x80000000; // Enable paging!
  asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void page_fault(cpu_state_t *regs) {
  uint32_t faulting_address;
  int present   = !(regs->err_code & 0x1); // Page not present
  int rw = regs->err_code & 0x2;           // Write operation?
  int us = regs->err_code & 0x4;           // Processor was in user-mode?
  int reserved = regs->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
  int id = regs->err_code & 0x10;          // Caused by an instruction fetch?
  asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

  printf("Page Fault at 0x%x:0x%x refrenced memory at 0x%x\n", regs->cs, regs->eip, faulting_address);
  if(present)   printf("Page present\n");
  if(rw)        printf("Readonly\n");
  if(us)        printf("Usermode\n");
  if(reserved)  printf("Reserved\n");
  if(id)        printf("Causes by instruction fetch\n");
  HLT();
}
