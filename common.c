
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

struct statement stmts[MAX_STATEMENTS];
int stmt_idx = 0; 

int makeUniqeStatement(int type , char* left_name , char* right_name , 
char* string_right_value , int int_right_value) { 
    printf("getting stmt of type : %d\n", type);
    stmts[stmt_idx] = (struct statement){
        .type = type , 
        .left_name = left_name, 
        .right_name = right_name, 
        .string_right_value = string_right_value , 
        .int_right_value = int_right_value, 
        .next_stmt = -1 , 
        .prev_stmt = -1,
    };
    return stmt_idx++;
}

 void printStatementChain(int start ) {
    printf("printing values \n");
    while(start != -1 && start < stmt_idx  && start >= 0) { 
        printf("type %d \n" , stmts[start].type);
        start = stmts[start].next_stmt;
    }
 }

void appendStatements(int first , int second) { 
    stmts[first].next_stmt = second; 
    stmts[second].prev_stmt = first; 
}

void exec_statements() { 
    for (int  i = 0 ; i  < stmt_idx; i++) { 
        // print
    }
}