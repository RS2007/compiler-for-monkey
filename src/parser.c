#include "parser.h"
#include "nodes.h"
#include "precedences.h"
#include "token.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#define DEFAULT_DYNAMIC_ARR_SIZE 10
#define STRING_MAX_SIZE 1024
#define MAX_ERROR_SIZE 1024
#define unimplemented printf("\nUnimplemented\n");

char* print_program_string(void* program_node_cast_to_void)
{
    program_t* program_node = ((program_t*)program_node_cast_to_void);
    char* program_node_string = (char*)malloc(32768 * sizeof(char));
    program_node_string[0] = '\0';
    for (int i = 0; i < program_node->statements_length; ++i) {
        // FIXME: might require a typecast to original statement type before accessing the function pointer
        statement_t* stmt = program_node->statements[i];
        char temp_string[512];
        sprintf(temp_string, "%s", stmt->node.string((void*)stmt));
        strcat(program_node_string, temp_string);
    }
    strcat(program_node_string, "\0");
    return program_node_string;
}

char* print_let_string(void* let_statement_cast_to_void)
{
    let_statement_t* let_statement = ((let_statement_t*)let_statement_cast_to_void);
    char* let_node_string = (char*)malloc(STRING_MAX_SIZE);
    sprintf(let_node_string, "%s %s = %s", token_strings[let_statement->token->type],
        let_statement->name->value, "unimplemented");
    return let_node_string;
}

char* print_return_string(void* ret_statement_cast_to_void)
{
    ret_statement_t* ret_statement = ((ret_statement_t*)ret_statement_cast_to_void);
    char* ret_node_string = (char*)malloc(STRING_MAX_SIZE);
    sprintf(ret_node_string, "%s %s", token_strings[ret_statement->token->type],
        "unimplemented");
    return ret_node_string;
}

char* print_expression_statement_string(void* expression_statement_cast_to_void)
{
    expression_statement_t* expression_statement = (expression_statement_t*)expression_statement_cast_to_void;
    return expression_statement->expression->node.string(
        (void*)expression_statement->expression);
}

char* print_infix_expression_string(void* expression_statement_cast_to_void)
{
    expression_statement_t* expression_statement = (expression_statement_t*)expression_statement_cast_to_void;
    infix_expression_t* infix_expression = (infix_expression_t*)expression_statement->expression;
    char* infix_expression_string = (char*)malloc(STRING_MAX_SIZE);
    sprintf(infix_expression_string, "(%s %s %s)",
        infix_expression->left->node.string(
            (void*)infix_expression->left),
        infix_expression->op,
        infix_expression->right->node.string(
            (void*)infix_expression->right));
    return infix_expression_string;
}

char* print_prefix_expression_string(void* expression_statement_cast_to_void)
{
    prefix_expression_t* prefix_expression = (prefix_expression_t*)expression_statement_cast_to_void;
    char* prefix_expression_string = (char*)malloc(STRING_MAX_SIZE);
    sprintf(prefix_expression_string, "(%s%s)", prefix_expression->op,
        prefix_expression->right->node.string(
            (void*)prefix_expression->right));
    return prefix_expression_string;
}

char* print_block_statement(void* block_statement_cast_to_void)
{
    block_statement_t* block_statement = (block_statement_t*)block_statement_cast_to_void;
    char* block_statement_string = (char*)malloc(STRING_MAX_SIZE);
    block_statement_string[0] = '\0';
    for (int i = 0; i < block_statement->statements_length; ++i) {
        // FIXME: will require a typeast to its original statement type before accessing the function pointer.
        statement_t* stmt = block_statement->statements[i];
        char temp_string[512];
        sprintf(temp_string, "%s", stmt->node.string((void*)stmt));
        strcat(block_statement_string, temp_string);
    }
    strcat(block_statement_string, "\0");
    return block_statement_string;
}


char* get_concated_arguments_string(expression_t** parameters,int parameters_length){
  return "hello";
}

char* get_body_string_function(function_literal_t* function_expression){
  return "world!";
}


char* print_function_string(void* function_expression_cast_to_void){
  function_literal_t* function_expression = (function_literal_t*)function_expression_cast_to_void;
  char* function_expression_string = (char*)malloc(STRING_MAX_SIZE);
  sprintf(function_expression_string,"%s(%s)%s","fn",get_concated_arguments_string(function_expression->arguments,function_expression->arguments_length),get_body_string_function(function_expression));
  return function_expression_string;
}

