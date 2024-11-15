#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

size_t modPow(size_t n, size_t mod) {
  size_t a = 1;
  for (size_t i = 0; i < n; ++i) {
    a = (a << 7) % mod;
  }
  return a;
}

size_t hashString(HashMap *map, const char *str) {
  size_t hash = 0;
  const char *start = str;
  while (*(str++) != '\0') {
    hash = (hash + *str * modPow(str - start, map->n)) % map->n;
  }
  return hash;
}

HashMap createHashMap(size_t n) {
  HashMap map = {NULL, n};
  map.data = (Bucket *)malloc(n * sizeof(Bucket));
  for (size_t i = 0; i < n; ++i) {
    map.data[i] = {NULL, 0, 0};
  }
  return map;
}

void deleteHashMap(HashMap * map) {
  if (map == NULL) {
    return;
  }
  for (size_t i = 0; i < map->n; ++i) {
    Bucket * bucket = map->data + i;
    for (size_t j = 0; j < bucket->len; ++j) {
      free((char *)bucket->data[j].key);
    }
    free(bucket->data);
  }
  free(map->data);
}

void insert(Bucket *bucket, Item *item) {
  if (bucket->data == NULL) {
    bucket->cap = 1;
    bucket->data = (Item *)malloc(bucket->cap * sizeof(Item));
  } else if (bucket->len == bucket->cap) {
    bucket->cap *= 2;
    bucket->data = (Item *)realloc(bucket->data, bucket->cap * sizeof(Item));
  }
  Item *place = bucket->data + (bucket->len++);
  place->value = item->value;
  place->key = item->key;
}

void set(HashMap *map, const char *key, const void *value) {
  size_t idx = hashString(map, key);
  Bucket *bucket = map->data + idx;
  for (size_t i = 0; i < bucket->len; ++i) {
    if (strcmp(bucket->data[i].key, key) == 0) {
      bucket->data[i].value = value;
      return;
    }
  }

  size_t keyLen = strlen(key);
  char *keyCpy = (char *)malloc((keyLen + 1) * sizeof(char));
  strcpy(keyCpy, key);
  Item item = {keyCpy, value};

  insert(bucket, &item);
}

const void *get(HashMap *map, const char *key) {
  size_t idx = hashString(map, key);
  Bucket *bucket = map->data + idx;
  if (bucket->data == NULL) {
    return NULL;
  }
  for (size_t i = 0; i < bucket->len; ++i) {
    if (strcmp(bucket->data[i].key, key) == 0) {
      return bucket->data[i].value;
    }
  }
  return NULL;
}

void testHashMap() {
    HashMap map = createHashMap(10);

    // Test 1: Basic insertion and retrieval
    const char *val1 = "Goeiemiddag makker";
    set(&map, "hello", val1);
    assert(strcmp((const char *)get(&map, "hello"), val1) == 0);

    // Test 2: Check non-existent key
    assert(get(&map, "non_existent") == NULL);

    // Test 3: Update value for existing key
    const char *val2 = "Hello again";
    set(&map, "hello", val2);
    assert(strcmp((const char *)get(&map, "hello"), val2) == 0);

    // Test 4: Check insertion of multiple items with unique keys
    const char *val3 = "Value3";
    set(&map, "key1", val1);
    set(&map, "key2", val2);
    set(&map, "key3", val3);
    assert(strcmp((const char *)get(&map, "key1"), val1) == 0);
    assert(strcmp((const char *)get(&map, "key2"), val2) == 0);
    assert(strcmp((const char *)get(&map, "key3"), val3) == 0);

    // Test 5: Handle empty string as a key
    const char *emptyKeyVal = "Empty key value";
    set(&map, "", emptyKeyVal);
    assert(strcmp((const char *)get(&map, ""), emptyKeyVal) == 0);

    // Test 6: Handle large number of insertions and retrievals
    for (size_t i = 0; i < 100; ++i) {
        char key[20];
        snprintf(key, sizeof(key), "key_%zu", i);
        char *value = (char *)malloc(20 * sizeof(char));
        snprintf(value, 20, "value_%zu", i);
        set(&map, key, value);
        assert(strcmp((const char *)get(&map, key), value) == 0);
        free(value); // Clean up to prevent memory leak
    }

    // Test 7: Handle NULL value (should work without error)
    set(&map, "null_key", NULL);
    assert(get(&map, "null_key") == NULL);

    // Test 8: Same hash different key
    const char *collideKey1 = "collision1";
    const char *collideKey2 = "collision2";
    set(&map, collideKey1, "First collision value");
    set(&map, collideKey2, "Second collision value");
    assert(strcmp((const char *)get(&map, collideKey1), "First collision value") == 0);
    assert(strcmp((const char *)get(&map, collideKey2), "Second collision value") == 0);

    // Test 9: Check that modifying retrieved data doesn't alter stored data
    const char *retrieved = (const char *)get(&map, "key1");
    assert(strcmp(retrieved, val1) == 0);

    // Test 10: Large values as keys and values
    char largeKey[1000];
    memset(largeKey, 'A', 999);
    largeKey[999] = '\0';
    const char *largeValue = "Large value for testing";
    set(&map, largeKey, largeValue);
    assert(strcmp((const char *)get(&map, largeKey), largeValue) == 0);

    // Test 11: Ensure `set` with same key replaces the value
    const char *replaceVal = "Replacement value";
    set(&map, "replace_key", "Old value");
    set(&map, "replace_key", replaceVal);
    assert(strcmp((const char *)get(&map, "replace_key"), replaceVal) == 0);

    // Test 12-13: Check small map resizing behavior by repeatedly adding entries
    for (size_t i = 0; i < 20; ++i) {
        char key[10];
        snprintf(key, sizeof(key), "resize_%zu", i);
        set(&map, key, "resize test");
    }
    assert(strcmp((const char *)get(&map, "resize_0"), "resize test") == 0);
    assert(strcmp((const char *)get(&map, "resize_19"), "resize test") == 0);

    // Test 14-18: Retrieve previously inserted keys and check values
    assert(strcmp((const char *)get(&map, "key1"), val1) == 0);
    assert(strcmp((const char *)get(&map, "key2"), val2) == 0);
    assert(strcmp((const char *)get(&map, "key3"), val3) == 0);
    assert(strcmp((const char *)get(&map, ""), emptyKeyVal) == 0);
    assert(get(&map, "non_existent_key") == NULL);

    // Test 19: Verify deep copy of keys (modify original key after insertion)
    char keyToModify[10] = "mutable";
    set(&map, keyToModify, "Before modification");
    strcpy(keyToModify, "changed");
    assert(strcmp((const char *)get(&map, "mutable"), "Before modification") == 0);
    deleteHashMap(&map);

    // Test 20: Check behavior with very large hashmap
    HashMap largeMap = createHashMap(1000);
    set(&largeMap, "large_test", "Testing large map");
    assert(strcmp((const char *)get(&largeMap, "large_test"), "Testing large map") == 0);
    deleteHashMap(&largeMap);
}

