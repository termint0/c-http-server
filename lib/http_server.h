#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>

#include "hashmap.h"
#include "mystring.h"
#include "parse_request.h"
#pragma once

typedef struct string_array {
  String * data;
  size_t len;
  size_t cap;
} StringArr;

typedef struct server {
  int fileDescriptor;
  struct sockaddr_in address;
  HashMap *handlers;
  volatile sig_atomic_t stopRequest;
  bool running;
  StringArr staticDirs; 
} Server;

Server *getServer(int addr, int port);
void attachHandlerMap(Server *server, HashMap *handlers);
void addStaticPath(Server *server, String * path);
bool runServer(Server *server);
void stopServer(Server *server);
void freeServer(Server *server);
/*void addEndpoint(Server *server, const char *endpoint, Response*
 * (*fun)(Request*));*/
