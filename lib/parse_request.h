#include "hashmap.h"
#pragma once

typedef struct request {
  char* data;
  char* method;
  char* uri;
  char* version;
  HashMap * headers;
} Request;

void freeRequest(Request *request);
Request *parseRequest(char *str);
