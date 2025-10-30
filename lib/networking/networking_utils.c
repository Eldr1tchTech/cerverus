#include "networking_utils.h"

http_method parse_http_method(char *raw_method)
{
    if (strcmp(raw_method, "GET") == 0)
    {
        return http_method_get;
    }
    else if (strcmp(raw_method, "POST") == 0)
    {
        return http_method_post;
    }
    else if (strcmp(raw_method, "PUT") == 0)
    {
        return http_method_put;
    }
    else if (strcmp(raw_method, "DELETE") == 0)
    {
        return http_method_delete;
    }
    else if (strcmp(raw_method, "PATCH") == 0)
    {
        return http_method_patch;
    }
    else if (strcmp(raw_method, "HEAD") == 0)
    {
        return http_method_head;
    }
    else if (strcmp(raw_method, "OPTIONS") == 0)
    {
        return http_method_options;
    }
    else if (strcmp(raw_method, "TRACE") == 0)
    {
        return http_method_trace;
    }
    else if (strcmp(raw_method, "CONNECT") == 0)
    {
        return http_method_connect;
    }

    LOG_DEBUG("parse_http_method - No method could be parsed from the input: %s. Returning unknown.", raw_method);
    return http_method_unknown;
}

char *serialize_http_method(http_method method)
{
    switch (method)
    {
    case http_method_get:
        return "GET";
    case http_method_post:
        return "POST";
    case http_method_put:
        return "PUT";
    case http_method_delete:
        return "DELETE";
    case http_method_patch:
        return "PATCH";
    case http_method_head:
        return "HEAD";
    case http_method_options:
        return "OPTIONS";
    case http_method_trace:
        return "TRACE";
    case http_method_connect:
        return "CONNECT";
    default:
        LOG_DEBUG("serialize_http_method - Unable to serialize method. Returning UNKOWN.");
        return "UNKNOWN";
    }
}

protocol_version parse_protocol_version(char *raw_version)
{
    if (strcmp(raw_version, "HTTP/1.1") == 0)
    {
        return protocol_version_http1p1;
    }
    else if (strcmp(raw_version, "HTTP/2") == 0)
    {
        return protocol_version_http2;
    }

    LOG_DEBUG("parse_protocol_version - No version could be parsed from the input: %s. Returning unknown.", raw_version);
    return protocol_version_unknown;
}

char* serialize_protocol_version(protocol_version version) {
    switch (version)
    {
    case protocol_version_http1p1:
        return "HTTP/1.1";
    case protocol_version_http2:
        return "HTTP/2";
    default:
        LOG_DEBUG("serialize_protocol_version - Unable to serialize version. Returning UNKNOWN.");
        return "UNKNOWN";
    }
}

void request_add_header(request *req, header h)
{
    int h_count = req->header_count;
    if (h_count != MAX_HEADER_COUNT)
    {
        req->headers[h_count] = h;
        req->header_count++;
    }

    LOG_DEBUG("request_add_header - Attempted to add more headers to an request than are allowed. MAX_HEADER_COUNT = %i", MAX_HEADER_COUNT);
}

void parse_headers(request *req, char *raw_headers)
{
    char *current;
    while (current = strstr(raw_headers, "\r\n"))
    {
        current = "\0";
        header h = {
            .key = strtok(raw_headers, ":"),
            .value = strtok(NULL, "\r"),
        };
        raw_headers = current + 4;
    }
}