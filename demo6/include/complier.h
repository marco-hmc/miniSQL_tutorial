/*
 * @Author: your name
 * @Date: 2022-01-27 15:48:28
 * @LastEditTime: 2022-01-28 17:26:14
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /latestDB/demo1/header/complier.h
 */
#pragma once
#include "data.h"
#include "common.h"

typedef enum{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED
} MetaCommandResult;

typedef enum{
    PREPARE_SUCCESS,
    PREPARE_NEGATIVE_ID,
    PREPARE_STRING_TOO_LONG,
    PREPARE_SYNTAX_ERROR,
    PREPARE_UNRECOGNIZED
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef struct{
    StatementType type;
    Row row_to_insert;
} Statement;

MetaCommandResult do_meta_command(InputBuffer *input_buffer, Table *table);

PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement);