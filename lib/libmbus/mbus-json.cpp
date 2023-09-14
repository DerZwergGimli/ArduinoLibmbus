#include "mbus-json.h"

char *mbus_frame_data_json(mbus_frame_data *data)
{
    if (data)
    {
        if (data->type == MBUS_DATA_TYPE_ERROR)
        {
            return mbus_data_error_json(data->error);
        }

        if (data->type == MBUS_DATA_TYPE_FIXED)
        {
            return mbus_data_fixed_json(&(data->data_fix));
        }

        if (data->type == MBUS_DATA_TYPE_VARIABLE)
        {
            return mbus_data_variable_json(&(data->data_var));
        }
    }

    return NULL;
}
char *mbus_frame_data_json_normalized(mbus_frame_data *data) {}
char *mbus_data_fixed_json(mbus_data_fixed *data) {}
char *mbus_data_variable_json(mbus_data_variable *data) {}
char *mbus_data_variable_json_normalized(mbus_data_variable *data) {}

char *mbus_data_error_json(int error)
{

    DynamicJsonDocument doc(1024);
    char buff[1024];

    if (buff == NULL)
        return NULL;

    doc["MBusData"]["SlaveInformation"]["Error"] = mbus_data_error_lookup(error);

    serializeJson(doc, buff);
    return buff;
}