precedence_t peek_precedence(parser_t* parser)
{
    precedence_t precedence = precedences_array[parser->peek_token->type];
    // TODO: fix this lint error
    if (precedence != NULL) {
        return precedence;
    }
    return LOWEST;
}

precedence_t curr_precedence(parser_t* parser)
{
    precedence_t precedence = precedences_array[parser->curr_token->type];
    // TODO: fix this lint error
    if (precedence != NULL) {
        return precedence;
    }
    return LOWEST;
}

program_t* push_statements_program(program_t* program, statement_t* stmt)
{
    if (program->statements_length + 1 > program->statements_capacity) {
        program->statements = realloc(program->statements, program->statements_capacity * 2);
    }
    program->statements[program->statements_length++] = stmt;
    return program;
}

block_statement_t* push_statements_block_statement(block_statement_t* block_statement, statement_t* stmt)
{
    if (block_statement->statements_length + 1 > block_statement->statements_capacity) {
        block_statement->statements = realloc(block_statement->statements, block_statement->statements_capacity * 2);
    }
    block_statement->statements[block_statement->statements_length++] = stmt;
    return block_statement;
}

parser_t* push_errors_parser(parser_t* parser, char* error_msg)
{
    if (parser->errors_size + 1 > parser->errors_capacity) {
        parser->errors = realloc(parser->errors, parser->errors_capacity * 2);
    }
    parser->errors[parser->errors_size++] = error_msg;
    return parser;
}

parser_t* new_parser(lexer_t* lexer)
{
    parser_t* parser = (parser_t*)malloc(sizeof(parser_t));
    parser->lexer = lexer;
    parser->errors = (char**)calloc(DEFAULT_DYNAMIC_ARR_SIZE, MAX_ERROR_SIZE);
    parser->errors_size = 0;
    parser->errors_capacity = DEFAULT_DYNAMIC_ARR_SIZE;
    // run twice to populate the curr_token and peek_token
    next_token_parser(parser);
    next_token_parser(parser);
    return parser;
}

void next_token_parser(parser_t* parser)
{
    parser->curr_token = parser->peek_token;
    parser->peek_token = next_token_lexer(parser->lexer);
}

char* program_token_literal(void* program_cast_to_void)
{
    program_t* program = (program_t*)program_cast_to_void;
    if (program->statements_length <= 0) {
        fprintf(stderr, "Fatal: program node has an invalid number of statements");
        exit(-1);
    }
    return program->statements[0]->node.token_literal(program->statements[0]);
}

char* let_token_literal(void* let_statement_cast_to_void)
{
    let_statement_t* let_statement = (let_statement_t*)let_statement_cast_to_void;
    return token_strings[let_statement->token->type];
}

char* ret_token_literal(void* ret_statement_cast_to_void)
{
    // TODO: is it possible to make a abstracted token literal function
    ret_statement_t* ret_statement = (ret_statement_t*)ret_statement_cast_to_void;
    return token_strings[ret_statement->token->type];
}

char* expression_token_literal(void* expression_statement_cast_to_void)
{
    expression_statement_t* expression_statement = (expression_statement_t*)expression_statement_cast_to_void;
    identifier_t* identifier_expr = (identifier_t*)expression_statement->expression;
  return identifier_expr->value;
}

void statement_node_let(void* let_statement_cast_to_void)
{
    let_statement_t* let_statement = (let_statement_t*)let_statement_cast_to_void;
    return;
}

program_t* new_program_node()
{
    program_t* program_node = (program_t*)malloc(sizeof(program_t));
    if (program_node == NULL) {
        fprintf(stderr, "Fatal: Cannot allocate memory of %zu bytes",
            sizeof(program_t));
    }
    program_node->node.type = PROGRAM;
    program_node->node.token_literal = program_token_literal;
    // TODO: function pointer token_literal
    program_node->statements = (statement_t**)calloc(DEFAULT_DYNAMIC_ARR_SIZE, sizeof(statement_t));
    program_node->statements_length = 0;
    program_node->statements_capacity = DEFAULT_DYNAMIC_ARR_SIZE;
    program_node->node.string = print_program_string;
    return program_node;
}

bool is_curr_token(parser_t* parser, token_type type)
{
    return parser->curr_token->type == type;
}

bool is_peek_token(parser_t* parser, token_type type)
{
    return parser->peek_token->type == type;
}

bool expect_peek_token(parser_t* parser, token_type type)
{
    /*
   * checks if peeked token is of the given type, if yes moves the parser to the
   * next token and returns true, if not just returns false without moving the
   * pointers
   */
    if (is_peek_token(parser, type)) {
        next_token_parser(parser);
        return true;
    }
    return false;
}

