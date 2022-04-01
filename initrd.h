#ifndef INITRD_H
#define INITRD_H

#include "filesystem.h"
#include "jaspdef.h"

typedef struct
{
  uint32_t nFiles;
} initrd_header_t;

typedef struct
{
  uint8_t   magic;
  uint8_t   name[64];
  uint32_t  offset;
  uint32_t  length;
} initrd_file_header_t;

uint32_t initrd_get_file_content_offset(fs_node_t *node, uint32_t offset);
fs_node_t *initialise_initrd(uint32_t location);
#endif /* end of include guard: INITRD_H */
