/*
 * @Author: your name
 * @Date: 2022-01-10 13:15:05
 * @LastEditTime: 2022-01-28 17:21:57
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /myDB/tutorial/2.c
 */
/*
 * @Author: your name
 * @Date: 2022-01-10 12:40:10
 * @LastEditTime: 2022-01-10 13:05:07
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /myDB/tutorial/1.c
 */
#include "common.h"
#include "complier.h"
#include "vm.h"
#include "data.h" 

int main(int argc, char **argv){
    char           *filename;
    Table          *table;
    InputBuffer    *input_buffer;

    if (argc < 2) {
        printf("Must supply a database filename.\n");
        exit(EXIT_FAILURE);
    }

    filename = argv[1];
    table = db_open(filename);
    input_buffer = new_input_buffer();

    while(true){
        print_prompt();
        read_input(input_buffer);
        
        if (input_buffer->buffer[0] == '.')
        {
            switch (do_meta_command(input_buffer, table)){
                case META_COMMAND_SUCCESS:
                    continue;
                case META_COMMAND_UNRECOGNIZED:
                    printf("Unrecognized command '%s'\n", input_buffer->buffer);
                    continue;
            }
        }
        Statement statement;
        switch(prepare_statement(input_buffer, &statement)){
            case PREPARE_SUCCESS:
                break;
            case PREPARE_NEGATIVE_ID:
                printf("ID must be positive.\n");
                continue;
            case PREPARE_STRING_TOO_LONG:
                printf("String is too long.\n");
                continue;
            case PREPARE_SYNTAX_ERROR:
                printf("Syntax error. Could not parse statement.\n");
                continue;
            case PREPARE_UNRECOGNIZED:
                printf("Unrecognized statement '%s'\n", input_buffer->buffer);
                continue;
        }
        
        switch (execute_statement(&statement, table)) {
            case EXECUTE_SUCCESS:
                printf("Executed.\n");
                break;
            case EXECUTE_TABLE_FULL:
                printf("Error: Table full.\n");
                break;
            case EXECUTE_UNKNOWN_STMT:
                printf("Error: Unknown statement.\n");
                break;
        }
    }
    return 0;
}