; Functions that are exposed from asm to C for convenience

global load_page_directory
global enable_paging
global disable_paging
global get_pdbr
global flush_tlb_entry

load_page_directory:

  mov cr3, eax

ret

; enable_paging:
;
;
;   mov eax, cr0
;   or  eax, 0x80000000
;   mov cr0, eax
;
; ret

; disable_paging:
;
;   mov eax, cr0
;
;   and eax, 0x7FFFFFFF
;
;   mov cr0, eax
; ret

get_pdbr:

  mov eax, cr3

ret

flush_tlb_entry:

  cli
  invlpg [eax]
  sti

ret