statement_t* parse_let_statement(parser_t* parser)
{
    let_statement_t* stmt = (let_statement_t*)malloc(sizeof(let_statement_t));
    // 1. token info
    stmt->token = parser->curr_token;
    stmt->statement.node.token_literal = let_token_literal;
    stmt->statement.node.string = print_let_string;
    if (!expect_peek_token(parser, IDENT)) {
        char* error_string = (char*)malloc(MAX_ERROR_SIZE);
        snprintf(error_string, MAX_ERROR_SIZE, "Let is not followed by literal");
        fprintf(stderr, "Let is not followed by literal");
        push_errors_parser(parser, error_string);
        exit(-1);
        // FIXME: find a way to bubble errors and then print them in the root
        // program
    }
    // 2. identifier info
    stmt->name->value = parser->curr_token->literal;
    if (!expect_peek_token(parser, ASSIGN)) {
        char* error_string = (char*)malloc(MAX_ERROR_SIZE);
        snprintf(error_string, MAX_ERROR_SIZE, "Expected token:%s ,Got: %s",
            token_strings[ASSIGN], token_strings[parser->peek_token->type]);
        printf("Expected token:%s ,Got: %s", token_strings[ASSIGN],
            token_strings[parser->peek_token->type]);
        push_errors_parser(parser, error_string);
        exit(-1);
        // FIXME: find a way to bubble errors and then print them in the root
        // program
    }
    while (!is_curr_token(parser, SEMICOLON)) {
        next_token_parser(parser);
    }
    return (statement_t*)stmt;
}
statement_t* parse_return_statement(parser_t* parser)
{
    ret_statement_t* stmt = (ret_statement_t*)malloc(sizeof(ret_statement_t));
    stmt->token = parser->curr_token;
    stmt->statement.node.token_literal = ret_token_literal;
    stmt->statement.node.string = print_return_string;
    next_token_parser(parser);
    while (!is_curr_token(parser, SEMICOLON)) {
        next_token_parser(parser);
    }
    return (statement_t*)stmt;
}

expression_t* parse_expression(parser_t* parser, precedence_t precedence)
{
    prefix_parse_function prefix = prefix_parse_functions[parser->curr_token->type];
    // TODO: not sure about this check, might have to remove later
    if (prefix == NULL) {
        fprintf(stderr, "Failure at %s\n", parser->lexer->input);
        fprintf(stderr, "Failure while parsing: %d.%s\n", parser->curr_token->type, parser->curr_token->literal);
        fprintf(stderr, "Prefix is null\n");
        return NULL;
    }
    expression_t* left_expression = prefix(parser);

    while (!is_peek_token(parser, SEMICOLON) && precedence < peek_precedence(parser)) {
        prefix_parse_function infix = infix_parse_functions[parser->peek_token->type];
        if (infix == NULL) {
            return left_expression;
        }
        next_token_parser(parser);
        left_expression = infix(parser, left_expression);
    }
    return left_expression;
}

expression_t* parse_grouped_expression(parser_t* parser)
{
    expression_t* expression;
    next_token_parser(parser);
    expression = parse_expression(parser, LOWEST);
    if (!is_peek_token(parser, RPAREN)) {
        assert(false);
        return NULL;
    }
    next_token_parser(parser);
    return expression;
}

statement_t* parse_expression_statement(parser_t* parser)
{
    expression_statement_t* stmt = (expression_statement_t*)malloc(sizeof(expression_statement_t));
    // stmt->token = parser->curr_token->type;
    stmt->expression = parse_expression(parser, LOWEST);
    stmt->statement.node.token_literal = expression_token_literal;
    stmt->statement.node.string = print_expression_statement_string;
    if (is_peek_token(parser, SEMICOLON)) {
        next_token_parser(parser);
    }
    return (statement_t*)stmt;
}

char* identifier_string(void* identifier_expression_cast_to_void)
{
    return ((identifier_t*)identifier_expression_cast_to_void)->value;
}

expression_t* parse_identifier(parser_t* parser)
{
    identifier_t* identifier = (identifier_t*)malloc(sizeof(expression_statement_t));
    identifier->expression.type = IDENTIFIER;
    identifier->value = parser->curr_token->literal;
    identifier->expression.node.string = identifier_string;
    return (expression_t*)identifier;
}

