/*
 * @Author: your name
 * @Date: 2022-01-27 15:53:22
 * @LastEditTime: 2022-01-28 14:59:27
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /latestDB/demo1/header/common.h
 */
#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

typedef struct
{
    char *buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

InputBuffer *new_input_buffer();

void print_prompt();

void read_input(InputBuffer *input_buffer);

//----------------------------------------------------------------
#define COLUMN_USERNAME_SIZE    32
#define COLUMN_EMAIL_SIZE       255
struct Row_t
{
    uint32_t    id;
    char username[COLUMN_USERNAME_SIZE + 1];
    char email[COLUMN_EMAIL_SIZE + 1];
};
typedef struct Row_t Row;

#define size_of_attribute(Struct, Attribute) sizeof(((Struct *)0)->Attribute)

#define ID_SIZE          size_of_attribute(Row, id)
#define USERNAME_SIZE    size_of_attribute(Row, username)
#define EMAIL_SIZE       size_of_attribute(Row, email)
#define ROW_SIZE         (ID_SIZE + USERNAME_SIZE + EMAIL_SIZE)

#define ID_OFFSET        0
#define USERNAME_OFFSET  (ID_OFFSET + ID_SIZE)
#define EMAIL_OFFSET     (USERNAME_OFFSET + USERNAME_SIZE)

//----------------------------------------------------------------
void serialize_row(Row *source, void *destination);

void deserialize_row(void *source, Row *destination);

//----------------------------------------------------------------
#define PAGE_SIZE        4096
#define TABLE_MAX_PAGES  100
#define ROWS_PER_PAGE    (PAGE_SIZE / ROW_SIZE)
#define TABLE_MAX_ROWS   (ROWS_PER_PAGE * TABLE_MAX_PAGES)

struct Table_t {
    void       *pages[TABLE_MAX_PAGES];
    uint32_t    num_rows;
};
typedef struct Table_t Table;

Table *new_table();
void print_row(Row *row);
