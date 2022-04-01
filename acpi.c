#include "acpi.h"
#include "vga.h"
#include "jaspdef.h"
#include "string.h"

uint8_t acpi_find_descriptor()
{

  uint8_t *p = (uint8_t*)0x000E0000;
  uint8_t *end = (uint8_t*)0x000FFFFF;
  while(p < end)
  {
    if(rstrncmp((char*)p, "RSD PTR ", 8) == 0) {
      return (uint32_t)p;
    }
    p += 16;
  }
  return 0;
}

void acpi_print_descriptor(RSDPDescriptor_t *desc)
{
  printf("Dumping Root System Descriptor Table at 0x%x\n", desc);
  printf("Checksum      = %u\n", desc->Checksum);
  printf("OEMID         = %s\n", desc->OEMID);
  printf("Revision      = %u\n", desc->Revision);
  printf("RSDT_Address  = 0x%x\n", desc->RsdtAddress);
}
