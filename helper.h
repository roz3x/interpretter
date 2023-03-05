
#include <stdio.h>
#include <string.h> 

/* temprary translation buffer used by printf */
char buff[0xFFFF];

void translate_input_string_to_c_string(char *a) { 
    
    const int a_size = strlen(a); 
    int effective_a_size = 0;

    for (int i = 0 ; i < a_size; i++) { 
        if (a[i] == '\\') { 
            switch(a[i+1]){ 
                case 'n': 
                    buff[effective_a_size] = '\n';
                    break;
                case 'r':
                    buff[effective_a_size] = '\r';
                    break; 
                case '\\':
                    buff[effective_a_size] = '\\';
                default: 
                    fprintf(stderr , "unrecognized escape sequence\n");
            }
            i++;
            effective_a_size++;
            continue; 
        } else { 
            buff[effective_a_size++] = a[i];
        }
    }
    buff[effective_a_size] = '\0';
}