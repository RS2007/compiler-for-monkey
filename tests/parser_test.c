#include "../src/parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATEMENTS_NUMBER 1

bool test_let_statement(statement_t* ast_statement, char* name)
{
    char* token_literal = ast_statement->token_literal((void*)ast_statement);
    if (strcmp(token_literal, "LET") != 0) {
        fprintf(stderr, "Expected: let statement, Got: %s", token_literal);
        return false;
    }
    let_statement_t* let_statement = (let_statement_t*)ast_statement;
    if (strcmp(name, let_statement->iden_name) != 0) {
        fprintf(stderr, "Expected: name of literal: %s, got %s", name,
            let_statement->iden_name);
        return false;
    }
    return true;
}

int test_let_statements(void)
{
    char* input = "let x = 5;"
                  "let y  = 10;"
                  "let foobar = 838383;";
    lexer_t* lexer = new_lexer(input, strlen(input));
    parser_t* parser = new_parser(lexer);
    program_t* program_node = parse_program(parser);
    if (parser->errors_size != 0) {
        int i;
        for (i = 0; i < parser->errors_size; ++i) {
            fprintf(stderr, "%s\n", parser->errors[i]);
        }
        exit(-1);
    }
    if (program_node->statements_size != STATEMENTS_NUMBER) {
        fprintf(stderr, "Error: expected %d statements, got %d statements",
            STATEMENTS_NUMBER, program_node->statements_size);
        return 69;
    }
    char* expectedIdentifiers[] = { "x", "y", "foobar" };
    int i;
    for (i = 0; i < STATEMENTS_NUMBER; ++i) {
        statement_t* stmt = program_node->statements[i];
        if (!test_let_statement(stmt, expectedIdentifiers[i])) {
            // fprintf(stderr,"Expected: %s,got:
            // %s",expectedIdentifiers[i],((let_statement_t*)stmt)->iden_name);
            fprintf(stderr, "Hello this is an error");
        }
    }
    fprintf(stdout, "All test cases passed ✅");
    return 0;
}

int test_ret_statements(void)
{
    char* input = "return 5;"
                  "return 10;"
                  "return 993322;";
    lexer_t* lexer = new_lexer(input, strlen(input));
    parser_t* parser = new_parser(lexer);
    program_t* program_node = parse_program(parser);
    if (parser->errors_size != 0) {
        int i;
        for (i = 0; i < parser->errors_size; ++i) {
            fprintf(stderr, "%s\n", parser->errors[i]);
        }
        exit(-1);
    }
    if (program_node->statements_size != STATEMENTS_NUMBER) {
        fprintf(stderr, "Error: expected %d statements, got %d statements",
            STATEMENTS_NUMBER, program_node->statements_size);
        return 69;
    }
    int i;
    for (i = 0; i < STATEMENTS_NUMBER; ++i) {
        statement_t* stmt = program_node->statements[i];
        if (strcmp(stmt->token_literal((void*)stmt), "RETURN") != 0) {
            fprintf(stderr, "Token literal not RETURN");
            return 69;
        }
    }
    fprintf(stdout, "All test cases passed ✅");
    return 0;
}

int test_string_method_on_statements(void)
{
    char* input = "return 5;"
                  "let a = 5;"
                  "return 993322;";
    lexer_t* lexer = new_lexer(input, strlen(input));
    parser_t* parser = new_parser(lexer);
    program_t* program_node = parse_program(parser);
    printf("%s", program_node->string((void*)program_node));
    return 0;
}

int test_parsing_identifiers(void)
{
    char* input = "foobar;";
    lexer_t* lexer = new_lexer(input, strlen(input));
    parser_t* parser = new_parser(lexer);
    program_t* program_node = parse_program(parser);
    if (program_node->statements_size != 1) {
        fprintf(stderr, "Expected %d statements , got %d statements", 1,
            program_node->statements_size);
        exit(-1);
    }

    if (strcmp(program_node->statements[0]->iden_name, "foobar") != 0) {
        fprintf(stderr, "Expected identifier name is %s, got %s", "foobar",
            program_node->statements[0]->iden_name);
        exit(-1);
    }

    if (strcmp(program_node->statements[0]->token_literal(
                   (void*)program_node->statements[0]),
            "foobar")
        != 0) {
        fprintf(stderr, "Expected identifier name is %s, got %s", "foobar",
            program_node->statements[0]->token_literal(
                (void*)program_node->statements[0]));
        exit(-1);
    }
    printf("All test cases passed ✅");
    return 0;
}

