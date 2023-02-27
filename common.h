#pragma once
#define MAX_STATEMENTS 0xFFFF
#define MAX_DATA  0xFFFF
#define MAX_EXPR 0xFFFF
#include "y.tab.h"


struct expr { 
    int lhs_frame_index;  /* this will index of data frame */
    int op; 
    int rhs_frame_index;
};

struct data { 
    int type; 
    union { 
        int int_value; 
        char*  string_value;
        char* variable_name; 
        int expr_frame_index; 
    };
};

struct statement { 
    int type ; 
    char* name; 
    int frame_index;
    int next_stmt , prev_stmt; 
    
    /* statements inside the if block or for block */
    int statement_index; 
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