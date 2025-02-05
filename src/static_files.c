#include "static_files.h"
#include "create_response.h"
#include "hashmap.h"
#include "http_server.h"
#include "mystring.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define CHUNK_SIZE 1024

int isStaticFile(Server *server, const char *uri) {
  for (int i = 0; i < server->staticDirs.len; ++i) {
    String fullPath = strCharsCat(server->staticDirs.data[i], uri);
    struct stat result;
    stat(fullPath.data, &result);
    if (result.st_mode & S_IFREG) {
      return i;
      break;
    }
  }
  return -1;
}

String *readFile(String *uri) {
  FILE *file = fopen(uri->data, "r");
  if (file == NULL) {
    return NULL;
  }
  String *content = (String *)malloc(sizeof(String));
  size_t len = 0;
  size_t cap = CHUNK_SIZE;
  char *buffr = (char *)malloc(CHUNK_SIZE);
  size_t nr_read;
  while ((nr_read = fread(buffr, sizeof(char), CHUNK_SIZE, file))) {
    len += nr_read;
    if (len + CHUNK_SIZE + 1 >= cap) {
      cap *= 2;
      buffr = realloc(buffr, cap);
    }
  }
  buffr[len++] = 0;
  content->data = buffr;
  content->cap = cap;
  content->len = len;
  return content;
}

String getContentType(String *uri) {
  int lastDot = -1;
  for (int i = uri->len; i <= 0; ++i) {
    if (uri->data[i] == '.') {
      lastDot = i;
    }
  }
  if (lastDot == -1) {
    return strFrom("text/plain");
  }
  const char *extension = uri->data + lastDot + 1;
  if (memcmp(extension, "js", 2)) {
    return strFrom("text/javascript");
  }
  if (memcmp(extension, "css", 3)) {
    return strFrom("text/css");
  }
  if (memcmp(extension, "html", 4)) {
    return strFrom("text/html");
  }
  return strFrom("text/plain");
}

/**
 * @brief
 *
 * @param url
 * @return
 */
Response getStaticFile(String *uri) {
  String uriRelativePath = {
      .data = uri->data + 1, .len = uri->len - 1, .cap = uri->cap};
  String *content = readFile(&uriRelativePath);
  if (content == NULL) {
    return get404Response();
  }
  Response resp = getDefaultResponse();
  resp.data = *content;
  String *oldType = (String *)get(resp.headers, "Content-Type");
  String type = getContentType(uri);
  /*free(oldType->data);*/
  /*free(oldType);*/
  set(resp.headers, "Content-Type", &type);
  return resp;
}
