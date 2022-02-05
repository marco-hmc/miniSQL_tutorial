/*
 * @Author: your name
 * @Date: 2022-01-10 13:15:05
 * @LastEditTime: 2022-01-28 11:40:26
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

int main(int argc, char **argv){
    InputBuffer *input_buffer = new_input_buffer();
    while(true){
        print_prompt();
        read_input(input_buffer);
        
        if (input_buffer->buffer[0] == '.')
        {
            switch(do_meta_command(input_buffer)){
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
            case PREPARE_UNRECOGNIZED:
                printf("Unrecognized statement '%s'\n", input_buffer->buffer);
                continue;
        }
        execute_statement(&statement);
        printf("Executed. \n");
    }
    return 0;
}