char* integer_literal_string(void* integer_expression_cast_to_void)
{
    integer_t* integer = (integer_t*)integer_expression_cast_to_void;
    char* integer_literal_string = (char*)malloc(STRING_MAX_SIZE);
    sprintf(integer_literal_string, "%lld",
        integer->value);
    return integer_literal_string;
}


char* get_if_expression_string(void* if_expression_cast_to_void)
{
    if_expression_t* if_expression = malloc(sizeof(if_expression_t));
    char* if_expression_string = (char*)malloc(STRING_MAX_SIZE);
    if_expression_string[0] = '\0';
    int i;
    sprintf(if_expression_string, "if %s %s", if_expression->condition->node.string((void*)if_expression->condition), if_expression->consequence->statement.node.string((void*)if_expression->consequence));
    if (if_expression->alternative != NULL) {
        strcat(if_expression_string, "else ");
        strcat(if_expression_string, if_expression->alternative->statement.node.string((void*)if_expression->alternative));
    }
    return if_expression_string;
}

char* get_boolean_expression_string(void* boolean_expression_cast_to_void)
{
    boolean_expression_t* boolean_expression = (boolean_expression_t*)boolean_expression_cast_to_void;
    char* boolean_string = (char*)malloc(STRING_MAX_SIZE);
    sprintf(boolean_string, "%s", boolean_expression->value ? "true" : "false");
    return boolean_string;
}

char* get_call_expressibon_string(void* call_expression_cast_to_void){
  call_expression_t* call_expression = (call_expression_t*)call_expression_cast_to_void;
  char* call_string = (char*)malloc(STRING_MAX_SIZE);
  sprintf(call_string,"%s(%s)",call_expression->function->node.string((void*)call_expression->function),get_concated_arguments_string(call_expression->arguments, call_expression->arguments_length));
  return call_string;
}

expression_t* parse_boolean_expression(parser_t* parser)
{
    boolean_expression_t* expression = (boolean_expression_t*)malloc(sizeof(expression_t));
    expression->token->type = parser->curr_token->type;
    bool is_true = strcmp(parser->curr_token->literal, "true") == 0;
    bool is_false = strcmp(parser->curr_token->literal, "false") == 0;
    if (!is_true && !is_false) {
        fprintf(stderr, "token literal must be a boolean");
        exit(-1);
    }
    expression->value = is_true ? true : false;
    expression->expression.node.string = get_boolean_expression_string;
    return (expression_t*)expression;
}

expression_t* parse_integer_literal(parser_t* parser)
{
    integer_t* expression = (integer_t*)malloc(sizeof(expression_statement_t));
    expression->token->type = parser->curr_token->type;
    expression->value = atoi(parser->curr_token->literal);
    expression->expression.node.string = integer_literal_string;
    return (expression_t*)expression;
}

statement_t* parse_statement(parser_t* parser)
{
    switch (parser->curr_token->type) {
    case LET:
        return parse_let_statement(parser);
    case RETURN:
        return parse_return_statement(parser);
    default:
        return parse_expression_statement(parser);
    }
}

statement_t* parse_block_statement(parser_t* parser)
{
    // setup the block_statement
    block_statement_t* block_statement = malloc(sizeof(block_statement_t));
    block_statement->statements_length = 0;
    block_statement->statements_capacity = DEFAULT_DYNAMIC_ARR_SIZE;
    block_statement->statements = calloc(DEFAULT_DYNAMIC_ARR_SIZE, sizeof(statement_t));
    int i;
    for (i = 0; i < block_statement->statements_length; ++i) {
        block_statement->statements[i] = malloc(sizeof(statement_t));
    }

    // actual parsing

    if (!is_curr_token(parser, RBRACE) && !is_curr_token(parser, END_OF_FILE)) {
        statement_t* statement = parse_statement(parser);
        if (statement != NULL) {
            push_statements_block_statement(block_statement, statement);
        }
        next_token_parser(parser);
    }
    return (statement_t*)block_statement;
}

expression_t* parse_if_expression(parser_t* parser)
{
    if_expression_t* if_expression = malloc(sizeof(if_expression_t));
    if (!is_peek_token(parser, LPAREN)) {
        fprintf(stderr, "Expected ( got %s", parser->peek_token->literal);
        exit(-1);
    }
    next_token_parser(parser);
    next_token_parser(parser);
    if_expression->condition = parse_expression(parser, LOWEST);
    if (!is_peek_token(parser, RPAREN)) {
        fprintf(stderr, "Expected ) got %s", parser->peek_token->literal);
        exit(-1);
    }
    next_token_parser(parser);
    if (!is_peek_token(parser, LBRACE)) {
        fprintf(stderr, "Expected { got %s", parser->peek_token->literal);
        exit(-1);
    }
    next_token_parser(parser);
    next_token_parser(parser);
    if_expression->consequence = (block_statement_t*)parse_block_statement(parser);
    if (is_peek_token(parser, ELSE)) {
        next_token_parser(parser);
        if (is_peek_token(parser, LBRACE)) {
            next_token_parser(parser);
            next_token_parser(parser);
            if_expression->alternative = (block_statement_t*)parse_block_statement(parser);
        }
    }
    return (expression_t*)if_expression;
}


