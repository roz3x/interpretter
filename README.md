### Interpretter

Simple Interpretter for C , in C . 

features. 

- [x] expression evaluation
- [X] IO
- [X] IF statements
- [X] FOR loop
- [X] Functions
- [ ] Arrays



### implementing the runtime function calling 

for the local expression evaluation , i was using the 
global evaluation context , but with the addition of function
definations i have to make different `trie`'s for each one of them ? 
not sure how to implement that now. will continue the work later on. 


### Idea for function imeplementation 

0. have a common global `runtime` stack that transfer single value from the called function to callee. 
0. fatten the arg list into static c array. 