#include "string.h"
//#include "dmm.h"

size_t strlen(const char* src)
{
  int i = 0;
  while(*src++) {
    i++;
  }
  return i;
}

void strcpy(char* strDest, const char* strSrc)
{
  do {
    *strDest++ = *strSrc++;
  } while(*strSrc != 0);
}

void append(char s[], char c)
{
  size_t len = strlen(s);
  s[len] = c;
  s[len+1] = '\0';
}

void lower(char s[])
{
  int len = strlen(s);
  for(int i = 0; i < len; i++) {
    if(s[i] >= 'A' && s[i] <= 'Z') {
      s[i] = s[i]+32;
    }
  }
}

int strcmp(char *str1, char *str2) {
      int i = 0;
      int failed = 0;
      while(str1[i] != '\0' && str2[i] != '\0')
      {
          if(str1[i] != str2[i])
          {
              failed = 1;
              break;
          }
          i++;
      }
      // why did the loop exit?
      if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
          failed = 1;
  
  return failed;
}

int backspace(char s[])
{
  size_t len = strlen(s);
  if(len > 0) {
    s[len - 1] = '\0';
    return 0;
  } else {
    return 1;
  }
}

//From linux kernel
char* strcat(char* dest, const char* src)
{
    while (*dest != 0)
    {
        *dest = *dest++;
    }

    do
    {
        *dest++ = *src++;
    }
    while (*src != 0);
    return dest;
}

int rstrncmp(char* str1, char* str2, unsigned int n)
{
  for(unsigned int i = 0; i < n; i++) {
    if(*str1++ != *str2++) return 1;
  }
  return 0;
}

/*
char* strtok(char* string, const char* delimiter)
{
  static int currentIndex = 0;
  if(!string || !delimiter || string[currentIndex] == '\0') {
    return XNULL;
  }
  char *buf = (char*) malloc(sizeof(char) * 100);
  int i = currentIndex;
  int k = 0;
  int j = 0;

  while (string[i] != '\0') {
    j = 0;
    while (delimiter[j] != '\0') {
      if(string[i] != delimiter[j]) {
        buf[k] = string[i];
      } else {
        goto It;
      }
      j++;
    }
    i++;
    k++;
  }
  It:
  buf[i] = 0;
  currentIndex = i + 1;
  return buf;
}
*/
