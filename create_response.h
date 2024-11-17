#include "hashmap.h"
#include "mystring.h"

typedef struct response {
  int status_code;
  String httpVersion;
  String data;
  HashMap *headers;
} Response;

/*
 * Sets the header "key" to "value".
 *
 * headers: hashmap representing the headers
 *
 * key: pointer to null terminated string to serve as the key. Function does not
 * take ownership
 *
 * val: pointer to null terminated string to serve as the value. Function does
 * not take ownership
 *
 * returns: void
 */
void setHeader(HashMap *headers, const char *key, const char *val);

/*
 * Transform Response struct to HTTP string.
 *
 * response: Rensponse to transform
 *
 * returns: HTTP string as String struct. String.data needs to be freed at the
 * end of lifecycle
 */
String responseToString(Response *response);

/*
 * Create default response with Server, Date and HTTP version
 * pre-filled. Content-Type is set to text/html and return code to 200.
 *
 * returns: Response struct with default options. Needs to have its members
 * freed at the end of the lifecycle with freeResponse()
 */
Response getDefaultResponse();

/*
 * Create default  response with Server, Date and HTTP version
 * pre-filled. Content-Type is set to text/html and return code to 404.
 * Response data is set to "Not found"
 *
 * returns: Response struct with default options. Needs to have its members
 * freed at the end of the lifecycle with freeResponse()
 */
Response get404Response();

/*
 * Create default  response with Server, Date and HTTP version
 * pre-filled. Content-Type is set to text/html and return code to 501.
 * Response data is set to "Not implemented"
 *
 * returns: Response struct with default options. Needs to have its members
 * freed at the end of the lifecycle with freeResponse()
 */
Response get501Response();

/*
 * Frees the response's memory.
 *
 * RESULTS IN UNDEFINED BEHAVIOR IF ANYTHING BUT
 * PRIMITIVE TYPES (int, bool, ...) IS STACK ALLOCATED.
 *
 * DOES NOT FREE THE MEMORY ALLOCATED FOR response ITSELF
 *
 * response: Rensponse to free.
 */
void freeResponse(Response *response);
