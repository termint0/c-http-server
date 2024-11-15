#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"
#include "parse_request.h"

#define BUFFLEN 4096
const char KNOWN_METHODS[][10] = {"GET", "POST"};

void freeRequest(Request *request) {
  if (request == NULL) {
    return;
  }
  free(request->data);
  free(request->uri);
  free(request->method);
  free(request->version);
  HashMap *headers = request->headers;
  Iterator it = iterMap(headers);
  while (it.item != NULL) {
    free((void*)it.item->value);
    it = next(headers, &it);
  }
  deleteHashMap(request->headers);
  free(request->headers);
}

char *skipWhiteSpace(char *str) {
  while (*str != '\0' && *str == ' ') {
    ++str;
  }
  return str;
}

char *trimWhiteSpace(char *start, char *end) {
  while (end > start && isspace(*end)) {
    --end;
  }
  return end;
}

Request *parseRequestLine(char *str) {
  if (str == NULL || *str == 0) {
    return NULL;
  }
  char *method = str;
  while (*str != ' ' && *str != '\0' && str - method < BUFFLEN) {
    str++;
  }
  *str = '\0';
  str = skipWhiteSpace(++str);
  bool methodGood = false;
  for (size_t i = 0; i < sizeof(KNOWN_METHODS); ++i) {
    if (strcmp(method, KNOWN_METHODS[i]) == 0) {
      methodGood = true;
      break;
    }
  }
  if (!methodGood) {
    return NULL;
  }

  char *endpoint = str;
  while (*str != ' ' && *str != '\0' && str - method < BUFFLEN) {
    ++str;
  }
  if (*str == 0 || str - method >= BUFFLEN) {
    return NULL;
  }
  *str = '\0';
  str = skipWhiteSpace(++str);

  char *htmlVersion = str;
  while (*str != ' ' && *str != '\0' && str - method < BUFFLEN) {
    str++;
  }
  if (*str != '\0') {
    *str = '\0';
    str = skipWhiteSpace(++str);
    if (*str != '\0') {
      return NULL;
    }
  }

  Request *request = (Request *)malloc(sizeof(Request));
  request->method = strdup(method);
  request->uri = strdup(endpoint);
  request->version = strdup(htmlVersion);
  return request;
}

bool parseHeaderLine(HashMap *headers, char *line) {
  char *str = line;
  char *key = line;
  while (*str != ':') {
    if (*str == '\0') {
      return false;
    }
    ++str;
  }
  *(str++) = '\0';
  str = skipWhiteSpace(str);
  if (*str == '\0') {
    return false;
  }
  char *val = str;
  while (*str != '\0') {
    ++str;
  }
  char * lastchar = trimWhiteSpace(val, str);
  *lastchar = '\0';
  set(headers, key, strdup(val));
  return true;
}

char *getHeaders(HashMap *headers, char *str) {
  *headers = createHashMap(50);

  while (*str != '\0') {
    char *lineStart = str;
    while (*str != '\n' && *str != '\0' && str - lineStart < BUFFLEN) {
      str++;
    }
    *(str++) = '\0';
    if (!parseHeaderLine(headers, lineStart)) {
      return NULL;
    }
    if (*str == '\r' && *(str + 1) == '\n') {
      return str;
    }
  }
  return str;
}

Request *parseRequest(char *str) {
  char *requestLine = str;
  while (*str != '\n' && *str != '\0' && str - requestLine < BUFFLEN) {
    str++;
  }
  *(str++) = '\0';
  Request *request = parseRequestLine(requestLine);
  if (request == NULL) {
    return NULL;
  }

  bool hasHeaders = !(*str == 0 || *str == '\n');
  if (hasHeaders) {
    request->headers = (HashMap *)malloc(sizeof(HashMap));
    str = getHeaders(request->headers, str);
  }
  if (str == NULL) {
    if (request != NULL) {
      free(request->headers);
    }
    free(request);
    return NULL;
  }

  if (*str == '\0') {
    return request;
  }
  request->data = strdup(++str);
  return request;
}
