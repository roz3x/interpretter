
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "common.h"
#include "helper.h"
#include "y.tab.h"
#include "trie.h"


struct statement stmts[MAX_STATEMENTS];
int stmt_idx = 0; 
struct data dataframe[MAX_DATA];
int data_idx = 0;
struct expr exprframe[MAX_EXPR];
int expr_idx = 0 ; 
struct arg_list arglistframe[MAX_ARG_LIST];
int arglist_idx = 0;
struct function_call function_call_frame[MAX_FUNCTION_CALL];
int function_call_index = 0 ; 


int makeForStatement(int firstForPart , int secondForPart , int thirdForPart , int middleStatements) { 
    stmts[stmt_idx] = (struct statement){ 
        .type = FOR_STATEMENT,
        .first_for_part = firstForPart , 
        .second_for_part = secondForPart, 
        .third_for_part = thirdForPart,
        .middle_statements = middleStatements,
    };
    return stmt_idx++;
}
int makeFunctionCallStatement(int function_call_idx) { 
    stmts[stmt_idx] = (struct statement) {
        .type = FUNCTION_CALL,
        .name = function_call_frame[function_call_idx].name , 
        .arg_list_idx = function_call_frame[function_call_idx].arg_list_idx,
    };
    return stmt_idx++;
} 
int makeFunctionCall(char* name , int arg_list_index) { 
    function_call_frame[function_call_index] = (struct function_call) { 
        .name  = name,
        .arg_list_idx = arg_list_index,
    };
    return function_call_index++;
}

int makeArgList(int start_dataframe_index, int next_arg) { 
    arglistframe[arglist_idx] = (struct arg_list) { 
        .start_dataframe_index  = start_dataframe_index,
        .next_arg = next_arg,
    };
    return arglist_idx++;
}


int makeIfStatement( int frame_index , int statement_index ) { 
    /* for if statements the the value of the check will be in frameIndex   */
    stmts[stmt_idx] = (struct statement) { 
        .type = IF , 
        .frame_index = frame_index ,
        .statement_index = statement_index,
    };
    return stmt_idx++; 
}

int makeIfElseStatement(int frame_index , int statement_index , int else_statement_index) { 
    stmts[stmt_idx] = (struct statement) { 
        .type = IF_ELSE , 
        .frame_index = frame_index, 
        .statement_index = statement_index , 
        .else_statement_index = else_statement_index , 
    }; 
    return stmt_idx++;
}

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
    struct data *variable;
    switch (exprframe[idx].op) { 
        case PLUS: 
            return lhs + rhs; 
        case MINUS: 
            return lhs - rhs; 
        case DIV: 
            return lhs/rhs; 
        case MUL : 
            return lhs * rhs; 
        case COMP: 
            return lhs == rhs; 
        case LT : 
            return lhs < rhs; 
        case GT : 
            return lhs > rhs; 
        case LTE : 
            return lhs <= rhs; 
        case GTE: 
            return lhs >= rhs; 
        case OR : 
            return lhs || rhs; 
        case AND: 
            return lhs && rhs; 
        case EQUALS: 
            variable = &dataframe[exprframe[idx].lhs_frame_index]; 
            assert(variable->type == VARIABLE); 
            insert(variable->variable_name , (void*)rhs); 
            return rhs; 
    };
    return 0 ; 
}

int evalueateDataFrame(int idx ) { 
    switch(dataframe[idx].type) { 
        case INT: 
            return dataframe[idx].int_value; 
        case VARIABLE: 
            return (int)get(dataframe[idx].variable_name); /* have to use trie here */
        case EXPR: 
            return evaluateExprFrame(dataframe[idx].expr_frame_index);
    }
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
        .string_value = a + 1,
    };
    a[strlen(a)-1] = '\0';
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

void registerFunctionName(char* name , int start_statement_index) { 
    insert(name , (void*)start_statement_index);
}


void exec_single_statement(int index) { 
    if (stmts[index].type == 1 || stmts[index].type == 2 ) { 
        int value = evalueateDataFrame(stmts[index].frame_index);
        insert(stmts[index].name , (void*)value); 
    }

    if (stmts[index].type == IF) { 
        int value = evalueateDataFrame(stmts[index].frame_index);
        if (value) { 
            exec_statements(stmts[index].statement_index);
        }
    }
    if (stmts[index].type == IF_ELSE) {
        // printf("index : %d type : %d\n",index, stmts[index].type); 
        int value = evalueateDataFrame(stmts[index].frame_index) ; 
        if (value ) { 
            exec_statements(stmts[index].statement_index);
        } else { 
            exec_statements(stmts[index].else_statement_index);
        }
    }
    if (stmts[index].type == FUNCTION_CALL) { 
        if (!strcmp(stmts[index].name, "printf")) { 
            /* todo for now we only print the value of variable. */
            /* simple implementation  */
            int start = stmts[index].arg_list_idx;
            while(start != -1) { 
                if (dataframe[arglistframe[start].start_dataframe_index].type == VARIABLE) { 
                    printf("%d" , (int)get(dataframe[arglistframe[start].start_dataframe_index].variable_name));
                }  else if (dataframe[arglistframe[start].start_dataframe_index].type == STRING) {
                    const char *temp_string = dataframe[arglistframe[start].start_dataframe_index].string_value;
                    translate_input_string_to_c_string(temp_string); 
                    printf(buff);
                } else { 
                    assert(0); 
                }
                start = arglistframe[start].next_arg;
            }
        }
    }
    if (stmts[index].type == FOR_STATEMENT) { 
        evaluateExprFrame(stmts[index].first_for_part); 
        while(evaluateExprFrame(stmts[index].second_for_part)) { 
            exec_statements(stmts[index].middle_statements);
            evaluateExprFrame(stmts[index].third_for_part); 
        }

    }
}
void exec_statements(int start) { 
    while(start != -1 && start < stmt_idx  && start >= 0) { 
        exec_single_statement(start); 
        start = stmts[start].next_stmt;
    }
}

void callFunction(char*name ) { 
    int start = (int) get(name);
    exec_statements(start); 
    // this was an old check , no need anymore
    // printf("a: %d\nb: %d\nc : %d\n", get("a"), get("b"), get("c"));  
}