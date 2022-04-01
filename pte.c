#include "pte.h"
#include "jaspdef.h"

void pt_entry_add_attrib(pt_entry *entry, uint32_t attrib) {
  *entry |= attrib;
}

void pt_entry_del_attrib(pt_entry *entry, uint32_t attrib) {
  *entry &= ~attrib;
}

void pt_entry_set_frame(pt_entry *entry, physical_addr addr) {
  *entry = (*entry & ~I86_PTE_PRESENT) | addr;
}

int pt_entry_is_present(pt_entry entry) {
  return entry & I86_PTE_PRESENT;
}

int pt_entry_is_writable(pt_entry entry) {
  return entry & I86_PTE_WRITABLE;
}

physical_addr pt_entry_pfn(pt_entry entry) {
  return entry & I86_PTE_FRAME;
}
