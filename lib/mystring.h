#include <stdlib.h>
#pragma once

typedef struct myString {
  char *data;
  size_t len;
  size_t cap;
} String;

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
String strCat(String a, String b);

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
String strCharsCat(String a, const char *b);
String strFrom(const char *str) ;
