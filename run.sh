# standalone script that cleans , builds and runs the test 
# :) cause i am lz 
rm y.tab.c y.tab.h lex.yy.c
rm ./a.out
yacc -d y
lex l 
clang -g -O0  lex.yy.c y.tab.c trie.c  common.c #2>/dev/null
./a.out < in
