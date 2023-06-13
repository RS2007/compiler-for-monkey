#include "../src/evaluator.h"
#include "../src/object.h"
#include "../src/parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static int test_integer_object(object_t *object, long long expected) {
  if (object->type() != INTEGER) {
    fprintf(stderr, "Object is not of type INTEGER");
    exit(-1);
  }
  integer_obj_t *int_obj = (integer_obj_t *)object;
  if (int_obj->value != expected) {
    fprintf(stderr, "Expected %lld, Got %lld", expected, int_obj->value);
    exit(-1);
  }
  return 1;
}

static int test_boolean_object(object_t *object, bool expected) {
  if (object->type() != BOOLEAN) {
    fprintf(stderr, "Object is not of type BOOLEAN");
    exit(-1);
  }
  boolean_obj_t *bool_obj = (boolean_obj_t *)object;
  if (bool_obj->value != expected) {
    fprintf(stderr, "Expected %d got %d", expected, bool_obj->value);
    exit(-1);
  }
  return 1;
}

static object_t *test_eval(char *string) {
  lexer_t *lexer = new_lexer(string, strlen(string));
  parser_t *parser = new_parser(lexer);
  program_t *program_node = parse_program(parser);
  environment_t *env = new_environment();
  return eval(program_node, env);
}

static int test_eval_integer_expression() {
  typedef struct test_t {
    char *input;
    long long expected;
  } test_t;
  test_t tests[] = {{"5", 5}, {"10", 10}};
  size_t tests_size = sizeof(tests) / sizeof(tests[0]);
  int i;
  for (i = 0; i < tests_size; ++i) {
    test_t curr_test = tests[i];
    object_t *evaluated = test_eval(curr_test.input);
    if (!test_integer_object(evaluated, curr_test.expected)) {
      fprintf(stderr, "Test failed");
    }
  }
  fprintf(stdout, "All test cases passed ✅");
  return 0;
}

static int test_eval_boolean_expression() {
  typedef struct test_t {
    char *input;
    bool expected;
  } test_t;
  test_t tests[] = {{"true", true}, {"false", false}};
  size_t tests_size = sizeof(tests) / sizeof(tests[0]);
  for (int i = 0; i < tests_size; ++i) {
    test_t curr_test = tests[i];
    object_t *evaluated = test_eval(curr_test.input);
    if (!test_boolean_object(evaluated, curr_test.expected)) {
      fprintf(stderr, "Test failed");
    }
  }
  fprintf(stdout, "All test cases passed ✅");
  return 0;
}

static int test_bang_operator() {
  typedef struct test_t {
    char *input;
    bool expected;
  } test_t;
  test_t tests[] = {{"!true", false},
                    {"!false", true},
                    {"!5", false},
                    {"!!false", false},
                    {"!!5", true}};
  size_t tests_size = sizeof(tests) / sizeof(tests[0]);
  for (int i = 0; i < tests_size; ++i) {
    test_t curr_test = tests[i];
    object_t *evaluated = test_eval(curr_test.input);
    if (!test_boolean_object(evaluated, curr_test.expected)) {
      fprintf(stderr, "Test failed");
      exit(-1);
    }
  }
  fprintf(stdout, "All test cases passed ✅");
  return 0;
}

static int test_prefix_minus_operator() {
  typedef struct test_t {
    char *input;
    int value;
  } test_t;
  test_t tests[] = {{"5", 5}, {"10", 10}, {"-5", -5}, {"-10", -10}};
  size_t tests_size = sizeof(tests) / sizeof(tests[0]);
  for (int i = 0; i < tests_size; ++i) {
    test_t curr_test = tests[i];
    object_t *evaluated = test_eval(curr_test.input);
    if (((integer_obj_t *)evaluated)->value != curr_test.value) {
      fprintf(stderr, "Test failed");
      exit(-1);
    }
  }
  fprintf(stdout, "All test cases passed ✅");
  return 0;
}

static int test_integer_infix_expressions() {
  typedef struct test_t {
    char *input;
    int value;
  } test_t;
  test_t tests[] = {{"5", 5},
                    {"10", 10},
                    {"-5", -5},
                    {"-10", -10},
                    {"5 + 5 + 5 + 5 - 10", 10},
                    {"2 * 2 * 2 * 2 * 2", 32},
                    {"-50 + 100 + -50", 0},
                    {"5 * 2 + 10", 20},
                    {"5 + 2 * 10", 25},
                    {"20 + 2 * -10", 0},
                    {"50 / 2 * 2 + 10", 60},
                    {"2 * (5 + 10)", 30},
                    {"3 * 3 * 3 + 10", 37},
                    {"3 * (3 * 3) + 10", 37},
                    {"(5 + 10 * 2 + 15 / 3) * 2 + -10", 50}};
  size_t tests_size = sizeof(tests) / sizeof(tests[0]);
  for (int i = 0; i < tests_size; ++i) {
    test_t curr_test = tests[i];
    object_t *evaluated = test_eval(curr_test.input);
    if (((integer_obj_t *)evaluated)->value != curr_test.value) {
      fprintf(stderr, "Test failed");
      exit(-1);
    }
  }
  fprintf(stdout, "All test cases passed ✅");
  return 0;
}

