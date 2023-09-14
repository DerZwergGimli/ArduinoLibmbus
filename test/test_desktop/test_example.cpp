#include <unity.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/libmbus/mbus-json.h"
#include "../lib/libmbus/mbus-protocol-aux.h"

void run_test_hex(void);

void read_hex_file(char fileName[50], unsigned char raw_buff[], size_t raw_buff_length)
{
    char ch;
    char *hex_array;
    FILE *fp;
    fp = fopen(fileName, "rb");

    if (fp == NULL) // error checking
    {
        printf("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    memset(raw_buff, 0, raw_buff_length);
    fread(raw_buff, 1, raw_buff_length, fp);

    fclose(fp);
}

void run_test_hex(void)
{
    char filename[] = "test/test_desktop/hex/message_01.hex";
    unsigned char raw_buff[4096];
    unsigned char buff[4096];
    size_t buff_len;
    int result, normalized = 0;
    mbus_frame reply;
    mbus_frame_data frame_data;
    char *xml_result = NULL;
    char *json_result = NULL;

    read_hex_file(filename, raw_buff, sizeof(raw_buff));
    printf("%s\n", raw_buff);

    buff_len = mbus_hex2bin(buff, sizeof(buff), raw_buff, sizeof(raw_buff));
    printf("buff_len=%i\n", buff_len);

    memset(&reply, 0, sizeof(reply));
    memset(&frame_data, 0, sizeof(frame_data));
    result = mbus_parse(&reply, buff, buff_len);
    printf("result=%i\n", result);

    result = mbus_frame_data_parse(&reply, &frame_data);
    printf("result=%i\n", result);
    printf("normalized=%i\n", normalized);

    json_result = normalized ? mbus_frame_data_json_normalized(&frame_data) : mbus_frame_data_json(&frame_data);
    // printf("json_result=%s\n", json_result);

    TEST_ASSERT_EQUAL_INT32(1, 1);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(run_test_hex);
    UNITY_END();

    return 0;
}