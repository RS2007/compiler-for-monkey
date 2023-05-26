#include "../src/evaluator.h"
#include "../src/object.h"
#include "../src/parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static int test_integer_object(object_t* object, long long expected)
{
    if (object->type() != INTEGER) {
        fprintf(stderr, "Object is not of type INTEGER");
        exit(-1);
    }
    integer_obj_t* int_obj = (integer_obj_t*)object;
    if (int_obj->value != expected) {
        fprintf(stderr, "Expected %lld, Got %lld", expected, int_obj->value);
        exit(-1);
    }
    return 1;
}


static int test_boolean_object(object_t* object,bool expected){
  if(object->type() != BOOLEAN){
    fprintf(stderr,"Object is not of type BOOLEAN");
    exit(-1);
  }
  boolean_obj_t* bool_obj = (boolean_obj_t*)object;
  if(bool_obj->value != expected){
    fprintf(stderr,"Expected %d got %d",expected,bool_obj->value);
    exit(-1);
  }
  return 1;
}

static object_t* test_eval(char* string)
{
    lexer_t* lexer = new_lexer(string, strlen(string));
    parser_t* parser = new_parser(lexer);
    program_t* program_node = parse_program(parser);
    return eval(program_node);
}

static int test_eval_integer_expression()
{
    typedef struct test_t {
        char* input;
        long long expected;
    } test_t;
    test_t tests[] = { { "5", 5 }, { "10", 10 } };
    size_t tests_size = sizeof(tests) / sizeof(tests[0]);
    int i;
    for (i = 0; i < tests_size; ++i) {
        test_t curr_test = tests[i];
        object_t* evaluated = test_eval(curr_test.input);
        if(!test_integer_object(evaluated,curr_test.expected)){
          fprintf(stderr,"Test failed");
        }
    }
    fprintf(stdout, "All test cases passed ✅");
    return 0;
}

static int test_eval_boolean_expression(){
  typedef struct test_t {
    char* input;
    bool expected;
  } test_t;
  test_t tests[] = {{"true",true},{"false",false}};
  size_t tests_size = sizeof(tests)/sizeof(tests[0]);
  for(int i = 0;i<tests_size;++i){
    test_t curr_test = tests[i];
    object_t* evaluated = test_eval(curr_test.input);
    if(!test_boolean_object(evaluated, curr_test.expected)){
      fprintf(stderr,"Test failed");
    }
  }
  fprintf(stdout, "All test cases passed ✅");
  return 0;
}


static int test_bang_operator(){
  typedef struct test_t {
    char* input;
    bool expected;
  } test_t;
  test_t tests[] = {{"!true",false},{"!false",true},{"!5",false},{"!!false",false},{"!!5",true}};
  size_t tests_size = sizeof(tests)/sizeof(tests[0]);
  for(int i = 0;i<tests_size;++i){
    test_t curr_test = tests[i];
    object_t* evaluated = test_eval(curr_test.input);
    if(!test_boolean_object(evaluated,curr_test.expected)){
      fprintf(stderr,"Test failed");
      exit(-1);
    }
  }
  fprintf(stdout,"All test cases passed ✅");
  return 0;
}

int main(void)
{
  test_eval_integer_expression();
  test_eval_boolean_expression();  
  return test_bang_operator();
}
