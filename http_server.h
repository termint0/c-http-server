#include <netinet/in.h>

#include "hashmap.h"
#include "parse_request.h"

typedef struct server {
  int fileDescriptor;
  struct sockaddr_in address;
  HashMap * endpoints;
} Server;


typedef struct response {
  int status_code;
  const char* data;
} Response;

Server *getServer(int addr, int port);
void attachEndpointMap(Server *server, HashMap * endpoints);
bool runServer(Server *server);
void stopServer(Server *server);
void freeServer(Server *server);
/*void addEndpoint(Server *server, const char *endpoint, Response* (*fun)(Request*));*/

