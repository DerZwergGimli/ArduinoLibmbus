#include <unity.h>
#include <example.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/libmbus/mbus-protocol-aux.h"
#include "../lib/libmbus/mbus-json.h"

void test_add(void)
{
    int expected = 1 + 3;
    int actual = add(1, 3);
    TEST_ASSERT_EQUAL_INT32(expected, actual);
}

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

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    char filename[] = "test/test_desktop/hex/message_01.hex";
    unsigned char raw_buff[4096];
    unsigned char buff[4096];
    size_t buff_len;
    int result, normalized = 0;
    mbus_frame reply;
    mbus_frame_data frame_data;
    char *xml_result = NULL;

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

    xml_result = normalized ? mbus_frame_data_xml_normalized(&frame_data) : mbus_frame_data_xml(&frame_data);

    printf("xml_result=%s\n", xml_result);

    RUN_TEST(test_add);
    UNITY_END();

    return 0;
}