/*
 * @Author: your name
 * @Date: 2022-01-28 16:48:21
 * @LastEditTime: 2022-02-05 21:24:08
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /demo4/src/data.c
 */
#include "common.h"
#include "data.h"
#include "bplustree.h"
#include "vm.h"

void serialize_row(Row *source, void *destination){
    char *dest = (char *) destination;
    memcpy(dest + ID_OFFSET, &(source->id), ID_SIZE);
    strncpy(dest + USERNAME_OFFSET, source->username, USERNAME_SIZE);
    strncpy(dest + EMAIL_OFFSET, source->email, EMAIL_SIZE);    
}

void deserialize_row(void *source, Row *destination){
    char *src = (char *) source;
    memcpy(&(destination->id), src + ID_OFFSET, ID_SIZE);
    memcpy(&(destination->username), src + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(destination->email), src + EMAIL_OFFSET, EMAIL_SIZE);
}

void print_row(Row *row){
    printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}

Table *db_open(const char *filename){
    Pager *pager = pager_open(filename);
    Table *table = (Table *)malloc(sizeof(Table));

    table->pager = pager;
    table->root_page_num = 0;

    if (pager->num_pages == 0) {
        /* New database file. Initialize page 0 as leaf node. */
        void *root_node = get_page(pager, 0);
        initialize_leaf_node(root_node);
        set_node_root(root_node, true);
    }
    return table;
}

Pager *pager_open(const char *filename){
    int        fd;
    off_t      file_length;
    uint32_t   i;
    Pager     *pager;

    fd = open(filename,
              O_RDWR |      /* Read/Write mode */
              O_CREAT,      /* Create file if it does not exist */
              S_IWUSR |     /* User write permission */
              S_IRUSR);     /* User Read permission */

    if (fd == -1) {
        printf("Unable to open file\n");
        exit(EXIT_FAILURE);
    }

    file_length = lseek(fd, 0, SEEK_END);

    pager = malloc(sizeof(Pager));
    pager->file_descriptor = fd;
    pager->file_length = file_length;
    pager->num_pages = (file_length / PAGE_SIZE);

    if (file_length % PAGE_SIZE != 0) {
        printf("Db file is not a whole number of pages. Corrupt file.\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < TABLE_MAX_PAGES; i++) {
        pager->pages[i] = NULL;
    }

    return pager;
}

void db_close(Table *table){
    Pager     *pager = table->pager;
    uint32_t   num_additional_rows;
    uint32_t   i;
    int        result;

    for (i = 0; i < pager->num_pages; i++) {
        if (pager->pages[i] == NULL) {
            continue;
        }

        pager_flush(pager, i);
        free(pager->pages[i]);
        pager->pages[i] = NULL;
    }

    result = close(pager->file_descriptor);
    if (result == -1) {
        printf("Error closing db file.\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < TABLE_MAX_PAGES; i++) {
        void *page = pager->pages[i];
        if (page) {
            free(page);
            pager->pages[i] = NULL;
        }
    }
    free(pager);
    free(table);
}

void pager_flush(Pager* pager, uint32_t page_num){
    off_t      offset;
    ssize_t  bytes_written;
    if (pager->pages[page_num] == NULL) {
        printf("Tried to flush null page\n");
        exit(EXIT_FAILURE);
    }

    offset = lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);

    if (offset == -1) {
        printf("Error seeking: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    bytes_written =
        write(pager->file_descriptor, pager->pages[page_num], PAGE_SIZE);

    if (bytes_written == -1) {
        printf("Error writing: %d\n", errno);
        exit(EXIT_FAILURE);
    }
}

Cursor *table_start(Table *table){
    uint32_t  num_cells;
    void     *root_node;
    Cursor   *cursor = (Cursor *) malloc(sizeof(Cursor));

    cursor->table = table;
    cursor->page_num = table->root_page_num;
    cursor->cell_num = 0;

    root_node = get_page(table->pager, table->root_page_num);
    num_cells = *leaf_node_num_cells(root_node);
    cursor->end_of_table = (num_cells == 0);

    return cursor;
}

void cursor_advance(Cursor *cursor){
    uint32_t page_num = cursor->page_num;
    void *node = get_page(cursor->table->pager, page_num);

    cursor->cell_num += 1;
    if (cursor->cell_num >= (*leaf_node_num_cells(node))) {
         cursor->end_of_table = true;
    }
}

/*
 * Return the position of the given key.
 * If the key is not present, return the position
 * where it should be inserted.
 */
Cursor *table_find(Table *table, uint32_t key){
    uint32_t  root_page_num = table->root_page_num;
    void     *root_node = get_page(table->pager, root_page_num);

    if (get_node_type(root_node) == NODE_LEAF) {
        return leaf_node_find(table, root_page_num, key);
    } else {
        printf("Need to implement searching an internal node\n");
        exit(EXIT_FAILURE);
    }
}

uint32_t get_unused_page_num(Pager *pager){
    return pager->num_pages; 
}