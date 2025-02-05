#include "mystring.h"
#include <string.h>
/*
 * Concatenates Strings "a" and "b". If buffer "a" doesn't have enough memory,
 * reallocates its memory (therefor invalidates the String passed in as "a").
 *
 * a: String to concatenate. Its buffer will be used for the final string. Do
 * not use the struct passed after calling the function, the pointer might point
 * to freed memory.
 *
 * b: String to concatenate. Its buffer is safe to use after calling.
 *
 * returns: String struct with concatenated string. Needs to be freed by caller.
 */
String strCat(String a, String b) {
  if (a.len + b.len >= a.cap) {
    a.cap = (a.cap + b.cap) * 2;
    a.data = (char *)realloc(a.data, a.cap * sizeof(char));
  }
  for (size_t i = 0; i < b.len; ++i) {
    a.data[a.len++] = b.data[i];
  }
  a.data[a.len] = '\0';
  return a;
}

/*
 * Concatenates String "a" and char array "b". If buffer "a" doesn't have enough
 * memory, reallocates its memory (therefor invalidates the String passed in as
 * "a").
 *
 * a: String to concatenate. Its buffer will be used for the final string. Do
 * not use the struct passed after calling the function, the pointer might point
 * to freed memory.
 *
 * b: null terminated char array to concatenate. The pointer remains safe to use
 * after calling.
 *
 * returns: String struct with concatenated string. Needs to be freed by caller.
 */
String strCharsCat(String a, const char *b) {
  size_t bLen = strlen(b);
  if (a.len + bLen >= a.cap) {
    a.cap = (a.cap + bLen) * 2;
    a.data = (char *)realloc(a.data, a.cap * sizeof(char));
  }
  for (size_t i = 0; i < bLen; ++i) {
    a.data[a.len++] = b[i];
  }
  a.data[a.len] = '\0';
  return a;
}

String strFrom(const char *str) {
  size_t len = strlen(str);
  String string =  {.data = strdup(str), .len = len, .cap = len + 1};
  return string;
}
