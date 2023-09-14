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
char *mbus_frame_data_json_normalized(mbus_frame_data *data)
{
    if (data)
    {
        if (data->type == MBUS_DATA_TYPE_FIXED)
        {
            return mbus_data_fixed_xml(&(data->data_fix));
        }

        if (data->type == MBUS_DATA_TYPE_VARIABLE)
        {
            return mbus_data_variable_json_normalized(&(data->data_var));
        }
    }

    return NULL;
}

//------------------------------------------------------------------------------
/// Generate JSON representation of fixed-length frame.
//------------------------------------------------------------------------------
char *mbus_data_fixed_json(mbus_data_fixed *data)
{

    DynamicJsonDocument doc(1024);
    char temp[128];

    char *buff = NULL;
    char str_encoded[256];
    size_t len = 0, buff_size = 8192;

    int val;

    if (data)
    {
        buff = (char *)malloc(buff_size);

        if (buff == NULL)
            return NULL;

        // len += snprintf(&buff[len], buff_size - len, MBUS_XML_PROCESSING_INSTRUCTION);
        // len += snprintf(&buff[len], buff_size - len, "<MBusData>\n\n");
        // len += snprintf(&buff[len], buff_size - len, "    <SlaveInformation>\n");

        sprintf(temp, "%llX", mbus_data_bcd_decode_hex(data->id_bcd, 4));
        doc["MBusData"]["SlaveInformation"]["Id"] = temp;
        // len += snprintf(&buff[len], buff_size - len, "        <Id>%llX</Id>\n", mbus_data_bcd_decode_hex(data->id_bcd, 4));

        mbus_str_json_encode(str_encoded, mbus_data_fixed_medium(data), sizeof(str_encoded));
        doc["MBusData"]["SlaveInformation"]["Medium"] = str_encoded;
        // len += snprintf(&buff[len], buff_size - len, "        <Medium>%s</Medium>\n", str_encoded);

        doc["MBusData"]["SlaveInformation"]["AccessNumber"] = data->tx_cnt;
        // len += snprintf(&buff[len], buff_size - len, "        <AccessNumber>%d</AccessNumber>\n", data->tx_cnt);

        sprintf(temp, "%.2X", data->status);
        doc["MBusData"]["SlaveInformation"]["Medium"] = str_encoded;
        // len += snprintf(&buff[len], buff_size - len, "        <Status>%.2X</Status>\n", data->status);
        // len += snprintf(&buff[len], buff_size - len, "    </SlaveInformation>\n\n");

        doc["DataRecord"][0]["id"] = 0;
        // len += snprintf(&buff[len], buff_size - len, "    <DataRecord id=\"0\">\n");

        mbus_str_json_encode(str_encoded, mbus_data_fixed_function(data->status), sizeof(str_encoded));
        doc["DataRecord"][0]["Function"] = str_encoded;
        // len += snprintf(&buff[len], buff_size - len, "        <Function>%s</Function>\n", str_encoded);

        mbus_str_json_encode(str_encoded, mbus_data_fixed_unit(data->cnt1_type), sizeof(str_encoded));
        doc["DataRecord"][0]["Unit"] = str_encoded;
        // len += snprintf(&buff[len], buff_size - len, "        <Unit>%s</Unit>\n", str_encoded);
        if ((data->status & MBUS_DATA_FIXED_STATUS_FORMAT_MASK) == MBUS_DATA_FIXED_STATUS_FORMAT_BCD)
        {
            sprintf(temp, "%llX", mbus_data_bcd_decode_hex(data->cnt1_val, 4));
            doc["DataRecord"][0]["Value"] = temp;
            //  len += snprintf(&buff[len], buff_size - len, "        <Value>%llX</Value>\n", mbus_data_bcd_decode_hex(data->cnt1_val, 4));
        }
        else
        {

            mbus_data_int_decode(data->cnt1_val, 4, &val);
            sprintf(temp, "%d", val);
            doc["DataRecord"][0]["Value"] = temp;
            // len += snprintf(&buff[len], buff_size - len, "        <Value>%d</Value>\n", val);
        }

        // len += snprintf(&buff[len], buff_size - len, "    </DataRecord>\n\n");

        doc["DataRecord"][1]["id"] = 1;
        // len += snprintf(&buff[len], buff_size - len, "    <DataRecord id=\"1\">\n");

        mbus_str_json_encode(str_encoded, mbus_data_fixed_function(data->status), sizeof(str_encoded));
        doc["DataRecord"][1]["Function"] = str_encoded;
        // len += snprintf(&buff[len], buff_size - len, "        <Function>%s</Function>\n", str_encoded);

        mbus_str_json_encode(str_encoded, mbus_data_fixed_unit(data->cnt2_type), sizeof(str_encoded));
        doc["DataRecord"][1]["Unit"] = str_encoded;
        // len += snprintf(&buff[len], buff_size - len, "        <Unit>%s</Unit>\n", str_encoded);
        if ((data->status & MBUS_DATA_FIXED_STATUS_FORMAT_MASK) == MBUS_DATA_FIXED_STATUS_FORMAT_BCD)
        {
            sprintf(temp, "%llX", mbus_data_bcd_decode_hex(data->cnt1_val, 4));
            doc["DataRecord"][1]["Value"] = temp;
            // len += snprintf(&buff[len], buff_size - len, "        <Value>%llX</Value>\n", mbus_data_bcd_decode_hex(data->cnt2_val, 4));
        }
        else
        {
            mbus_data_int_decode(data->cnt2_val, 4, &val);
            sprintf(temp, "%d", val);
            doc["DataRecord"][1]["Value"] = temp;
            // len += snprintf(&buff[len], buff_size - len, "        <Value>%d</Value>\n", val);
        }

        // len += snprintf(&buff[len], buff_size - len, "    </DataRecord>\n\n");
        // len += snprintf(&buff[len], buff_size - len, "</MBusData>\n");
        // serializeJson(doc, buff);
        return buff;
    }

    return NULL;
}

