// Server side C program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "hashmap.h"
#include "http_server.h"

Response *indexEndpoint(Request *request) {
  Response *response = (Response *)malloc(sizeof(Response));
  response->status_code = 200;
  response->data = "<b>Hello world</b>";
  return response;
}

int main(int argc, char const *argv[]) {
  Server *server = getServer(0, 8080);
  HashMap endpointMap = createHashMap(50);
  set(&endpointMap, "/", (void *)indexEndpoint);
  attachEndpointMap(server, &endpointMap);

  runServer(server);
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