int test_parsing_integer_literal(void)
{
    char* input = "5;";
    lexer_t* lexer = new_lexer(input, strlen(input));
    parser_t* parser = new_parser(lexer);
    program_t* program_node = parse_program(parser);
    if (program_node->statements_size != 1) {
        fprintf(stderr, "Expected %d statements , got %d statements", 1,
            program_node->statements_size);
        exit(-1);
    }
    if (strcmp(program_node->statements[0]->token_literal(
                   (void*)program_node->statements[0]),
            "5")
        != 0) {
        fprintf(stderr, "Expected identifier name is %s, got %s", "5",
            program_node->statements[0]->token_literal(
                (void*)program_node->statements[0]));
        exit(-1);
    }
    if (program_node->statements[0]->expression->int_value != 5) {
        fprintf(stderr, "Expected integer value is %d got %d", 5,
            program_node->statements[0]->expression->int_value);
        exit(-1);
    }

    printf("All test cases passed ✅");
    return 0;
}

int test_parsing_prefix_literal(void)
{
    typedef struct prefix_struct_t {
        char* input;
        char* operator;
        int integer_value;
    } prefix_struct_t;
    prefix_struct_t tests[2];
    prefix_struct_t test1, test2;
    test1.input = strdup("!5");
    test1.operator= strdup("!");
    test1.integer_value = 5;
    test2.input = strdup("-15");
    test2.operator= strdup("-");
    test2.integer_value = 15;
    tests[0] = test1;
    tests[1] = test2;

    int i;
    for (i = 0; i < 2; ++i) {
        prefix_struct_t curr_test = tests[i];
        lexer_t* lexer = new_lexer(curr_test.input, strlen(curr_test.input));
        parser_t* parser = new_parser(lexer);
        program_t* program_node = parse_program(parser);

        if (program_node->statements_size != 1) {
            fprintf(stderr, "Expected %d statements , got %d statements", 1,
                program_node->statements_size);
            exit(-1);
        }
        statement_t* test_node = program_node->statements[0];

        if (strcmp(test_node->expression->op, curr_test.operator) != 0) {
            fprintf(stderr, "Expected %s operand , got %s", test_node->expression->op,
                curr_test.operator);
            exit(-1);
        }
        // check for literal
        if (test_node->expression->right->int_value != curr_test.integer_value) {
            fprintf(stderr, "Expected %d operand , got %d",
                test_node->expression->right->int_value, curr_test.integer_value);
            exit(-1);
        }
    }
    printf("All test cases passed ✅");
    return 0;
}

int test_infix_expressions(void)
{
    typedef struct infix_test_t {
        char* input;
        int left_value;
        char* op;
        int right_value;
    } infix_test_t;
    infix_test_t tests[] = {
        { "5 + 5;", 5, "+", 5 },
        { "5 - 5;", 5, "-", 5 },
        { "5 * 5;", 5, "*", 5 },
        { "5 / 5;", 5, "/", 5 },
        { "5 > 5;", 5, ">", 5 },
        { "5 < 5;", 5, "<", 5 },
        { "5 == 5;", 5, "==", 5 },
        { "5 != 5;", 5, "!=", 5 },
    };
    int tests_size = sizeof(tests) / sizeof(tests[0]);
    int i;
    for (i = 0; i < tests_size; ++i) {
        infix_test_t curr_test = tests[i];
        lexer_t* lexer = new_lexer(curr_test.input, strlen(curr_test.input));
        parser_t* parser = new_parser(lexer);
        program_t* program_node = parse_program(parser);

        if (program_node->statements_size != 1) {
            fprintf(
                stderr,
                "Error: expected %d statements, got %d statements at test case %d",
                STATEMENTS_NUMBER, program_node->statements_size, i + 1);
            return -1;
        }
        if (program_node->statements[0]->expression->left->int_value != curr_test.left_value) {
            fprintf(stderr, "Error: expected %d value, got %d value at test case %d",
                program_node->statements[0]->expression->left->int_value,
                curr_test.left_value, i + 1);
            return -1;
        }

        if (program_node->statements[0]->expression->right->int_value != curr_test.right_value) {
            fprintf(stderr, "Error: expected %d value, got %d value at test case %d",
                program_node->statements[0]->expression->right->int_value,
                curr_test.right_value, i + 1);
            return -1;
        }
    }
    fprintf(stdout, "All test cases passed ✅");
    return 0;
}

