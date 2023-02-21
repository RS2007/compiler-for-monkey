//
// Created by rohith on 2/21/23.
//

#include "lexer.h"
#include<stdlib.h>

Lexer* new_lexer(char* input){
    Lexer* new_lexer_instance = (Lexer*)malloc(sizeof(Lexer));
    new_lexer_instance->input_string = input;
    return new_lexer_instance;
}

void readChar(Lexer* l){
    if(l->read_position >= strlen(l->input_string)){
        l->current_char = 0;
    }else{
        l->current_char = l->input_string[l->read_position];
    }
    l->position = l->read_position;
    l->read_position += 1;
}