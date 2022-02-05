/*
 * @Author: your name
 * @Date: 2022-01-27 15:47:56
 * @LastEditTime: 2022-01-28 17:34:54
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /latestDB/demo1/header/vm.h
 */
#pragma once
#include "data.h"
#include "common.h"
#include "complier.h"

enum ExecuteResult_t
{
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL,
    EXECUTE_UNKNOWN_STMT
};
typedef enum ExecuteResult_t ExecuteResult;

ExecuteResult execute_statement(Statement *statement, Table *table);

void *cursor_value(Cursor *cursor);

void *get_page(Pager *pager, uint32_t page_num);
