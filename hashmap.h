#include <stdlib.h>
#pragma once

typedef struct item {
  const char *key;
  const void *value;
} Item;

typedef struct bucket {
  Item *data;
  size_t len;
  size_t cap;
} Bucket;

typedef struct HashMap {
  Bucket *data;
  size_t n;
} HashMap;

HashMap createHashMap(size_t n);
void set(HashMap *map, const char *key, const void *value);
const void *get(HashMap *map, const char *key);
void deleteHashMap(HashMap *map);
