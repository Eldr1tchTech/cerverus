#pragma once

#include "core/logger.h"
#include <string.h>

#define MAX_HEADER_COUNT 32
#define MAX_RESPONSE_REQUEST_SIZE 8192

typedef enum http_method {
    http_method_get,
    http_method_put,
    http_method_post,
    http_method_delete,
    http_method_head,
    http_method_options,
    http_method_patch,
    http_method_connect,
    http_method_trace,
    http_method_unknown
} http_method;

typedef enum protocol_version {
    protocol_version_http1p1,
    protocol_version_http2,
    protocol_version_unknown
} protocol_version;

typedef struct header
{
    char* key;
    char* value;
} header;

typedef struct request {
    http_method method;
    char* target;
    protocol_version p_version;
    header headers[MAX_HEADER_COUNT];
    int header_count;
    char* body;
} request;

typedef enum status_type {
    // 2xx — Success
    status_type_200 = 0,
    status_type_201,
    status_type_202,
    status_type_204,

    // 3xx — Redirection
    status_type_301,
    status_type_302,
    status_type_304,

    // 4xx — Client Error
    status_type_400,
    status_type_401,
    status_type_403,
    status_type_404,
    status_type_405,

    // 5xx — Server Error
    status_type_500,
    status_type_501,
    status_type_502,
    status_type_503,
    status_type_504,

    status_type_count
} status_type;

typedef enum status_line_type {
    status_line_type_custom,
    status_line_type_predefined
} status_line_type;

typedef struct status_line {
    protocol_version p_version;
    status_line_type lin_type;
    union {
        status_type status_type;
        struct {
            int status_code;
            char *reason_phrase;
        };
    };
} status_line;

typedef struct response {
    status_line stat_lin;
    header headers[MAX_HEADER_COUNT];
    int header_count;
    char *body;
} response;

// Lookup table for predefined statuses
static const struct {
    int status_code;
    const char *reason_phrase;
} status_info[status_type_count] = {
    {200, "OK"},
    {201, "Created"},
    {202, "Accepted"},
    {204, "No Content"},

    {301, "Moved Permanently"},
    {302, "Found"},
    {304, "Not Modified"},

    {400, "Bad Request"},
    {401, "Unauthorized"},
    {403, "Forbidden"},
    {404, "Not Found"},
    {405, "Method Not Allowed"},

    {500, "Internal Server Error"},
    {501, "Not Implemented"},
    {502, "Bad Gateway"},
    {503, "Service Unavailable"},
    {504, "Gateway Timeout"},
};