/*
 * @Author: your name
 * @Date: 2022-01-28 18:36:54
 * @LastEditTime: 2022-02-05 22:15:48
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /demo6/include/bplustre.h
 */
#pragma once
#include "common.h"
#include "data.h"

typedef enum { NODE_INTERNAL, NODE_LEAF } NodeType;

uint32_t *leaf_node_num_cells(void *node);

void *leaf_node_cell(void *node, uint32_t cell_num);

uint32_t *leaf_node_key(void *node, uint32_t cell_num);

void *leaf_node_value(void *node, uint32_t cell_num);

void initialize_leaf_node(void *node);

void leaf_node_insert(Cursor *cursor, uint32_t key, Row *value);

void print_constants();


/*
 * Common Node Header Layout
 */
extern const uint32_t NODE_TYPE_SIZE;
extern const uint32_t NODE_TYPE_OFFSET;
extern const uint32_t IS_ROOT_SIZE;
extern const uint32_t IS_ROOT_OFFSET;
extern const uint32_t PARENT_POINTER_SIZE;
extern const uint32_t PARENT_POINTER_OFFSET;
extern const uint8_t COMMON_NODE_HEADER_SIZE;

/*
 * Leaf Node Header Layout
 */
extern const uint32_t LEAF_NODE_NUM_CELLS_SIZE;
extern const uint32_t LEAF_NODE_NUM_CELLS_OFFSET;
extern const uint32_t LEAF_NODE_NEXT_LEAF_SIZE;
extern const uint32_t LEAF_NODE_NEXT_LEAF_OFFSET;
extern const uint32_t LEAF_NODE_HEADER_SIZE;

/*
 * Leaf Node Body Layout
 */
extern const uint32_t LEAF_NODE_KEY_SIZE;
extern const uint32_t LEAF_NODE_KEY_OFFSET;
extern const uint32_t LEAF_NODE_VALUE_SIZE;
extern const uint32_t LEAF_NODE_VALUE_OFFSET;
extern const uint32_t LEAF_NODE_CELL_SIZE;
extern const uint32_t LEAF_NODE_SPACE_FOR_CELLS;
extern const uint32_t LEAF_NODE_MAX_CELLS;

Cursor *leaf_node_find(Table *table, uint32_t page_num, uint32_t key);

NodeType get_node_type(void *node);

void set_node_type(void *node, NodeType type);

void leaf_node_split_and_insert(Cursor *cursor, uint32_t key, Row *value);

extern const uint32_t LEAF_NODE_RIGHT_SPLIT_COUNT;
extern const uint32_t LEAF_NODE_LEFT_SPLIT_COUNT;

void create_new_root(Table *table, uint32_t right_child_page_num);

extern const uint32_t INTERNAL_NODE_NUM_KEYS_SIZE;
extern const uint32_t INTERNAL_NODE_NUM_KEYS_OFFSET;
extern const uint32_t INTERNAL_NODE_RIGHT_CHILD_SIZE;
extern const uint32_t INTERNAL_NODE_RIGHT_CHILD_OFFSET;
extern const uint32_t INTERNAL_NODE_HEADER_SIZE;

extern const uint32_t INTERNAL_NODE_KEY_SIZE;
extern const uint32_t INTERNAL_NODE_CHILD_SIZE;
extern const uint32_t INTERNAL_NODE_CELL_SIZE;
extern const uint32_t INTERNAL_NODE_MAX_CELLS;

uint32_t *internal_node_num_keys(void *node);

uint32_t *internal_node_right_child(void *node);

uint32_t *internal_node_cell(void *node, uint32_t cell_num);

uint32_t *internal_node_child(void *node, uint32_t child_num);

uint32_t *internal_node_key(void *node, uint32_t key_num);

uint32_t get_node_max_key(void *node);

bool is_node_root(void *node);

void set_node_root(void *node, bool is_root);

void initialize_internal_node(void *node);

void indent(uint32_t level);

void print_tree(Pager *pager, uint32_t page_num, uint32_t indentation_level);

Cursor *internal_node_find(Table *table, uint32_t page_num, uint32_t key);

uint32_t *leaf_node_next_leaf(void *node);

uint32_t *node_parent(void *node);

void update_internal_node_key(void *node, uint32_t old_key, uint32_t new_key);

void internal_node_insert(Table *table, uint32_t parent_page_num,
                          uint32_t child_page_num);