int main() { 
    printf("this ", "is ", "how ", "it ", "works\n");
    for (i= 0; i < 10 ; i++) { 
        for (j = 0 ; j < 10 ; j++) { 
            int t1 = 5 - i ; 
            int t2 = 5 - j ; 
            if (((t1 * t1) * (t2 * t2 )) < 10 ) { 
                printf(".");
            } else { 
                printf(" ");
            }
        }
        printf("\n");
    }
}