/*
 * @Author: your name
 * @Date: 2022-01-27 15:47:46
 * @LastEditTime: 2022-01-28 14:24:43
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /latestDB/demo1/src/vr.c
 */

#include "common.h"
#include "complier.h"
#include "vm.h"

void execute_statement(Statement *statement){
    switch(statement->type){
        case STATEMENT_INSERT:
            printf("Insetr here\n");
            break;
        case STATEMENT_SELECT:
            printf("Select here\n");
            break;
    }
}