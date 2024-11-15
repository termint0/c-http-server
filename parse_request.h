#include "hashmap.h"
#pragma once

typedef struct request {
  char* data;
  char* method;
  char* uri;
  char* version;
  HashMap * headers;
} Request;

Request *parseRequest(char *str);
