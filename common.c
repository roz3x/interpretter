
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "y.tab.h"
#include "trie.h"

struct statement stmts[MAX_STATEMENTS];
int stmt_idx = 0; 
struct data dataframe[MAX_DATA];
int data_idx = 0;
struct expr exprframe[MAX_EXPR];
int expr_idx = 0 ; 

int makeUniqeStatement(int type , char* name , 
int frame_index ) { 
    stmts[stmt_idx] = (struct statement){
        .type = type , 
        .name = name , 
        .frame_index = frame_index,  
        .next_stmt = -1 , 
        .prev_stmt = -1,
    };
    return stmt_idx++;
}

 void printStatementChain(int start ) {
    while(start != -1 && start < stmt_idx  && start >= 0) { 
        printf("type %d \n" , stmts[start].type);
        start = stmts[start].next_stmt;
    }
 }

void appendStatements(int first , int second) { 
    stmts[first].next_stmt = second; 
    stmts[second].prev_stmt = first; 
}

int evaluateExprFrame(int idx) { 
    int lhs = evalueateDataFrame(exprframe[idx].lhs_frame_index); 
    int rhs = evalueateDataFrame(exprframe[idx].rhs_frame_index);
    switch (exprframe[idx].op) { 
        case PLUS: 
            return lhs + rhs; 
        case MINUS: 
            return lhs - rhs; 
        case DIV: 
            return lhs/rhs; 
        case MUL : 
            return lhs * rhs; 
        
    };
    return 0 ; 
}

int evalueateDataFrame(int idx ) { 
    switch(dataframe[idx].type) { 
        case INT: 
            return dataframe[idx].int_value; 
        case VARIABLE: 
            return get(dataframe[idx].variable_name); /* have to use trie here */
        case EXPR: 
            return evaluateExprFrame(dataframe[idx].expr_frame_index);
    }
}

void exec_statements() { 
    for (int  i = 0 ; i  < stmt_idx; i++) {
        if (stmts[i].type == 1 || stmts[i].type == 2 ) { 
            int value = evalueateDataFrame(stmts[i].frame_index);
            insert(stmts[i].name , (void*)value); 
        }
    }
    printf("a : %d\nb : %d\nc : %d\n", get("a"), get("b"), get("c"));
}

int createIntegerDataFrame(int a) { 
    dataframe[data_idx] = (struct data){ 
        .type = INT, 
        .int_value  = a ,
    };
    return data_idx++;
}
int createStringDataFrame(char* a ) { 
    dataframe[data_idx] = (struct data){
        .type = STRING , 
        .string_value = a,
    };
    return data_idx++; 
}

/* for evaluation of this we will need 
trie values */
int createVariableFrame(char* a) { 
    dataframe[data_idx] = (struct data){
        .type = VARIABLE , 
        .variable_name = a,
    };
    return data_idx++;
}

int makeExpr(int lhs_frame_index, int op , int rhs_frame_index) { 
    exprframe[expr_idx] = (struct expr) { 
        .lhs_frame_index  =lhs_frame_index , 
        .op = op, 
        .rhs_frame_index = rhs_frame_index, 
    };
    return expr_idx++;
}

int makeDataFrameFromExpr(int expr_frame_index){  /* index of start of expr is given */
    dataframe[data_idx] = (struct data){ 
        .type = EXPR , 
        .expr_frame_index = expr_frame_index,
    };
    return data_idx++;
}