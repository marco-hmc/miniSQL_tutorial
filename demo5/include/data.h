/*
 * @Author: your name
 * @Date: 2022-01-28 16:48:16
 * @LastEditTime: 2022-01-28 18:33:27
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /demo4/include/data.h
 */
#pragma once
#include "common.h"

// ----------------------------------------------------------------
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

void print_row(Row *row);



//----------------------------------------------------------------
#define PAGE_SIZE        4096
#define TABLE_MAX_PAGES  100
#define ROWS_PER_PAGE    (PAGE_SIZE / ROW_SIZE)
#define TABLE_MAX_ROWS   (ROWS_PER_PAGE * TABLE_MAX_PAGES)

typedef struct Pager_t{
    int       file_descriptor;
    uint32_t  file_length;
    void     *pages[TABLE_MAX_PAGES];
} Pager;


struct Table_t {
    Pager      *pager;
    uint32_t    num_rows;
};
typedef struct Table_t Table;

Table *db_open(const char *filename);

Pager *pager_open(const char *filename);

void db_close(Table *table);

void pager_flush(Pager *pager, uint32_t page_num, uint32_t size);

typedef struct {
    Table    *table;
    uint32_t  row_num;
    bool      end_of_table; /* Indicates a position one past the last element */
} Cursor;

Cursor *table_start(Table *table);

Cursor *table_end(Table *table);

void cursor_advance(Cursor *cursor);