#pragma once

#include "networking_types.inl"

void response_parse(response* res, char* raw_response);

char* response_serialize(response* res);