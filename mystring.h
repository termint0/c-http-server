#include <stdlib.h>
#pragma once

typedef struct myString {
  char * data;
  size_t len;
  size_t cap;
} String;
