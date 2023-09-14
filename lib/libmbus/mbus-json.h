//
// Header for creating a JSON representation of a frame
//

#ifndef __MBUS_JSON_H__
#define __MBUS_JSON_H__

#include "mbus-protocol.h"

#ifdef __cplusplus
extern "C"
{
#endif

    char *mbus_frame_json(mbus_frame *frame);

    char *mbus_data_variable_header_json(mbus_data_variable_header *header);

    char *mbus_data_variable_record_json(mbus_data_record *record, int record_cnt, int frame_cnt,
                                         mbus_data_variable_header *header);

    char *mbus_frame_data_json_normalized(mbus_frame_data *data);

    char *mbus_data_error_json(int error);

    char *mbus_data_fixed_json(mbus_data_fixed *data);

    char *mbus_data_variable_json(mbus_data_variable *data);

    char *mbus_data_variable_json_normalized(mbus_data_variable *data);

    int mbus_str_json_encode(unsigned char *dst, const unsigned char *src, size_t max_len);

    char *mbus_frame_data_json(mbus_frame_data *data);

#ifdef __cplusplus
}
#endif

#endif