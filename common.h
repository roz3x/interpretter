#pragma once
#define MAX_STATEMENTS 0xFFFF
#define MAX_DATA  0xFFFF
#define MAX_EXPR 0xFFFF
#define MAX_FUNCTION_CALL 0xFFFF
#define MAX_ARG_LIST 0xFFFF
#define MAX_FLATTEN_ARG_LIST_SIZE 0xFF
#include "y.tab.h"


struct expr { 
    int lhs_frame_index;  /* this will index of data frame */
    int op; 
    int rhs_frame_index;
};

struct data { 
    int type; 
    union { 
        int   int_value; 
        char* string_value;
        char* variable_name; 
        int   expr_frame_index; 
    };
};

struct function_call { 
    char* name; 
    int arg_list_idx; 
    int flatten_arg_list[MAX_FLATTEN_ARG_LIST_SIZE];
    int flatten_arg_list_len; 
};

struct arg_list { 
    int start_dataframe_index;
    int next_arg; 
};

struct statement { 
    int type ; 
    char* name; 
    int frame_index;
    int next_stmt , prev_stmt; 
    
    /* statements inside the if block or for block */
    int statement_index; 
    int arg_list_idx;


    /* for loop arguments */
    int first_for_part , second_for_part, third_for_part; 
    int middle_statements; 

    /* else statement */
    int else_statement_index; 


    /* function calling flatten stuff */
    int flatten_arg_list[MAX_FLATTEN_ARG_LIST_SIZE];
    int flatten_arg_list_len; 
    
};

int makeUniqeStatement( int , char* , int);

void printStatementChain(int  );
void appendStatements(int , int);
void exec_statements();

int createIntegerDataFrame(int ); 
int createStringDataFrame(char* );
int createVariableFrame(char* );

int makeExpr(int lhs_frame_index, int op , int rhs_frame_index); 
int makeDataFrameFromExpr(int);
int makeIfStatement(int , int); 
void registerFunctionName(char* name , int start_statement_index); 
void callFunction(char* name); 