int test_operator_precedence()
{
    typedef struct input_expected_t {
        char* input;
        char* expected;
    } input_expected_t;
    input_expected_t tests[] = {
        { "-a * b", "((-a) * b)" },
        { "!-a", "(!(-a))" },
        { "a + b + c", "((a + b) + c)" },
        { "a + b - c", "((a + b) - c)" },
        { "a * b * c", "((a * b) * c)" },
        { "a * b / c", "((a * b) / c)" },
        { "a + b / c", "(a + (b / c))" },
        { "a + b * c + d / e - f", "(((a + (b * c)) + (d / e)) - f)" },
        { "3 + 4; -5 * 5", "(3 + 4)((-5) * 5)" },
        { "5 > 4 == 3 < 4", "((5 > 4) == (3 < 4))" },
        { "5 < 4 != 3 > 4", "((5 < 4) != (3 > 4))" },
        { "3 + 4 * 5 == 3 * 1 + 4 * 5", "((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))" },
    };
    int arr_size = sizeof(tests) / sizeof(tests[0]);
    int i;
    for (i = 0; i < arr_size; ++i) {
        input_expected_t curr_test = tests[i];
        lexer_t* lexer = new_lexer(curr_test.input, strlen(curr_test.input));
        parser_t* parser = new_parser(lexer);
        program_t* program_node = parse_program(parser);
        char* program_string = program_node->string((void*)program_node);
        if (strcmp(curr_test.expected, program_string) != 0) {
            fprintf(stderr, "Failure at expression: %s", curr_test.input);
            fprintf(stderr, "Expected %s,Got %s", curr_test.expected, program_string);
            exit(-1);
        }
    }
    fprintf(stdout, "All test cases passed ✅");
    return 0;
}

int boolean_parse_test(void){
  char* tests[] = {"true;","false;"};
    // ,"let foobar = true","let barfoo = false"};
  // char* tests2[] = {"let foobar = true;","let barfoo = false;"};
  typedef struct test_precedence_t {
    char* input;
    char* expected;
  } test_precedence_t;
  test_precedence_t tests3[] = {{"true;","true"},{"false;","false"},
  {"3 > 5 == false;","((3 > 5) == false)"},{"3 < 5 == true;","((3 < 5) == true)"}};
  //TODO: test later for the infix expressions(after finishing complete implementation of let) 
  int expected_values[] = {1,0};
  int tests_size = sizeof(tests)/sizeof(tests[0]);
  int tests3_size = sizeof(tests3)/sizeof(tests3[0]);
  int i;
  for(i = 0;i<tests_size;++i){
    char* curr_test = tests[i];
    lexer_t* lexer = new_lexer(curr_test, strlen(curr_test));
    parser_t* parser = new_parser(lexer);
    program_t* program_node = parse_program(parser);
    if(program_node->statements_size != 1){
      fprintf(stderr,"Statements size expected to be 1, got %d",program_node->statements_size);
      exit(-1);
    }
    if(program_node->statements[0]->expression->boolean_value != expected_values[i]){
      fprintf(stdout,"Error for input %s",tests[i]);
      fprintf(stdout,"Boolean value expected: %d,%d\n",expected_values[i],program_node->statements[0]->expression->boolean_value);
      fprintf(stderr,"Statements value expected to be %d, got %d\n",program_node->statements[0]->expression->boolean_value,expected_values[i]);
      exit(-1);
    }
  }
  for(i = 0;i<tests3_size;++i){
    test_precedence_t curr_test = tests3[i];
    lexer_t* lexer = new_lexer(curr_test.input,strlen(curr_test.input));
    parser_t* parser = new_parser(lexer);
    program_t* program_node = parse_program(parser);
    if(program_node->statements_size != 1){
      fprintf(stderr,"Statements size expected to be 1, got %d",program_node->statements_size);
      exit(-1);
    }
    char* program_string = program_node->string((void*)program_node);
    if(strcmp(program_string,curr_test.expected) != 0){
      fprintf(stderr,"Expected %s, got %s",program_string,curr_test.expected);
      exit(-1);
    }
  }
  fprintf(stdout, "All test cases passed ✅");
  return 0;
}

int main(void) { return boolean_parse_test(); }
