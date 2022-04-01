#include "vga.h"
#include "string.h"
#include "ports.h"
#include "util.h"
#include "sys.h"
#include "multiboot.h"

static char* vidmem = (char*) VIDMEM_ADDR;

static int xpos = 0;
static int ypos = 0;

void init_framebuffer(unsigned int addr) {
  vidmem = (char*) addr;
}

void putchar(const char c, const uint8_t color) {

  set_cursor(get_offset(xpos, ypos));
  if(ypos + 1 == COLS_MAX) {
    scroll_ln();
  }

  if(c == '\t') {
    for(int i = 0; i < 4; i++) {
        *(vidmem+ (xpos + ypos * COLS_MAX) * 2) = ' ' & 0xFF;
        *(vidmem + (xpos + ypos * COLS_MAX) * 2 + 1) = color;
    }
  }

  if( c == '\n' || c == '\r') {
    newline:
      xpos = 0;
      ypos++;
      if(ypos >= ROWS_MAX) {
          scroll_ln();
        }
        return;
      }
        *(vidmem+ (xpos + ypos * COLS_MAX) * 2) = c & 0xFF;
        *(vidmem + (xpos + ypos * COLS_MAX) * 2 + 1) = color;
        xpos++;

  if(xpos >= COLS_MAX) {
    goto newline;
  }
}

void print_binary(uint32_t n) {
  for(int i = sizeof(n) << 3; i; i--) {
    putchar('0'+((n >> (i - 1))&1), 7);
  }
  printf("\n");
}

/*  Format a string and print it on the screen, just like the libc
   function printf. */
void printf (const char *format, ...)
{
  char **arg = (char **) &format;
  int c;
  char buf[64];

  arg++;

  while ((c = *format++) != 0)
    {
      if (c != '%')
        putchar (c, 7);
      else
        {
          char *p, *p2;
          int pad0 = 0, pad = 0;

          c = *format++;
          if (c == '0')
            {
              pad0 = 1;
              c = *format++;
            }

          if (c >= '0' && c <= '9')
            {
              pad = c - '0';
              c = *format++;
            }

          switch (c)
            {
            case 'd':
            case 'u':
            case 'x':
              itoa (buf, c, *((int *) arg++));
              p = buf;
              goto string;
              break;

            case 's':
              p = *arg++;
              if (! p)
                p = "(null)";

            string:
              for (p2 = p; *p2; p2++);
              for (; p2 < p + pad; p2++)
                putchar (pad0 ? '0' : ' ', 7);
              while (*p)
                putchar (*p++, 7);
              break;

            default:
              putchar (*((int *) arg++), 7);
              break;
            }
        }
    }
}

void nprintf(const char* format, int n, ...)
{
  char **arg = (char **) &format;
  int c;
  char buf[64];

  arg++;

  while (n-- > 0)
    {
      c = *format++;
      if (c != '%')
        putchar (c, 7);
      else
        {
          char *p, *p2;
          int pad0 = 0, pad = 0;

          c = *format++;
          if (c == '0')
            {
              pad0 = 1;
              c = *format++;
            }

          if (c >= '0' && c <= '9')
            {
              pad = c - '0';
              c = *format++;
            }

          switch (c)
            {
            case 'd':
            case 'u':
            case 'x':
              itoa (buf, c, *((int *) arg++));
              p = buf;
              goto string;
              break;

            case 's':
              p = *arg++;
              if (! p)
                p = "(null)";

            string:
              for (p2 = p; *p2; p2++);
              for (; p2 < p + pad; p2++)
                putchar (pad0 ? '0' : ' ', 7);
              while (*p)
                putchar (*p++, 7);
              break;

            default:
              putchar (*((int *) arg++), 7);
              break;
            }
        }
    }
}

/*  Format a string and print it on the screen colorfully, just like the libc
   function printf but with color */
void clrprintf(const char *format, const uint8_t color, ...)
{
  char **arg = (char **) &format;
  int c;
  char buf[64];

  arg+=2;

  while ((c = *format++) != 0)
    {
      if (c != '%')
        putchar (c, color);
      else
        {
          char *p, *p2;
          int pad0 = 0, pad = 0;

          c = *format++;
          if (c == '0')
            {
              pad0 = 1;
              c = *format++;
            }

          if (c >= '0' && c <= '9')
            {
              pad = c - '0';
              c = *format++;
            }

          switch (c)
            {
            case 'd':
            case 'u':
            case 'x':
              itoa (buf, c, *((int *) arg++));
              p = buf;
              goto string;
              break;

            case 's':
              p = *arg++;
              if (! p)
                p = "(null)";

            string:
              for (p2 = p; *p2; p2++);
              for (; p2 < p + pad; p2++)
                putchar (pad0 ? '0' : ' ', color);
              while (*p)
                putchar (*p++, color);
              break;

            default:
              putchar (*((int *) arg++), color);
              break;
            }
        }
    }

}

void clrscr()
{
	for(size_t i = 0; i < COLS_MAX * ROWS_MAX; ++i) {
		set_char_at(i * 2, ' ', get_color(VIDMEM_SIZE));
	}
	set_cursor(get_offset(0, 0));
    ypos = 0;
    xpos = 0;
}

void set_cursor(size_t offset)
{
	offset /= 2;
	outb(VGA_CTRL_REG, VGA_OFFSET_HIGH);
	outb(VGA_DATA_REG, (unsigned char) (offset >> 8));
	outb(VGA_CTRL_REG, VGA_OFFSET_LOW);
	outb(VGA_DATA_REG, (unsigned char) (offset & 0xFF));
}

size_t get_cursor()
{
	outb(VGA_CTRL_REG, VGA_OFFSET_HIGH);
	size_t offset = inb(VGA_DATA_REG) << 8;
	outb(VGA_CTRL_REG, VGA_OFFSET_LOW);
	offset += inb(VGA_DATA_REG);
	return offset * 2;
}

size_t get_color(size_t offset) {
	return ((unsigned char*) vidmem)[offset];
}

void set_char_at(size_t offset, const char chr, const char color)
{
	vidmem[offset] = chr;
	vidmem[offset + 1] = color;
}

void set_char_xy(size_t x, size_t y, const char chr, const char color)
{
	vidmem[(x*2) + (COLS_MAX * y * 2)] = chr;
	vidmem[(x*2) + (COLS_MAX * y * 2) + 1] = color;
}

void print_backspace()
{
	set_cursor(get_cursor() - 2);
	set_char_at(get_cursor(), ' ', get_color(get_cursor() + 1));
}

size_t row_from_offset(size_t offset)
{
	return offset / (2 * COLS_MAX);
}

size_t get_offset(size_t col, size_t row)
{
	return 2 * (row * COLS_MAX + col);
}

size_t move_offset_to_newline(size_t offset)
{
    ypos++;
    xpos = 0;
	return get_offset(0, row_from_offset(offset) + 1);
}

void scroll_ln()
{
	memcpy(
		(uint8_t *) (get_offset(0, 0) + vidmem),
		(uint8_t *) (get_offset(0, 1) + vidmem),
		(COLS_MAX * (ROWS_MAX - 1) * 2)
	);

	for(size_t col = 0; col < COLS_MAX; col++) {
		set_char_at(get_offset(col, ROWS_MAX - 1), ' ', 7);
	}
    ypos--;
}
