#include "hashmap.h"
#include "mystring.h"

typedef struct response {
  int status_code;
  String httpVersion;
  String data;
  HashMap *headers;
} Response;

String responseToString(Response *response);
Response getDefaultResponse();

Response get404Response();

Response get501Response();
void freeResponse(Response * response);
