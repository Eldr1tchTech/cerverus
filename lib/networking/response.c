#include "response.h"

#include "networking_utils.h"
#include "core/cmemory.h"

#include <string.h>
#include <stdio.h>

void response_parse(response* res, char* raw_response) {

}

char* response_serialize(response* res) {
    char* raw_response = "";

    status_line* s_lin = &res->stat_lin;

    // Resolve predefined status_info lookup
    if (s_lin->lin_type == status_line_type_predefined)
    {
        int temp_status_code = status_info[s_lin->status_type].status_code;
        s_lin->reason_phrase = status_info[s_lin->status_type].reason_phrase;
        s_lin->status_code = temp_status_code;
    }

    char* res_stat_lin_buffer = (char *)cmem_allocate(MEMORY_TAG_STRING, sizeof(char *) * 64);
    int chars_written = snprintf(res_stat_lin_buffer, sizeof(res_stat_lin_buffer),
                                    "%s %i %s\r\n", serialize_protocol_version(s_lin->p_version),
                                    s_lin->status_code, s_lin->reason_phrase);
}