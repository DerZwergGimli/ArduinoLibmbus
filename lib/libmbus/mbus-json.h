#ifndef _MBUS_JSON_H_
#define _MBUS_JSON_H_

#include "mbus-protocol.h"
#include <ArduinoJson.h>

char *mbus_frame_data_json(mbus_frame_data *data);
char *mbus_frame_data_json_normalized(mbus_frame_data *data);
char *mbus_data_fixed_json(mbus_data_fixed *data);
char *mbus_data_variable_json(mbus_data_variable *data);
char *mbus_data_variable_json_normalized(mbus_data_variable *data);
char *mbus_data_error_json(int error);

#endif