//------------------------------------------------------------------------------
/// Generate JSON data varibale.
//------------------------------------------------------------------------------
char *mbus_data_variable_json(mbus_data_variable *data)
{

    DynamicJsonDocument doc(1024);
    char temp[128];

    mbus_data_record *record;
    char *buff = NULL, *new_buff;
    size_t len = 0, buff_size = 8192;
    int i;

    if (data)
    {
        buff = (char *)malloc(buff_size);

        if (buff == NULL)
            return NULL;

        // len += snprintf(&buff[len], buff_size - len, MBUS_XML_PROCESSING_INSTRUCTION);
        // len += snprintf(&buff[len], buff_size - len, "<MBusData>\n\n");
        sprintf(temp, "%s", mbus_data_variable_header_json(&(data->header)));
        doc["MBusData"] = temp;
        // len += snprintf(&buff[len], buff_size - len, "%s",
        //                 mbus_data_variable_header_json(&(data->header)));

        // TODO: Make this work "loop"
        // for (record = data->record, i = 0; record; record = record->next, i++)
        // {
        //     if ((buff_size - len) < 1024)
        //     {
        //         buff_size *= 2;
        //         new_buff = (char *)realloc(buff, buff_size);

        //         if (new_buff == NULL)
        //         {
        //             free(buff);
        //             return NULL;
        //         }

        //         buff = new_buff;
        //     }

        //     sprintf(temp, "%s", mbus_data_variable_record_json(record, i, -1, &(data->header)));
        //     doc["MBusData"] = temp;
        //     // len += snprintf(&buff[len], buff_size - len, "%s",
        //     //                 mbus_data_variable_record_json(record, i, -1, &(data->header)));
        // }
        // len += snprintf(&buff[len], buff_size - len, "</MBusData>\n");

        serializeJson(doc, buff);
        return buff;
    }

    return NULL;
}

// char *mbus_data_variable_header_json(mbus_data_variable_header *header)
// {
//     DynamicJsonDocument doc(1024);
//     char temp[128];

//     static char buff[8192];
//     char str_encoded[768];
//     size_t len = 0;

//     if (header)
//     {
//         // len += snprintf(&buff[len], sizeof(buff) - len, "    <SlaveInformation>\n");

//         sprintf(temp, "%llX", mbus_data_bcd_decode_hex(header->id_bcd, 4));
//         doc["SlaveInformation"]["Id"] = temp;
//         // len += snprintf(&buff[len], sizeof(buff) - len, "        <Id>%llX</Id>\n", mbus_data_bcd_decode_hex(header->id_bcd, 4));

//         doc["SlaveInformation"]["Manufacturer"] = mbus_decode_manufacturer(header->manufacturer[0], header->manufacturer[1]);
//         // len += snprintf(&buff[len], sizeof(buff) - len, "        <Manufacturer>%s</Manufacturer>\n",
//         //                 mbus_decode_manufacturer(header->manufacturer[0], header->manufacturer[1]));

//         sprintf(temp, "%d", header->version);
//         doc["SlaveInformation"]["Version"] = temp;
//         // len += snprintf(&buff[len], sizeof(buff) - len, "        <Version>%d</Version>\n", header->version);

//         mbus_str_json_encode(str_encoded, mbus_data_product_name(header), sizeof(str_encoded));
//         doc["SlaveInformation"]["ProductName"] = str_encoded;
//         // len += snprintf(&buff[len], sizeof(buff) - len, "        <ProductName>%s</ProductName>\n", str_encoded);

//         mbus_str_json_encode(str_encoded, mbus_data_variable_medium_lookup(header->medium), sizeof(str_encoded));
//         doc["SlaveInformation"]["Medium"] = str_encoded;
//         // len += snprintf(&buff[len], sizeof(buff) - len, "        <Medium>%s</Medium>\n", str_encoded);

//         sprintf(temp, "%d", header->version);
//         doc["SlaveInformation"]["AccessNumber"] = temp;
//         len += snprintf(&buff[len], sizeof(buff) - len, "        <AccessNumber>%d</AccessNumber>\n", header->access_no);

//         sprintf(temp, "%.2X", header->status);
//         doc["SlaveInformation"]["Status"] = temp;
//         len += snprintf(&buff[len], sizeof(buff) - len, "        <Status>%.2X</Status>\n", header->status);

//         sprintf(temp, "%.2X%.2X", header->signature[1], header->signature[0]);
//         doc["SlaveInformation"]["Signature"] = temp;
//         // len += snprintf(&buff[len], sizeof(buff) - len, "        <Signature>%.2X%.2X</Signature>\n", header->signature[1], header->signature[0]);
//         // len += snprintf(&buff[len], sizeof(buff) - len, "    </SlaveInformation>\n\n");

//         serializeJson(doc, buff);
//         return buff;
//     }

//     return "";
// }
// char *mbus_data_variable_json_normalized(mbus_data_variable *data) {}
// int mbus_str_json_encode(char *dst, const char *src, size_t max_len) {}
// char *mbus_data_error_json(int error)
// {

//     DynamicJsonDocument doc(1024);
//     char buff[1024];

//     if (buff == NULL)
//         return NULL;

//     doc["MBusData"]["SlaveInformation"]["Error"] = mbus_data_error_lookup(error);

//     serializeJson(doc, buff);
//     return buff;
// }
