#include "http_server.h"
#include "create_response.h"
#include "mystring.h"
#pragma once

int isStaticFile(Server *server, const char *url);
String *readFile(String *url);
Response getStaticFile(String *url);
