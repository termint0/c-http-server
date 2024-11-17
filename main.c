#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "create_response.h"
#include "hashmap.h"
#include "http_server.h"
#include "mystring.h"

Server *g_Server = NULL;

Response *indexEndpoint(Request *request) {
  Response *response = (Response *)malloc(sizeof(Response));
  *response = getDefaultResponse();
  response->status_code = 200;
  char html[] = "<b>Hello world</b>\r\n<script src=\"/a.js\"></script>";
  int a;
  response->data = {strdup(html), sizeof(html) - 1, sizeof(html)};
  return response;
}

Response *scriptEndpoint(Request *request) {
  Response *response = (Response *)malloc(sizeof(Response));
  *response = getDefaultResponse();
  response->status_code = 200;
  setHeader(response->headers, "Content-Type", "text/javascript");
  char html[] = "console.log(\"hi\")";
  int a;
  response->data = {strdup(html), sizeof(html) - 1, sizeof(html)};
  return response;
}

void onSigInt(int sig) {
  g_Server->stopRequest = true;
  close(g_Server->fileDescriptor);
}

int main(int argc, char const *argv[]) {
  Server *server = getServer(0, 8080);
  g_Server = server;
  HashMap endpointMap = createHashMap(50);

  set(&endpointMap, "/", (void *)indexEndpoint);
  set(&endpointMap, "/a.js", (void *)scriptEndpoint);
  attachHandlerMap(server, &endpointMap);
  signal(SIGINT, onSigInt);

  runServer(server);
  stopServer(server);
  freeServer(server);
  /*char *str = strdup("GET /index.html HTML1.1\n"*/
  /*                   "Thing:  aaaa\n"*/
  /*                   "Other-Thing:  bbbb\n"*/
  /*                   "\n"*/
  /*                   "datadata");*/
  /*Request *thing = parseRequest(str);*/
  /**/
  /*if (thing == NULL) {*/
  /*  printf("ERROR\n");*/
  /*  return 1;*/
  /*}*/
  /*printf("%s %s %s", thing->method, thing->uri, thing->version);*/
  /*/*runServer(a);*/
  deleteHashMap(&endpointMap);

  return 0;
}
