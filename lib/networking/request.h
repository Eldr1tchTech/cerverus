#pragma once

#include "networking_types.inl"

void request_parse(request* req, char* raw_request);

char* request_serialize(request* req);