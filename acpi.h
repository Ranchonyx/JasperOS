#ifndef ACPI_H
#define ACPI_H
#include "jaspdef.h"

typedef struct RSDPDescriptor {
 char Signature[8];
 uint8_t Checksum;
 char OEMID[6];
 uint8_t Revision;
 uint32_t RsdtAddress;
} __attribute__ ((packed)) RSDPDescriptor_t;

typedef struct RSDPDescriptor20 {
 RSDPDescriptor_t firstPart;

 uint32_t Length;
 uint64_t XsdtAddress;
 uint8_t ExtendedChecksum;
 uint8_t reserved[3];
} __attribute__ ((packed)) RSDPDescriptor20_t;

typedef struct ACPISDTHeader {
  char Signature[4];
  uint32_t Length;
  uint8_t Revision;
  uint8_t Checksum;
  char OEMID[6];
  char OEMTableID[8];
  uint32_t OEMRevision;
  uint32_t CreatorID;
  uint32_t CreatorRevision;
} __attribute__((packed)) ACPISDTHeader_t;

void acpi_print_descriptor(RSDPDescriptor_t *desc);
uint8_t acpi_find_descriptor();

#endif
