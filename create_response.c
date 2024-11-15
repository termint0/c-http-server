#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "create_response.h"
#include "hashmap.h"
#include "mystring.h"

#define TIME_BUF_LEN 40

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

String getResponseLine(Response *response) {
  String responseLine = {(char *)malloc(50 * sizeof(char)), 0, 50};
  responseLine = strCat(responseLine, response->httpVersion);
  responseLine = strCharsCat(responseLine, " ");

  if (response->status_code > 999 || response->status_code < -999) {
    response->status_code = 500;
  }
  char statusCode[5];
  sprintf(statusCode, "%d", response->status_code);
  responseLine = strCharsCat(responseLine, statusCode);
  responseLine = strCharsCat(responseLine, " ");

  const char *status = "OK";
  responseLine = strCharsCat(responseLine, status);
  responseLine = strCharsCat(responseLine, "\r\n");
  return responseLine;
}

String headerToLine(Item *item) {
  String headersString = {(char *)malloc(20 * sizeof(char)), 0, 20};
  headersString = strCharsCat(headersString, item->key);
  headersString = strCharsCat(headersString, ": ");
  headersString = strCharsCat(headersString, (char *)item->value);
  headersString = strCharsCat(headersString, "\r\n");
  return headersString;
}

String headersToString(Response *response) {
  String headersString = {(char *)malloc(50 * sizeof(char)), 0, 50};

  HashMap *headers = response->headers;
  Iterator it = iterMap(headers);
  while (it.item != NULL) {
    String headerLineStr = headerToLine(it.item);
    headersString = strCat(headersString, headerLineStr);
    free(headerLineStr.data);
    it = next(headers, &it);
  }
  return headersString;
}

String responseToString(Response *response) {
  String responseStr = {(char *)malloc(100 * sizeof(char)), 0, 100};
  String respLine = getResponseLine(response);
  responseStr = strCat(responseStr, respLine);
  free(respLine.data);
  String headerStr = headersToString(response);
  responseStr = strCat(responseStr, headerStr);
  free(headerStr.data);
  if (response->data.data == NULL) {
    return responseStr;
  }

  responseStr = strCharsCat(responseStr, "\r\n");
  responseStr = strCat(responseStr, response->data);
  return responseStr;
}

void getDate(char *buf) {
  time_t t = time(NULL);
  tm *now = localtime(&t);
  strftime(buf, TIME_BUF_LEN, "%a, %d %b %Y %H:%M:%S %Z", now);
}

Response getDefaultResponse() {
  HashMap *headers = (HashMap *)malloc(sizeof(HashMap));
  *headers = createHashMap(50);
  set(headers, "Server", strdup("TermintsThing"));
  set(headers, "Content-type", strdup("text/html"));
  char date[TIME_BUF_LEN];
  getDate(date);
  set(headers, "Date", strdup(date));
  const char httpVersion[] = "HTTP/1.1";
  String http = {strdup(httpVersion), sizeof(httpVersion) - 1,
                 sizeof(httpVersion)};
  return {200, http, {NULL, 0, 0}, headers};
}

Response get404Response() {
  Response response = getDefaultResponse();
  response.status_code = 404;
  char html[] = "Not found";
  response.data = {strdup(html), sizeof(html) - 1, sizeof(html)};
  return response;
}

Response get501Response() {
  Response response = getDefaultResponse();
  response.status_code = 501;
  char html[] = "Not implemented";
  response.data = {strdup(html), sizeof(html) - 1, sizeof(html)};
  return response;
}

void freeResponse(Response *response) {
  if (response == NULL) {
    return;
  }
  HashMap *headers = response->headers;
  Iterator it = iterMap(headers);
  while (it.item != NULL) {
    free((void*)it.item->value);
    it = next(headers, &it);
  }
  deleteHashMap(response->headers);
  free(response->headers);
  free(response->data.data);
  free(response->httpVersion.data);
}
