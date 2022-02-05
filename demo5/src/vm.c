/*
 * @Author: your name
 * @Date: 2022-01-27 15:47:46
 * @LastEditTime: 2022-01-28 18:20:34
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /latestDB/demo1/src/vr.c
 */

#include "data.h"
#include "common.h"
#include "complier.h"
#include "vm.h"


void *get_page(Pager *pager, uint32_t page_num){
    if (page_num > TABLE_MAX_PAGES) {
        printf("Tried to fetch page number out of bounds. %d > %d\n",
               page_num, TABLE_MAX_PAGES);
        exit(EXIT_FAILURE);
    }

    if (pager->pages[page_num] == NULL) {
        /* Cache miss. Allocate memory and load from file. */
        void      *page = malloc(PAGE_SIZE);
        uint32_t   num_pages = pager->file_length / PAGE_SIZE;

        /* We might save a partial page at the end of the file. */
        if (pager->file_length % PAGE_SIZE) {
            num_pages += 1;
        }

        if (page_num < num_pages) {
            ssize_t bytes_read;
            lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
            bytes_read = read(pager->file_descriptor, page, PAGE_SIZE);
            if (bytes_read == -1) {
                printf("Error reading file: %d\n", errno);
                exit(EXIT_FAILURE);
            }
        }

        pager->pages[page_num] = page;
    }

    return pager->pages[page_num];
}

void *cursor_value(Cursor *cursor) {
    uint32_t   row_num = cursor->row_num;
    uint32_t   page_num = row_num / ROWS_PER_PAGE;
    void      *page = get_page(cursor->table->pager, page_num);
    uint32_t    row_offset = row_num % ROWS_PER_PAGE;
    uint32_t    byte_offset = row_offset * ROW_SIZE;
    return (char *) page + byte_offset;
}

ExecuteResult execute_insert(Statement *statement, Table *table){
    if (table->num_rows >= TABLE_MAX_ROWS) {
        return EXECUTE_TABLE_FULL;
    }

    Row *row_to_insert = &statement->row_to_insert;
    Cursor *cursor = table_end(table);

    serialize_row(row_to_insert, cursor_value(cursor));
    table->num_rows += 1;

    free(cursor);

    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement *statement, Table *table) {
    Cursor *cursor = table_start(table);
    Row row;
    while (!(cursor->end_of_table)) {
        deserialize_row(cursor_value(cursor), &row);
        print_row(&row);
        cursor_advance(cursor);
    }

    free(cursor);
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