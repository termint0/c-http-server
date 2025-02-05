#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "create_response.h"
#include "hashmap.h"
#include "mystring.h"

#define TIME_BUF_LEN 40
void setHeader(HashMap *headers, const char *key, const char *val) {
  free((void *)get(headers, key));
  set(headers, key, strdup(val));
}

/**
 * @brief Create string representing first line of response (HTTP/1.1 200 OK)
 * and the newline chars.
 *
 * @param  pointer to response struct. Function does not take ownership.
 *
 * @return String with first line of response in it. Needs to be freed by
 * caller.
 */
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

/*
 * Create string with the header line (Content-type: text/html) and the
 * newline chars.
 *
 * Response: pointer to response struct. Function does not take ownership.
 *
 * returns: String with header line in it. Needs to be freed by
 * caller.
 */
String headerToLine(Item *item) {
  String headersString = {(char *)malloc(20 * sizeof(char)), 0, 20};
  headersString = strCharsCat(headersString, item->key);
  headersString = strCharsCat(headersString, ": ");
  headersString = strCharsCat(headersString, (char *)item->value);
  headersString = strCharsCat(headersString, "\r\n");
  return headersString;
}

/*
 * Create string with the headers and the
 * newline chars.
 *
 * Response: pointer to response struct. Function does not take ownership.
 *
 * returns: String with header line in it. Needs to be freed by
 * caller.
 */
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

/*
 * description
 *
 * buf: buffer of length at least TIME_BUF_LEN to write date into.
 *
 * returns: void
 */
void getDate(char *buf) {
  time_t t = time(NULL);
  struct tm *now = localtime(&t);
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
  Response resp = {200, http, {NULL, 0, 0}, headers};
  return resp;
}

Response get404Response() {
  Response response = getDefaultResponse();
  response.status_code = 404;
  char html[] = "Not found";
  String respStr = {strdup(html), sizeof(html) - 1, sizeof(html)};
  response.data = respStr;
  return response;
}

Response get501Response() {
  Response response = getDefaultResponse();
  response.status_code = 501;
  char html[] = "Not implemented";
  String respStr = {strdup(html), sizeof(html) - 1, sizeof(html)};
  response.data = respStr;
  return response;
}

void freeResponse(Response *response) {
  if (response == NULL) {
    return;
  }
  HashMap *headers = response->headers;
  Iterator it = iterMap(headers);
  while (it.item != NULL) {
    free((void *)it.item->value);
    it = next(headers, &it);
  }
  deleteHashMap(response->headers);
  free(response->headers);
  free(response->data.data);
  free(response->httpVersion.data);
}
