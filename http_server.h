#include <netinet/in.h>

#include "hashmap.h"
#include "parse_request.h"

typedef struct server {
  int fileDescriptor;
  struct sockaddr_in address;
  HashMap * handlers;
} Server;


Server *getServer(int addr, int port);
void attachHandlerMap(Server *server, HashMap * handlers);
bool runServer(Server *server);
void stopServer(Server *server);
void freeServer(Server *server);
/*void addEndpoint(Server *server, const char *endpoint, Response* (*fun)(Request*));*/

