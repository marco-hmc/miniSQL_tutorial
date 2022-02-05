/*
 * @Author: your name
 * @Date: 2022-01-27 15:47:46
 * @LastEditTime: 2022-02-05 21:12:03
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /latestDB/demo1/src/vr.c
 */

#include "data.h"
#include "common.h"
#include "complier.h"
#include "vm.h"
#include "bplustree.h"

void *get_page(Pager *pager, uint32_t page_num)
{
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

        if (page_num >= pager->num_pages) {
            pager->num_pages = page_num + 1;
        }
    }

    return pager->pages[page_num];
}

void *cursor_value(Cursor *cursor){
    uint32_t   page_num = cursor->page_num;
    void      *page = get_page(cursor->table->pager, page_num);

    return leaf_node_value(page, cursor->cell_num);
}

ExecuteResult execute_insert(Statement *statement, Table *table){
    Row *row_to_insert;
    Cursor *cursor;
    void *node = get_page(table->pager, table->root_page_num);
    uint32_t num_cells = (*leaf_node_num_cells(node));
    uint32_t key_to_insert;

    row_to_insert = &statement->row_to_insert;
    key_to_insert = row_to_insert->id;
    cursor = table_find(table, key_to_insert);

    if (cursor->cell_num < num_cells) {
        uint32_t key_at_index = *leaf_node_key(node, cursor->cell_num);
        if (key_at_index == key_to_insert) {
            return EXECUTE_DUPLICATE_KEY;
        }
    }
    leaf_node_insert(cursor, row_to_insert->id, row_to_insert);

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