#include "util.h"
#include "string.h"
#include "stddef.h"
#include "jaspdef.h"
#include "vga.h"
//#include "timer.h"

void reverse(char s[])
{
	int g, numb;
	int tmpry = 0;

	for(numb=0; s[numb] != 0; numb++);

	for(g = 0; g <numb/2; g++)
	{
			tmpry = s[g];
			s[g]=s[numb - 1 - g];
			s[numb - 1 - g] = tmpry;
	}
}

void itoa (char *buf, int base, int d)
{
  char *p = buf;
  char *p1, *p2;
  unsigned long ud = d;
  int divisor = 10;

  /*  If %d is specified and D is minus, put ‘-’ in the head. */
  if (base == 'd' && d < 0)
    {
      *p++ = '-';
      buf++;
      ud = -d;
    }
  else if (base == 'x')
    divisor = 16;

  /*  Divide UD by DIVISOR until UD == 0. */
  do
    {
      int remainder = ud % divisor;

      *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
  while (ud /= divisor);

  /*  Terminate BUF. */
  *p = 0;

  /*  Reverse BUF. */
  p1 = buf;
  p2 = p - 1;
  while (p1 < p2)
    {
      char tmp = *p1;
      *p1 = *p2;
      *p2 = tmp;
      p1++;
      p2--;
    }
}

void map_field_to_matrix(unsigned char src[], size_t rows, size_t cols, unsigned char dest[rows][cols])
{
	size_t k = 0;
	for(size_t i = 0; i < rows; i++) {
		for(size_t j = 0; j < cols; j++) {
			//dest[i][j] = src[(j*rows) + i];
			dest[i][j] = src[k];
			k++;
		}
	}
}

void map_matrix_to_field(unsigned char dest[], size_t rows, size_t cols, unsigned char src[rows][cols])
{
	for(size_t i = 0; i < rows; i++) {
		for(size_t j = 0; i < cols; j++) {
			//DEBUG
			dest[i * cols + j] = src[i][j];
		}
	}
}

// Literally Fast-Nop's C incarnation
// void delaySeconds(int s)
// {
// 	long time = (100000000 * s) / 2;
// 	for(volatile int i = 0; i < time; i++) {
// 		__asm volatile ("nop" : : : "memory");
// 	}
// }

int abs(int x)
{
	int t = (unsigned int) x >> 31;
	return (- t) ^ (x - t);
}

double pow(double base, double exponent)
{
	double r = 1;

	for(;exponent > 0; exponent--) {
		r *= base;
	}

	return r;
}

	double sqrt(double x)
	{
		double r = x;

		__asm__ volatile("fsqrt" : "+t"(r));

		return r;
	}

double sin(double x)
{
	double r = x;

	__asm__ volatile("fsin" : "+t"(r));

	return r;
}

double cos(double x)
{
	return sin(1.5708 + x);
}

double tan(double x)
{
	return sin(x)/sin(1.5708 + x);
}

uint32_t hwrand32()
{
	uint32_t v;
	__asm__ volatile("rdrand %0" : "=a"(v)::);
	if(v == 0) {
		return hwrand32();
	}
	return v;
}

uint16_t hwrand16()
{
	return (uint16_t) hwrand32();
}

uint8_t hwrand8()
{
	return (uint8_t) hwrand32();
}

uint32_t rgb(int r, int g, int b)
{
	return ((r << 16) | (g << 8) | b);
}
