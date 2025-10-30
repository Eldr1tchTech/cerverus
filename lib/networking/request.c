#include "request.h"

#include "core/logger.h"
#include "networking_utils.h"

// move to another file and create a string library
#include <string.h>


// maybe add an failure return value
void request_parse(request* req, char* raw_request) {
    char* body_start = strstr(raw_request, "\r\n\r\n");
    if (body_start)
    {
        *body_start = "\0";
        req->body = body_start + 4; // Maybe check for null here?
    } else {
        req->body = NULL;
    }

    char* l1 = strtok(raw_request, "\n");
    parse_headers(req, strtok(NULL, "\0"));

    req->method = parse_http_method(strtok(l1, " "));
    req->target = strtok(NULL, " ");
    req->p_version = strtok(NULL, " ");
}

// TODO: Needed in client.
char* request_serialize(request* req) {

}