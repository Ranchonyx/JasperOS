int mod_main() {
  char* vidmem = (char*) 0x000b8000;
  for(int i = 0; i < 80; i += 2) {
    vidmem[i] = '#';
    vidmem[i+1] = 0x0a;
  }
}
