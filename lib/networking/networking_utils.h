#pragma once

#include "networking_types.inl"

http_method parse_http_method(char* raw_method);

char* serialize_http_method(http_method method);

protocol_version parse_protocol_version(char* raw_version);

char* serialize_protocol_version(protocol_version version);

void request_add_header(request* req, header h);

void parse_headers(request* req, char* raw_headers);

char* serialize_headers(header* headers, int header_count);