static int test_bool_infix_expressions() {
  typedef struct test_t {
    char *input;
    bool value;
  } test_t;
  test_t tests[] = {
      {"true", true},
      {"false", false},
      {"1 < 2", true},
      {"1 > 2", false},
      {"1 < 1", false},
      {"1 > 1", false},
      {"1 == 1", true},
      {"1 != 1", false},
      {"1 == 2", false},
      {"1 != 2", true},
      {"true == true", true},
      {"false == false", true},
      {"true == false", false},
      {"true != false", true},
      {"false != true", true},
      {"(1 < 2) == true", true},
      {"(1 < 2) == false", false},
      {"(1 > 2) == true", false},
      {"(1 > 2) == false", true},
  };
  size_t tests_size = sizeof(tests) / sizeof(tests[0]);
  for (int i = 0; i < tests_size; ++i) {
    test_t curr_test = tests[i];
    object_t *evaluated = test_eval(curr_test.input);
    if (((integer_obj_t *)evaluated)->value != curr_test.value) {
      fprintf(stderr, "Test failed");
      exit(-1);
    }
  }
  fprintf(stdout, "All test cases passed ✅");
  return 0;
}

int nested_if_with_return() {
  typedef struct test_t {
    char *input;
    long long expected;
  } test_t;
  test_t tests[] = {{"if(10 > 1){"
                     "if(10 > 1){"
                     "return 10;"
                     "}"
                     "return 1;"
                     "}",
                     10}};

  int tests_size = sizeof(tests) / sizeof(tests[0]);
  for (int i = 0; i < tests_size; ++i) {
    object_t *evaluated = test_eval(tests[i].input);
    if (((integer_obj_t *)evaluated)->value != tests[i].expected) {
      fprintf(stderr, "Test failed");
      exit(-1);
    }
  }
  fprintf(stdout, "All test cases passed ✅");
  return 0;
}

int test_function_expression() {
  char *input = "fn(x) { x + 2;}";
  environment_t *env = (environment_t *)malloc(sizeof(environment_t));
  object_t *evaluated = test_eval(input);
  if (evaluated->type() != FUNCTION_OBJ) {
    fprintf(stderr, "Expected function got some trash: %s",
            object_type_strings[evaluated->type()]);
    exit(-1);
  }
  function_obj_t *func_obj = (function_obj_t *)evaluated;
  if (func_obj->parameters_length != 1) {
    fprintf(stderr, "Expected %zu parameters, got %d",
            func_obj->parameters_length, 1);
    exit(-1);
  }
  if (strcmp(
          func_obj->parameters[0]->node.string((void *)func_obj->parameters[0]),
          "x") != 0) {
    fprintf(
        stderr, "Expected %s parameter, got %s",
        func_obj->parameters[0]->node.string((void *)func_obj->parameters[0]),
        "x");
    exit(-1);
  }
  if (strcmp(func_obj->body->statement.node.string((void *)func_obj->body),
             "(x + 2);") != 0) {
    fprintf(stderr, "Expected %s parameter, got %s",
            func_obj->body->statement.node.string((void *)func_obj->body),
            "(x + 2)");
    exit(-1);
  }
  fprintf(stdout, "All test cases passed ✅");
  return 0;
}

int test_closures(void){
  char* input = "let newAdder = fn(x) {"
                " return fn(y) { x+y; }"
                "};"
                "let addTwo = newAdder(2);"
                "addTwo(2);";
  object_t* res = test_eval(input);
  if(res->type == ERROR_OBJ){
    fprintf(stderr,"ERROR: %s",((error_obj_t*)res)->message);
    exit(-1);
  }
  if(res->type() != INTEGER_LITERAL){
    fprintf(stderr,"Expected integer got, %s",object_type_strings[res->type()]);
    exit(-1);
  }
  integer_obj_t* integer = (integer_obj_t*)res;
  if(integer->value != 4){
    fprintf(stderr,"Expected %lld, got %lld",4,integer->value);
    exit(-1);
  }
  fprintf(stdout, "All test cases passed ✅");
  return 0;
}

int main(void) {
  test_eval_integer_expression();
  test_eval_boolean_expression();
  test_bang_operator();
  test_prefix_minus_operator();
  test_integer_infix_expressions();
  test_bool_infix_expressions();
  test_function_expression();
  nested_if_with_return();
  return test_closures();
}
