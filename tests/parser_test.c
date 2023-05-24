#include "../src/parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool test_let_statement(statement_t* ast_statement, char* name)
{
    char* token_literal = ast_statement->node.token_literal((void*)ast_statement);
    if (strcmp(token_literal, "LET") != 0) {
        fprintf(stderr, "Expected: let statement, Got: %s", token_literal);
        return false;
    }
    let_statement_t* let_statement = (let_statement_t*)ast_statement;
    if (strcmp(name, let_statement->name->value) != 0) {
        fprintf(stderr, "Expected: name of literal: %s, got %s", name,
            let_statement->name->value);
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
    if (program_node->statements_length != 3) {
        fprintf(stderr, "Error: expected %d statements, got %zu statements",
            3, program_node->statements_length);
        return 69;
    }
    char* expectedIdentifiers[] = { "x", "y", "foobar" };
    int i;
    for (i = 0; i < 3; ++i) {
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
    if (program_node->statements_length != 3) {
        fprintf(stderr, "Error: expected %d statements, got %zu statements",
            3, program_node->statements_length);
        return 69;
    }
    int i;
    for (i = 0; i < 3; ++i) {
        statement_t* stmt = program_node->statements[i];
        if (strcmp(stmt->node.token_literal((void*)stmt), "RETURN") != 0) {
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
    printf("%s", program_node->node.string((void*)program_node));
    return 0;
}

int test_parsing_identifiers(void)
{
    char* input = "foobar;";
    lexer_t* lexer = new_lexer(input, strlen(input));
    parser_t* parser = new_parser(lexer);
    program_t* program_node = parse_program(parser);
    if (program_node->statements_length != 1) {
        fprintf(stderr, "Expected %d statements , got %zu statements", 1,
            program_node->statements_length);
        exit(-1);
    }

    char* identifier_name = ((identifier_t*)((expression_statement_t*)program_node->statements[0])->expression)->value;
    if (strcmp(identifier_name, "foobar") != 0) {
        fprintf(stderr, "Expected identifier name is %s, got %s", "foobar", identifier_name);
        exit(-1);
    }

    if (strcmp(program_node->statements[0]->node.token_literal(
                   (void*)program_node->statements[0]),
            "foobar")
        != 0) {
        fprintf(stderr, "Expected identifier name is %s, got %s", "foobar",
            program_node->statements[0]->node.token_literal(
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
    if (program_node->statements_length != 1) {
        fprintf(stderr, "Expected %d statements , got %zu statements", 1,
            program_node->statements_length);
        exit(-1);
    }
    integer_t* integer_expr = ((integer_t*)((expression_statement_t*)program_node->statements[0])->expression);
    if (strcmp(program_node->statements[0]->node.token_literal(
                   (void*)program_node->statements[0]),
            "5")
        != 0) {
        fprintf(stderr, "Expected identifier name is %s, got %s", "5",
            program_node->statements[0]->node.token_literal(
                (void*)program_node->statements[0]));
        exit(-1);
    }
    if (((integer_t*)(((expression_statement_t*)(program_node->statements[0]))->expression))->value != 5) {
        fprintf(stderr, "Expected integer value is %d got %lld", 5,
            ((integer_t*)(((expression_statement_t*)(program_node->statements[0]))->expression))->value);
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

        if (program_node->statements_length != 1) {
            fprintf(stderr, "Expected %d statements , got %zu statements", 1,
                program_node->statements_length);
            exit(-1);
        }
        expression_statement_t* test_node = (expression_statement_t*)program_node->statements[0];
        prefix_expression_t* prefix_expression = (prefix_expression_t*)test_node->expression;

        if (strcmp(prefix_expression->op, curr_test.operator) != 0) {
            fprintf(stderr, "Expected %s operand , got %s", prefix_expression->op,
                curr_test.operator);
            exit(-1);
        }
        // check for literal
        integer_t* integer_right = (integer_t*)(prefix_expression->right);
        if (integer_right->value != curr_test.integer_value) {
            fprintf(stderr, "Expected %lld operand , got %d",
                integer_right->value, curr_test.integer_value);
            exit(-1);
        }
    }
    printf("All test cases passed ✅");
    return 0;
}

typedef struct infix_test_t {
    char* input;
    int left_value;
    char* op;
    int right_value;
} infix_test_t;

void test_infix_expression(infix_test_t* curr_test_pointer, int i)
{
    infix_test_t curr_test = *curr_test_pointer;
    lexer_t* lexer = new_lexer(curr_test.input, strlen(curr_test.input));
    parser_t* parser = new_parser(lexer);
    program_t* program_node = parse_program(parser);

    if (program_node->statements_length != 1) {
        fprintf(
            stderr,
            "Error: expected %d statements, got %zu statements at test case %d",
            1, program_node->statements_length, i + 1);
        exit(-1);
    }
    expression_statement_t* stmt = (expression_statement_t*)program_node->statements[0];
    infix_expression_t* infix_expr = (infix_expression_t*)stmt->expression;
    integer_t* integer_left = (integer_t*)infix_expr->left;
    integer_t* integer_right = (integer_t*)infix_expr->right;
    if (integer_left->value != curr_test.left_value) {
        fprintf(stderr, "Error: expected %lld value, got %d value at test case %d",
            integer_left->value,
            curr_test.left_value, i + 1);
        exit(-1);
    }

    if (integer_right->value != curr_test.right_value) {
        fprintf(stderr, "Error: expected %lld value, got %d value at test case %d",
            integer_right->value,
            curr_test.right_value, i + 1);
        exit(-1);
    }
}

int test_infix_expressions(void)
{
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
        test_infix_expression(tests + i, i);
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
        char* program_string = program_node->node.string((void*)program_node);
        if (strcmp(curr_test.expected, program_string) != 0) {
            fprintf(stderr, "Failure at expression: %s", curr_test.input);
            fprintf(stderr, "Expected %s,Got %s", curr_test.expected, program_string);
            exit(-1);
        }
    }
    fprintf(stdout, "All test cases passed ✅");
    return 0;
}

int boolean_parse_test(void)
{
    char* tests[] = { "true;", "false;" };
    // ,"let foobar = true","let barfoo = false"};
    // char* tests2[] = {"let foobar = true;","let barfoo = false;"};
    typedef struct test_precedence_t {
        char* input;
        char* expected;
    } test_precedence_t;
    test_precedence_t tests3[] = { { "true;", "true" }, { "false;", "false" },
        { "3 > 5 == false;", "((3 > 5) == false)" }, { "3 < 5 == true;", "((3 < 5) == true)" } };
    typedef struct infix_test_t {
        char* input;
        bool left;
        char* op;
        bool right;
    } infix_test_t;
    infix_test_t tests4[] = { { "true == true", true, "==", true }, { "true != false", true, "!=", false },
        { "false == false", false, "==", false } };
    typedef struct prefix_test_t {
        char* input;
        char* op;
        bool value;
    } prefix_test_t;
    prefix_test_t tests5[] = { { "!true", "!", true }, { "!false", "!", false } };
    //TODO: test later for the infix expressions(after finishing complete implementation of let)
    int expected_values[] = { 1, 0 };
    int tests_size = sizeof(tests) / sizeof(tests[0]);
    int tests3_size = sizeof(tests3) / sizeof(tests3[0]);
    int tests4_size = sizeof(tests4) / sizeof(tests4[0]);
    int tests5_size = sizeof(tests5) / sizeof(tests5[0]);
    int i;
    for (i = 0; i < tests_size; ++i) {
        char* curr_test = tests[i];
        lexer_t* lexer = new_lexer(curr_test, strlen(curr_test));
        parser_t* parser = new_parser(lexer);
        program_t* program_node = parse_program(parser);
        if (program_node->statements_length != 1) {
            fprintf(stderr, "Statements size expected to be 1, got %zu", program_node->statements_length);
            exit(-1);
        }
        expression_statement_t* stmt = (expression_statement_t*)program_node->statements[0];
        boolean_expression_t* boolean_expression = (boolean_expression_t*)stmt->expression;
        if (boolean_expression->value != expected_values[i]) {
            fprintf(stdout, "Error for input %s", tests[i]);
            fprintf(stdout, "Boolean value expected: %d,%d\n", expected_values[i], boolean_expression->value);
            fprintf(stderr, "Statements value expected to be %d, got %d\n", boolean_expression->value, expected_values[i]);
            exit(-1);
        }
    }
    for (i = 0; i < tests3_size; ++i) {
        test_precedence_t curr_test = tests3[i];
        lexer_t* lexer = new_lexer(curr_test.input, strlen(curr_test.input));
        parser_t* parser = new_parser(lexer);
        program_t* program_node = parse_program(parser);
        if (program_node->statements_length != 1) {
            fprintf(stderr, "Statements size expected to be 1, got %zu", program_node->statements_length);
            exit(-1);
        }
        char* program_string = program_node->node.string((void*)program_node);
        if (strcmp(program_string, curr_test.expected) != 0) {
            fprintf(stderr, "Expected %s, got %s", program_string, curr_test.expected);
            exit(-1);
        }
    }
    for (i = 0; i < tests4_size; ++i) {
        infix_test_t curr_test = tests4[i];
        lexer_t* lexer = new_lexer(curr_test.input, strlen(curr_test.input));
        parser_t* parser = new_parser(lexer);
        program_t* program_node = parse_program(parser);
        expression_statement_t* stmt = (expression_statement_t*)program_node->statements[0];
        infix_expression_t* infix_expr = (infix_expression_t*)stmt->expression;
        boolean_expression_t* boolean_left = (boolean_expression_t*)infix_expr->left;
        boolean_expression_t* boolean_right = (boolean_expression_t*)infix_expr->right;

        if (strcmp(infix_expr->op, curr_test.op) != 0) {
            fprintf(stderr, "Expected %s,got %s", infix_expr->op, curr_test.op);
            exit(-1);
        }
        if (boolean_left->value != curr_test.left) {
            fprintf(stderr, "Expected %d,got %d", boolean_left->value, curr_test.left);
            exit(-1);
        }
        if (boolean_right->value != curr_test.right) {
            fprintf(stderr, "Expected %d,got %d", boolean_right->value, curr_test.right);
            exit(-1);
        }
    }
    for (i = 0; i < tests5_size; ++i) {
        prefix_test_t curr_test = tests5[i];
        lexer_t* lexer = new_lexer(curr_test.input, strlen(curr_test.input));
        parser_t* parser = new_parser(lexer);
        program_t* program_node = parse_program(parser);
        expression_statement_t* stmt = (expression_statement_t*)program_node->statements[0];
        prefix_expression_t* prefix_expr = (prefix_expression_t*)stmt->expression;
        boolean_expression_t* boolean_right = (boolean_expression_t*)prefix_expr->right;
        if (strcmp(prefix_expr->op, curr_test.op) != 0) {
            fprintf(stderr, "Expected %s, got %s", prefix_expr->op, curr_test.op);
            exit(-1);
        }
        if (boolean_right->value != curr_test.value) {
            fprintf(stderr, "Expected %d, got %d", boolean_right->value, curr_test.value);
            exit(-1);
        }
    }
    fprintf(stdout, "All test cases passed ✅");
    return 0;
}

int test_grouped_expressions(void)
{
    typedef struct grouped_expr_test_t {
        char* input;
        char* expected;
    } grouped_expr_test_t;
    grouped_expr_test_t tests[] = { { "1 + (2+3) + 4", "((1 + (2 + 3)) + 4)" },
        { "(5 + 5) * 2", "((5 + 5) * 2)" },
        { "2 / (5 + 5)", "(2 / (5 + 5))" },
        { "-(5 + 5)", "(-(5 + 5))" },
        { "!(true == true)", "(!(true == true))" } };
    int tests_size = sizeof(tests) / sizeof(tests[0]);
    int i;
    for (i = 0; i < tests_size; ++i) {
        grouped_expr_test_t curr_test = tests[i];
        lexer_t* lexer = new_lexer(curr_test.input, strlen(curr_test.input));
        parser_t* parser = new_parser(lexer);
        program_t* program_node = parse_program(parser);
        char* program_string = program_node->node.string((void*)program_node);
        if (strcmp(program_string, curr_test.expected) != 0) {
            fprintf(stderr, "Expected %s, Got %s", program_string, curr_test.expected);
            exit(-1);
        }
    }
    fprintf(stdout, "All test cases passed ✅");
    return 0;
}

int test_if_expression(void)
{
    char* input = "if (x<y) { x }";
    lexer_t* lexer = new_lexer(input, strlen(input));
    parser_t* paresr = new_parser(lexer);
    program_t* program_node = parse_program(paresr);
    if (program_node->statements_length != 1) {
        fprintf(stderr, "Expected %zu elements, got %d elements", program_node->statements_length, 1);
        exit(-1);
    }
    expression_statement_t* stmt = (expression_statement_t*)program_node->statements[0];
    if_expression_t* expr = (if_expression_t*)stmt->expression;
    infix_expression_t* condition = (infix_expression_t*)expr->condition;
    if (strcmp(condition->op, "<") != 0) {
        fprintf(stderr, "Expected %s,got %s", "<", condition->op);
        exit(-1);
    }
    if (strcmp(((identifier_t*)condition->left)->value, "x") != 0) {
        fprintf(stderr, "Expected %s,got %s", "x", ((identifier_t*)condition->left)->value);
        exit(-1);
    }
    if (strcmp(((identifier_t*)condition->right)->value, "y") != 0) {
        fprintf(stderr, "Expected %s,got %s", "y", ((identifier_t*)condition->right)->value);
        exit(-1);
    }
    if (expr->consequence->statements_length != 1) {
        fprintf(stderr, "Expected %d,got %zu", 1, expr->consequence->statements_length);
        exit(-1);
    }
    expression_statement_t* consequence_stmt = (expression_statement_t*)expr->consequence->statements[0];
    identifier_t* consequence = (identifier_t*)consequence_stmt->expression;
    if (strcmp(consequence->value, "x")) {
        fprintf(stderr, "Expected %s,got %s", "x", consequence->value);
        exit(-1);
    }
    if (expr->alternative != NULL) {
        fprintf(stderr, "Alternative should be null");
        exit(-1);
    }
    fprintf(stdout, "All test cases passed ✅");
    return 0;
}

int test_if_else_expression(void)
{
    char* input = "if (x<y) { x } else { y }";
    lexer_t* lexer = new_lexer(input, strlen(input));
    parser_t* paresr = new_parser(lexer);
    program_t* program_node = parse_program(paresr);
    if (program_node->statements_length != 1) {
        fprintf(stderr, "Expected %zu elements, got %d elements", program_node->statements_length, 1);
        exit(-1);
    }
    expression_statement_t* stmt = (expression_statement_t*)program_node->statements[0];
    if_expression_t* expr = (if_expression_t*)stmt->expression;
    infix_expression_t* condition = (infix_expression_t*)expr->condition;
    identifier_t* left_iden_condition = (identifier_t*)condition->left;
    identifier_t* right_iden_condition = (identifier_t*)condition->right;
    block_statement_t* consequence = expr->consequence;
    block_statement_t* alternative = expr->alternative;
    expression_statement_t* consequence_expr = (expression_statement_t*)consequence->statements[0];
    expression_statement_t* alternative_expr = (expression_statement_t*)alternative->statements[0];

    identifier_t* consequence_iden = (identifier_t*)consequence_expr->expression;
    identifier_t* alternative_iden = (identifier_t*)alternative_expr->expression;
    if (strcmp(condition->op, "<") != 0) {
        fprintf(stderr, "Expected %s,got %s", "<", condition->op);
        exit(-1);
    }
    if (strcmp(left_iden_condition->value, "x") != 0) {
        fprintf(stderr, "Expected %s,got %s", "x", left_iden_condition->value);
        exit(-1);
    }
    if (strcmp((char*)right_iden_condition->value, "y") != 0) {
        fprintf(stderr, "Expected %s,got %s", "y", right_iden_condition->value);
        exit(-1);
    }
    if (consequence->statements_length != 1) {
        fprintf(stderr, "Expected %d,got %zu", 1, consequence->statements_length);
        exit(-1);
    }
    if (strcmp(consequence_iden->value, "x")) {
        fprintf(stderr, "Expected %s,got %s", "x", consequence_iden->value);
        exit(-1);
    }
    if (strcmp(alternative_iden->value, "y")) {
        fprintf(stderr, "Expected %s,got %s", "x", alternative_iden->value);
        exit(-1);
    }
    fprintf(stdout, "All test cases passed ✅");
    return 0;
}

int test_function_literal_parsing(void)
{
    char* input = "fn(x,y) { x + y; }";
    lexer_t* lexer = new_lexer(input, strlen(input));
    parser_t* parser = new_parser(lexer);
    program_t* program_node = parse_program(parser);

    if (program_node->statements_length != 1) {
        fprintf(stderr, "Expected statements length to be %d, got %zu", 1, program_node->statements_length);
        exit(-1);
    }

    expression_statement_t* stmt = (expression_statement_t*)program_node->statements[0];
    function_literal_t* function_expression = (function_literal_t*)stmt->expression;
    if (function_expression->arguments_length != 2) {
        fprintf(stderr, "Expected %d arguments,got %zu", 2, function_expression->arguments_length);
        exit(-1);
    }

    if (strcmp(function_expression->arguments[0]->node.string((void*)function_expression->arguments[0]), "x") != 0) {
        fprintf(stderr, "Expected value %s,got %s", "x", function_expression->arguments[0]->node.string((void*)function_expression->arguments[0]));
        exit(-1);
    }

    if (strcmp(function_expression->arguments[1]->node.string((void*)function_expression->arguments[1]), "y") != 0) {
        fprintf(stderr, "Expected value %s,got %s", "y", function_expression->arguments[1]->node.string((void*)function_expression->arguments[1]));
        exit(-1);
    }

    if (function_expression->body->statements_length != 1) {
        fprintf(stderr, "Expected %d statements in body, got %zu", 1, function_expression->body->statements_length);
        exit(-1);
    }
    expression_statement_t* body_statement = (expression_statement_t*)function_expression->body->statements[0];
    infix_expression_t* infix_expr = (infix_expression_t*)body_statement->expression;

    if (strcmp(((identifier_t*)infix_expr->left)->value, "x") != 0) {
        fprintf(stderr, "Expected %s, got %s", "x", ((identifier_t*)infix_expr->left)->value);
        exit(-1);
    }

    if (strcmp((infix_expr->op), "+") != 0) {
        fprintf(stderr, "Expected %s, got %s", "+", infix_expr->op);
        exit(-1);
    }
    if (strcmp(((identifier_t*)infix_expr->right)->value, "y") != 0) {
        fprintf(stderr, "Expected %s, got %s", "y", ((identifier_t*)infix_expr->right)->value);
        exit(-1);
    }

    fprintf(stdout, "All test cases passed ✅");
    return 0;
}

int test_call_expressions(void)
{
    char* input = "add(1,2*3,4+5)";
    lexer_t* lexer = new_lexer(input, strlen(input));
    parser_t* parser = new_parser(lexer);
    program_t* program_node = parse_program(parser);
    if (program_node->statements_length != 1) {
        fprintf(stderr, "Expected statements length to be %d, got %zu", 1, program_node->statements_length);
        exit(-1);
    }

    expression_statement_t* stmt = (expression_statement_t*)program_node->statements[0];
    call_expression_t* call_expression = (call_expression_t*)stmt->expression;
    if (strcmp(call_expression->function->node.string((void*)call_expression->function), "add") != 0) {
        fprintf(stderr, "Expected %s , got %s", "add", call_expression->function->node.string((void*)call_expression->function));
        exit(-1);
    }
    if (call_expression->arguments_length != 3) {
        fprintf(stderr, "Expected 3 arguments,got %zu", call_expression->arguments_length);
        exit(-1);
    }
    integer_t* first_argument = (integer_t*)call_expression->arguments[0];
    if (first_argument->value != 1) {
        fprintf(stderr, "Expected first argument as 1,got %lld", first_argument->value);
        exit(-1);
    }
    infix_expression_t* second_argument = (infix_expression_t*)call_expression->arguments[1];
    infix_expression_t* third_argument = (infix_expression_t*)call_expression->arguments[2];
    if (((integer_t*)second_argument->left)->value == 2) {
        fprintf(stderr, "Expected %d, got %lld", 2, ((integer_t*)second_argument->left)->value);
        exit(-1);
    }
    if (strcmp(second_argument->op, "*") != 0) {
        fprintf(stderr, "Expected %s, got %s", "*", second_argument->op);
        exit(-1);
    }
    if (((integer_t*)second_argument->right)->value == 3) {

        fprintf(stderr, "Expected %d, got %lld", 3, ((integer_t*)second_argument->right)->value);
        exit(-1);
    }

    if (((integer_t*)third_argument->left)->value == 4) {
        fprintf(stderr, "Expected %d, got %lld", 4, ((integer_t*)third_argument->left)->value);
        exit(-1);
    }
    if (strcmp(third_argument->op, "+") != 0) {
        fprintf(stderr, "Expected %s, got %s", "+", third_argument->op);
        exit(-1);
    }
    if (((integer_t*)third_argument->right)->value == 5) {
        fprintf(stderr, "Expected %d, got %lld", 5, ((integer_t*)third_argument->right)->value);
        exit(-1);
    }

    fprintf(stdout, "All test cases passed ✅");
    return 0;
}

int main(void)
{
    test_let_statements();
    test_if_expression();
    test_if_else_expression();
    test_operator_precedence();
    test_ret_statements();
    test_infix_expressions();
    test_parsing_prefix_literal();
    test_parsing_integer_literal();
    test_parsing_identifiers();
    boolean_parse_test();
    test_grouped_expressions();
    // test_call_expressions();
    return test_function_literal_parsing();
}
