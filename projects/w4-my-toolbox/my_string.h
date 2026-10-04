#ifndef MY_STRING_H        
#define MY_STRING_H          

#include <stddef.h>         

size_t my_strlen(const char *s);
char *my_strcpy(char *dest, const char *src);
int my_strcmp(const char *s1, const char *s2);
char *my_strcat(char *dest, const char *src);
char *my_strchr(const char *s, int c);
void *my_memcpy(void *dest, const void *src, size_t n);
void *my_memset(void *s, int c, size_t n);

#endif /* MY_STRING_H */
