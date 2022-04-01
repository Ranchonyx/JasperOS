#include "vga.h"
#include "multiboot.h"
#include "util.h"
#include "asmfunc.h"
#include "IDT.h"
#include "ISR.h"
#include "filesystem.h"
#include "initrd.h"
#include "sys.h"
#include "pic.h"
#include "timer.h"

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

typedef void (*call_prog_t)(void);


unsigned int initrd_start;
unsigned int initrd_end;
uint32_t program_addr = 0;

//KERNEL ENTRY POINT
void kmain(uint32_t magic, uint32_t mbiaddr, uint32_t kernel_end, uint32_t kernel_start) {

  multiboot_info_t *mbi = (multiboot_info_t*)mbiaddr;

  init_framebuffer(mbi->framebuffer_addr);
  printf("[GOOD] Framebuffer initialized at 0x%x\n", mbi->framebuffer_addr);

  init_timer(60);

  isr_install();
  printf("[GOOD] ISRs installed\n");

  // printf("[INFO] Performing MBI_TABLE data dump\n");
  if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    printf("[BAD] Passed Bootloader Magic 0x%x != 0x%x\n", magic, MULTIBOOT_BOOTLOADER_MAGIC);
    PANIC("Invalid Bootloader Magic\n");
  }

  printf("[INFO] MBI_TABLE_ADDRESS: 0x%x | MAGIC: 0x%x\n", mbiaddr, magic);

  if(CHECK_FLAG(mbi->flags, 0)) {
    printf("LOWMEM: %uKB, HIMEM: %uKB\n", (unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);
    // printf("[INFO] Enabling PMM with %u KB.\n", mem_sz);
    // pmmngr_init(mem_sz, 0x100000+(end-start)*512);
  }

  if(CHECK_FLAG(mbi->flags, 3)) {
    multiboot_module_t *mod;
    unsigned int i;

    for(i = 0, mod = (multiboot_module_t *) mbi->mods_addr; i < mbi->mods_count; i++, mod += sizeof(multiboot_module_t)) {
      // clrprintf("INITRD_START = 0x%x, INITRD_END = 0x%x\n", TEAL_ON_BLACK, (unsigned) mod->mod_start, (unsigned) mod->mod_end);
      if(i == 0) {
        initrd_start = (unsigned)mod->mod_start;
        initrd_end   = (unsigned)mod->mod_end;
      }
    }

  }

  if(CHECK_FLAG(mbi->flags, 4) && CHECK_FLAG(mbi->flags, 5)) {
    printf("[BAD] BITS 4 & 5 SET\n");
    printf("[HALTED]");
  }

  uint32_t mem_sz = 1024 + (mbi->mem_lower + mbi->mem_upper)*64;

  printf("[INFO] Kernel start: 0x%x\n", (kernel_start));
  printf("[INFO] Kernel end: 0x%x\n", (kernel_start+kernel_end));
  printf("[INFO] Kernel size: %u bytes\n", (kernel_end), (kernel_end/1024));
  printf("[INFO] Memory size: %u bytes\n", mem_sz);

  // if (CHECK_FLAG (mbi->flags, 6)) {
  //       multiboot_memory_map_t *mmap;
  //
  //       // printf("mmap_addr = 0x%x, mmap_length = 0x%x\n",(unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);
  //       for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr; (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;) {
  //             mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size));
  //               if((unsigned)mmap->addr != 0x0) {
  //                 // printf("[INFO] Initialising region at 0x%x, length: 0x%x\n", (unsigned) mmap->addr, (unsigned) mmap->len);
  //                 // pmmngr_init_region((unsigned) mmap->addr, (unsigned) mmap->len);
  //               }
  //     }
  // }

  // pmmngr_deinit_region(0x100000, (end)*512);
  // pmmngr_deinit_region(mbi->framebuffer_addr-0x1000, (VIDMEM_SIZE)*2);
  // printf("[INFO] PMM Regions initialised, used/reserved: %u, free: %u\n", pmmngr_get_used_blk_count(), pmmngr_get_free_blk_count());


/*0x100000+(kernel_end-kernel_start)*512*/
  // initialise_kheap(kernel_end+0x1000);
  // initialise_paging(kernel_end+initrd_end, mem_sz);


  uint32_t initrd_end        = initrd_end;
  fs_root                    = initialise_initrd(initrd_start);
  int i = 0;
  struct dirent *node = 0;
  while((node = readdir_fs(fs_root, i)) != 0) {
    clrprintf("Found file: %s\n", TEAL_ON_BLACK, node->name);

    fs_node_t *fsnode = finddir_fs(fs_root, node->name);
    if(strcmp(node->name, "program.mod") == 0) {
      program_addr = initrd_get_file_content_offset(fsnode, 0);
      // printf("Found program.mod at 0x%x\n", program_addr);
    }
    clrprintf("File location: 0x%x\n", TEAL_ON_BLACK, initrd_get_file_content_offset(fsnode, 0));
    if((fsnode->flags & 0x7) == FS_DIR) {
      printf("\n(directory)\n");
    } else {
      printf("\ncontents: ");
      char *buf = (char*) malloc(fsnode->length);
      uint32_t sz = read_fs(fsnode, 0, fsnode->length, (uint8_t*)buf);

      nprintf(buf, sz);

      //no free yet
      printf("\n");
    }
    i++;
  }

  // printf("Calling program.mod\n");
  // call_prog_t program = (call_prog_t) program_addr;
  // program();
  //
  // asm("int $14");



}
