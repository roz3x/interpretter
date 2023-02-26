#pragma once
#define MAX_STATEMENTS 0xFFFF


struct statement { 
    int type ; 
    char* left_name; 
    char* right_name; 
    char* string_right_value; 
    int   int_right_value;
    int next_stmt , prev_stmt; 
};

int makeUniqeStatement(
    int , char* , char*, char* , int 
);

void printStatementChain(int  );
void appendStatements(int , int);
void exec_statements();