void push_to_arguments_array(function_literal_t* function_expression,expression_t* expression){
    if (function_expression->arguments_length + 1 > function_expression->arguments_capacity) {
        function_expression->arguments = realloc(function_expression->arguments, function_expression->arguments_capacity * 2);
        printf("Realloced");
    }
    function_expression->arguments[function_expression->arguments_length++] = expression;
}

void parse_function_parameters(parser_t* parser,function_literal_t* function_expression){
  //creating the expression array
  function_expression->arguments = (expression_t**)calloc(DEFAULT_DYNAMIC_ARR_SIZE,sizeof(expression_t));
  function_expression->arguments_length = 0;
  function_expression->arguments_capacity = DEFAULT_DYNAMIC_ARR_SIZE;
  // parsing arguments
  while(!is_curr_token(parser,RPAREN)){
    if(!is_curr_token(parser,IDENT)){
      fprintf(stderr,"Expected identifier argument here got %s",token_strings[parser->curr_token->type]);
      exit(-1);
    }
    expression_t* curr_identifier = parse_identifier(parser);
    push_to_arguments_array(function_expression,curr_identifier);
    if(is_peek_token(parser,RPAREN)){
      next_token_parser(parser);
      break;
    }
    if(!(is_peek_token(parser,COMMA))){
      fprintf(stderr,"Expected comma here, got %s",token_strings[parser->peek_token->type]);
    }
    next_token_parser(parser);
    next_token_parser(parser);
  }
}

expression_t* parse_function_expression(parser_t* parser){
  function_literal_t* function_expression = malloc(sizeof(function_literal_t));
  if(!is_peek_token(parser,LPAREN)){
    fprintf(stderr,"Expected ( got %s",token_strings[parser->peek_token->type]);
    exit(-1);
  }
  next_token_parser(parser);
  next_token_parser(parser);
  parse_function_parameters(parser,function_expression);
  if(!is_peek_token(parser,LBRACE)){
    fprintf(stderr,"Expected { got %s",token_strings[parser->peek_token->type]);
    exit(-1);
  }
  next_token_parser(parser);
  next_token_parser(parser);
  function_expression->body = (block_statement_t*)parse_block_statement(parser);
  return (expression_t*)function_expression;
}


expression_t* parse_prefix_expression(parser_t* parser)
{
    prefix_expression_t* expression = (prefix_expression_t*)malloc(sizeof(expression_statement_t));
    expression->expression.type = PREFIX_EXPRESSION;
    expression->op = parser->curr_token->literal;
    next_token_parser(parser);
    precedence_t precedence = peek_precedence(parser);
    expression->right = parse_expression(parser, PREFIX);
    expression->expression.node.string = print_prefix_expression_string;
    return (expression_t*)expression;
}

expression_t* parse_infix_function(parser_t* parser, expression_t* left)
{
    infix_expression_t* infix_expression = (infix_expression_t*)malloc(sizeof(infix_expression_t));
    if (infix_expression == NULL) {
        fprintf(stderr, "Failed to allocated %zu bytes", sizeof(expression_t));
        exit(-1);
    }
    infix_expression->expression.type = INFIX_EXPRESSION;
    infix_expression->op = parser->curr_token->literal;
    infix_expression->left = left;
    precedence_t precedence = curr_precedence(parser);
    next_token_parser(parser);
    infix_expression->right = parse_expression(parser, precedence);
    infix_expression->expression.node.string = print_infix_expression_string;
    return (expression_t*)infix_expression;
}

program_t* parse_program(parser_t* parser)
{
    program_t* program_node = new_program_node();
    while (parser->curr_token->type != END_OF_FILE) {
        statement_t* stmt = parse_statement(parser);
        // TODO: there is a null check here, the api should be such that this doesn
        // t happen
        if (stmt != NULL) {
            push_statements_program(program_node, stmt);
        }
        next_token_parser(parser);
    }
    return program_node;
}
