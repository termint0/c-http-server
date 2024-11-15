#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "create_response.h"
#include "http_server.h"
#include "parse_request.h"

#define BUFFLEN 4096

Server *getServer(int addr, int port) {
  Server *server = (Server *)malloc(sizeof(Server));

  int server_fd;
  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    return NULL;
  }

  // Forcefully attaching socket to the port 8080
  int opt = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    return NULL;
  }

  server->address.sin_family = AF_INET;
  server->address.sin_addr.s_addr = addr;
  server->address.sin_port = htons(port);

  server->fileDescriptor = server_fd;
  return server;
}

void attachHandlerMap(Server *server, HashMap *handlers) {
  server->handlers = handlers;
}

bool runServer(Server *server) {
  // Forcefully attaching socket to the port 8080
  if (bind(server->fileDescriptor, (struct sockaddr *)&server->address,
           sizeof(server->address)) < 0) {
    perror("bind failed");
    return false;
  }
  if (listen(server->fileDescriptor, 3) < 0) {
    perror("listen");
    return false;
  }
  socklen_t addrlen = sizeof(server->address);
  int new_socket;
  while (true) {

    if ((new_socket = accept(server->fileDescriptor,
                             (struct sockaddr *)&server->address, &addrlen)) <
        0) {
      perror("accept");
      return false;
    }
    ssize_t valread;

    char buffer[BUFFLEN] = {0};
    valread = read(new_socket, buffer,
                   BUFFLEN - 1); // subtract 1 for the null
                                 // terminator at the end
    Request *request = parseRequest(buffer);

    String responseStr;
    if (request == NULL) {
      /*responseStr = "ERROR";*/
    } else {
      Response *(*handler)(Request *) =
          (Response * (*)(Request *)) get(server->handlers, request->uri);
      if (handler != NULL) {
        Response *response = handler(request);
        responseStr = responseToString(response);

        freeResponse(response);
        free(response);
      } else {
        Response r = get404Response();
        responseStr = responseToString(&r);
        freeResponse(&r);
        /*free(responseStr.data);*/
      }
    }
    send(new_socket, responseStr.data, responseStr.len, 0);
    free(responseStr.data);

    freeRequest(request);
    free(request);
    close(new_socket);
  }

  // closing the connected socket
  // closing the listening socket
  close(server->fileDescriptor);
  return true;
}
void stopServer(Server *server);
void freeServer(Server *server) { free(server); }
