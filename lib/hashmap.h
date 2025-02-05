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

typedef struct hashMap {
  Bucket *data;
  size_t n;
} HashMap;

typedef struct iterator {
  Item *item;
  size_t bucketIdx;
  size_t itemIdx;
} Iterator;

HashMap createHashMap(size_t n);
void set(HashMap *map, const char *key, const void *value);
const void *get(HashMap *map, const char *key);
void deleteHashMap(HashMap *map);
Iterator iterMap(HashMap *map);
Iterator next(HashMap *map, Iterator *it);
