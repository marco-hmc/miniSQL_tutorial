/*
 * @Author: your name
 * @Date: 2022-01-27 15:47:46
 * @LastEditTime: 2022-01-28 14:34:21
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /latestDB/demo1/src/vr.c
 */

#include "common.h"
#include "complier.h"
#include "vm.h"

void * row_slot(Table *table, uint32_t row_num){
    uint32_t   page_num = row_num / ROWS_PER_PAGE;
    void      *page = table->pages[page_num];

    if (!page) {
        /* Allocate memory only when we try to access page */
        page = table->pages[page_num] = malloc(PAGE_SIZE);
    }

    uint32_t    row_offset = row_num % ROWS_PER_PAGE;
    uint32_t    byte_offset = row_offset * ROW_SIZE;
    return (char *) page + byte_offset;
}

ExecuteResult execute_insert(Statement *statement, Table *table){
    if (table->num_rows >= TABLE_MAX_ROWS) {
        return EXECUTE_TABLE_FULL;
    }

    Row *row_to_insert = &statement->row_to_insert;
    serialize_row(row_to_insert, row_slot(table, table->num_rows));
    table->num_rows += 1;

    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement *statement, Table *table){
    Row row;
    for (uint32_t i = 0; i < table->num_rows; i++) {
        deserialize_row(row_slot(table, i), &row);
        print_row(&row);
    }

    return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement(Statement *statement, Table *table){
     switch (statement->type) {
     case STATEMENT_INSERT:
        return execute_insert(statement, table);
     case STATEMENT_SELECT:
        return execute_select(statement, table);
     }

    return EXECUTE_UNKNOWN_STMT;
}