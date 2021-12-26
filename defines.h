#pragma once

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE    32

#define size_of_attribute(Struct, Attribute)sizeof(((Struct*)0)->Attribute)

typedef struct {
	uint32_t id;
	char username[COLUMN_USERNAME_SIZE];
	char email[COLUMN_EMAIL_SIZE];
}Row;

const uint32_t ID_SIZE         = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE   = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE      = size_of_attribute(Row, email);
const uint32_t ID_OFFSET       = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET    = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE        = ID_SIZE   + USERNAME_SIZE + EMAIL_SIZE; 



const uint32_t PAGE_SIZE = 4096; //4 kilobytes
#define TABLE_MAX_PAGES    100
const uint32_t ROWS_PER_PAGE   = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS  = TABLE_MAX_PAGES * ROWS_PER_PAGE;


typedef struct{
    uint32_t num_rows;
    void* pages[TABLE_MAX_PAGES];
}Table;
typedef enum{ EXECUTE_SUCCESS, EXECUTE_TABLE_FULL} ExecuteResult;

//Gives the void pointer in which to read or write to
void* row_slot(Table* table, uint32_t row_num);


//Constructor
Table* new_table();

//Destructor 
void free_table(Table *table);


typedef struct{
    char* buffer;
    size_t buffer_length;
    size_t input_length;

}InputBuffer;

/*Input buffer constructor */
InputBuffer * new_input_buffer();

void print_prompt() {printf("db >");}

typedef intptr_t  ssize_t;

//Cause we don't have this in windows
ssize_t
getdelim(char** buf, size_t* bufsiz, int delimiter, FILE* fp)
{
	char* ptr, * eptr;


	if (*buf == NULL || *bufsiz == 0) {
		*bufsiz = BUFSIZ;
		if ((*buf = (char *) malloc(*bufsiz)) == NULL)
			return -1;
	}

	for (ptr = *buf, eptr = *buf + *bufsiz;;) {
		int c = fgetc(fp);
		if (c == -1) {
			if (feof(fp)) {
				ssize_t diff = (ssize_t)(ptr - *buf);
				if (diff != 0) {
					*ptr = '\0';
					return diff;
				}
			}
			return -1;
		}
		*ptr++ = c;
		if (c == delimiter) {
			*ptr = '\0';
			return ptr - *buf;
		}
		if (ptr + 2 >= eptr) {
			char* nbuf;
			size_t nbufsiz = *bufsiz * 2;
			ssize_t d = ptr - *buf;
			if ((nbuf = (char *)realloc(*buf, nbufsiz)) == NULL)
				return -1;
			*buf = nbuf;
			*bufsiz = nbufsiz;
			eptr = nbuf + nbufsiz;
			ptr = nbuf + d;
		}
	}
}

ssize_t
getline(char** buf, size_t* bufsiz, FILE* fp)
{
	return getdelim(buf, bufsiz, '\n', fp);
}

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
    PREPARE_UNRECOGNIZED_STATEMENT,
	PREPARE_SYNTAX_ERROR
}PrepareResult;

/* 
 * do_meta_command is just a wrapper for existing functionality that leaves room for more commands
 */

MetaCommandResult do_meta_command(InputBuffer * input_buffer);

typedef enum{STATEMENT_INSERT, STATEMENT_SELECT} StatementType;

/*
 * Statement struct will have statement
 * Will contain more data
 */
 
typedef struct{
    StatementType type;
    Row row_to_insert;//Only used by insert statement
}Statement;

//Execute insert statement
ExecuteResult execute_insert(Statement* statement, Table* table);
ExecuteResult execute_select(Statement*statenemtn, Table *table);


/* 
 * our sql compiler, now only supprts two words 
 */
PrepareResult prepare_statement(InputBuffer *input_buffer, Statement* statement);

/*
 * execute the statement, i mean the statement struct
 */

ExecuteResult execute_statement(Statement* statement,Table * table);


/*
 * code to convert to and from the compact representation
 * memcpy all the elements from source to destination
 * SEARIALIZATION (Big word)
 */


void serialize_row(Row* source, void* destination);


void deserialize_row(void* source, Row* destination);


/*
 * A table structure that points to the pages of rows and keeps track of how many rows there are
 */

void print_row(Row* row);