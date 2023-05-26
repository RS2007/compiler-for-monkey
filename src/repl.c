#include "evaluator.h"
#include "lexer.h"
#include "object.h"
#include "parser.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    while (1) {
        char user_input[STRING_MAX_SIZE];
        user_input[0] = '\0';
        printf(">>");
        fgets(user_input, STRING_MAX_SIZE, stdin);
        if (strcmp(user_input, "quit\n") == 0) {
            return 0;
        }
        lexer_t* lexer = new_lexer(user_input, strlen(user_input));
        parser_t* parser = new_parser(lexer);
        program_t* program_node = parse_program(parser);
        object_t* object = eval((node_t*)program_node);
        printf("%s\n", object->inspect((void*)object));
    }
}
