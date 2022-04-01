#include "pde.h"
#include "jaspdef.h"

void pd_entry_add_attrib(pd_entry *entry, uint32_t attrib) {
  *entry |= attrib;
}

void pd_entry_del_attrib(pd_entry *entry, uint32_t attrib) {
  *entry &= ~attrib;
}

void pd_entry_set_frame(pd_entry *entry, physical_addr addr) {
  *entry = (*entry & ~I86_PDE_PRESENT) | addr;
}

int pd_entry_is_present(pd_entry entry) {
  return entry & I86_PDE_PRESENT;
}

int pd_entry_is_writable(pd_entry entry) {
  return entry & I86_PDE_WRITABLE;
}

physical_addr pd_entry_pfn(pd_entry entry) {
  return entry & I86_PDE_FRAME;
}

int pd_entry_is_user(pd_entry entry) {
  return entry & I86_PDE_USER;
}

int pd_entry_is_4mb(pd_entry entry) {
  return entry & I86_PDE_4MB;
}
