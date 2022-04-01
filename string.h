#ifndef __STRING_H__
#define __STRING_H__

#include "stddef.h"

size_t strlen(const char* str);
char* strcat(char* dest, const char* src);
void append(char s[], char c);
void lower(char s[]);
int strcmp(char *s1, char *s2);
int rstrncmp(char* str1, char* str2, unsigned int n);
int backspace(char s[]);
void strcpy(char* strDest, const char* strSrc);

#endif
