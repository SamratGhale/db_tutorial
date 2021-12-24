#pragma once

#include "stdio.h"
#include "stdlib.h"

typedef struct{
    char* buffer;
    size_t buffer_length;
    size_t input_length;

}InputBuffer;

/*Input buffer constructor */
InputBuffer * new_input_buffer();

void print_prompt() {printf("db >");}


//ssize_t getline(char **lineptr, size_t *n, FILE *stream);
/*To read a line of input from filestream

lineptr: A pointer to the variable we use to point to the buffer containing the read line.

n: a pointer to the variable we use to save the size of allocated buffer

strea: the input streram to read from file. We'll be reading from standard input

return : the number of bytes read, which may be less than the size of the buffer

*/


/*
Get input from stdin and put inside the buffer 
Adjusts the input_length of the buffer
*/
void read_input(InputBuffer* input_buffer);

/*
Closes the input buffer by freeing all the allocated memoty
*/
void close_input_buffer(InputBuffer* input_buffer);


/*
    command result enums
*/

typedef enum{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
}MetaCommandResult;

typedef enum{
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT 
}PrepareResult;


