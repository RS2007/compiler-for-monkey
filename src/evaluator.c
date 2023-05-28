#include "nodes.h"
#include "object.h"
#include "utils.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>

object_t* eval(node_t*);
object_t* eval_expression(expression_t*);
object_t* eval_statement(statement_t*);

static integer_obj_t* new_integer_obj(long long value)
{
    integer_obj_t* integer_obj = (integer_obj_t*)malloc(sizeof(integer_obj_t));
    integer_obj->value = value;
    integer_obj->object.type = type_int;
    integer_obj->object.inspect = inspect_int;
    return integer_obj;
}

object_t* eval_if_expression(if_expression_t* if_expr)
{
    object_t* condition_obj = eval_expression(if_expr->condition);
    if (condition_obj->type() != BOOLEAN) {
        assert("Condition should evaluate to a boolean");
    }
    bool is_condition_true = ((boolean_obj_t*)condition_obj)->value;

    if (is_condition_true) {
        object_t* eval_consequence = eval_statement(if_expr->consequence);
        return eval_consequence;
    } else {
        if (if_expr->alternative == NULL) {
            null_obj_t* null_obj = (null_obj_t*)malloc(sizeof(null_obj));
            null_obj->object.type = type_null;
            null_obj->object.inspect = inspect_null;
            return (object_t*)null_obj;
        }
        object_t* eval_alternative = eval_statement(if_expr->alternative);
        return eval_alternative;
    }
    return NULL;
}

object_t* eval_infix_expression(infix_expression_t* infix_expr)
{
    if (strcmp(infix_expr->op, "+") == 0) {
        object_t* left_obj = eval_expression(infix_expr->left);
        object_t* right_obj = eval_expression(infix_expr->right);
        if (left_obj->type() != INTEGER || right_obj->type() != INTEGER) {
            fprintf(stderr, "Monkey only supports addition of integers");
            exit(-1);
        }
        integer_obj_t* left_int = (integer_obj_t*)left_obj;
        integer_obj_t* right_int = (integer_obj_t*)right_obj;
        integer_obj_t* return_obj = (integer_obj_t*)malloc(sizeof(integer_obj_t));
        return_obj->value = left_int->value + right_int->value;
        return_obj->object.type = type_int;
        return_obj->object.inspect = inspect_int;
        return (object_t*)return_obj;
    } else if (strcmp(infix_expr->op, "-") == 0) {
        object_t* left_obj = eval_expression(infix_expr->left);
        object_t* right_obj = eval_expression(infix_expr->right);
        if (left_obj->type() != INTEGER || right_obj->type() != INTEGER) {
            fprintf(stderr, "Monkey only supports addition of integers");
            exit(-1);
        }
        integer_obj_t* left_int = (integer_obj_t*)left_obj;
        integer_obj_t* right_int = (integer_obj_t*)right_obj;
        integer_obj_t* return_obj = (integer_obj_t*)malloc(sizeof(integer_obj_t));
        return_obj->value = left_int->value - right_int->value;
        return_obj->object.type = type_int;
        return_obj->object.inspect = inspect_int;
        return (object_t*)return_obj;
    } else if (strcmp(infix_expr->op, "*") == 0) {
        object_t* left_obj = eval_expression(infix_expr->left);
        object_t* right_obj = eval_expression(infix_expr->right);
        if (left_obj->type() != INTEGER || right_obj->type() != INTEGER) {
            fprintf(stderr, "Monkey only supports addition of integers");
            exit(-1);
        }
        integer_obj_t* left_int = (integer_obj_t*)left_obj;
        integer_obj_t* right_int = (integer_obj_t*)right_obj;
        integer_obj_t* return_obj = (integer_obj_t*)malloc(sizeof(integer_obj_t));
        return_obj->value = left_int->value * right_int->value;
        return_obj->object.type = type_int;
        return_obj->object.inspect = inspect_int;
        return (object_t*)return_obj;
    } else if (strcmp(infix_expr->op, "/") == 0) {
        object_t* left_obj = eval_expression(infix_expr->left);
        object_t* right_obj = eval_expression(infix_expr->right);
        if (left_obj->type() != INTEGER || right_obj->type() != INTEGER) {
            fprintf(stderr, "Monkey only supports addition of integers");
            exit(-1);
        }
        integer_obj_t* left_int = (integer_obj_t*)left_obj;
        integer_obj_t* right_int = (integer_obj_t*)right_obj;
        integer_obj_t* return_obj = (integer_obj_t*)malloc(sizeof(integer_obj_t));
        return_obj->value = left_int->value / right_int->value;
        return_obj->object.type = type_int;
        return_obj->object.inspect = inspect_int;
        return (object_t*)return_obj;
    } else if (strcmp(infix_expr->op, "==") == 0) {
        object_t* left_obj = eval_expression(infix_expr->left);
        object_t* right_obj = eval_expression(infix_expr->right);
        bool are_both_ints = left_obj->type() == INTEGER && right_obj->type() == INTEGER;
        bool are_both_bools = right_obj->type() == BOOLEAN && right_obj->type() == BOOLEAN;
        if (!are_both_ints && !are_both_bools) {
            fprintf(stderr, "Incompatible types for comparision");
            exit(-1);
        }
        boolean_obj_t* bool_obj = (boolean_obj_t*)malloc(sizeof(boolean_obj_t));
        if (are_both_ints) {
            integer_obj_t* left_num = (integer_obj_t*)left_obj;
            integer_obj_t* right_num = (integer_obj_t*)right_obj;
            bool_obj->value = (left_num->value == right_num->value);
        } else {
            boolean_obj_t* left_bool = (boolean_obj_t*)left_obj;
            boolean_obj_t* right_bool = (boolean_obj_t*)right_obj;
            bool_obj->value = (left_bool->value == right_bool->value);
        }
        bool_obj->object.type = type_boolean;
        bool_obj->object.inspect = inspect_boolean;
        return (object_t*)bool_obj;
    } else if (strcmp(infix_expr->op, "!=") == 0) {
        object_t* left_obj = eval_expression(infix_expr->left);
        object_t* right_obj = eval_expression(infix_expr->right);
        bool are_both_ints = left_obj->type() == INTEGER && right_obj->type() == INTEGER;
        bool are_both_bools = right_obj->type() == BOOLEAN && right_obj->type() == BOOLEAN;
        if (!are_both_ints && !are_both_bools) {
            fprintf(stderr, "Incompatible types for comparision");
            exit(-1);
        }
        boolean_obj_t* bool_obj = (boolean_obj_t*)malloc(sizeof(boolean_obj_t));
        if (are_both_ints) {
            integer_obj_t* left_num = (integer_obj_t*)left_obj;
            integer_obj_t* right_num = (integer_obj_t*)right_obj;
            bool_obj->value = (left_num->value != right_num->value);
        } else {
            boolean_obj_t* left_bool = (boolean_obj_t*)left_obj;
            boolean_obj_t* right_bool = (boolean_obj_t*)right_obj;
            bool_obj->value = (left_bool->value != right_bool->value);
        }
        bool_obj->object.type = type_boolean;
        bool_obj->object.inspect = inspect_boolean;
        return (object_t*)bool_obj;
    } else if (strcmp(infix_expr->op, ">") == 0) {
        object_t* left_obj = eval_expression(infix_expr->left);
        object_t* right_obj = eval_expression(infix_expr->right);
        boolean_obj_t* bool_obj = (boolean_obj_t*)malloc(sizeof(boolean_obj_t));
        bool are_both_ints = (left_obj->type() == INTEGER && right_obj->type() == INTEGER);
        if (!are_both_ints) {
            fprintf(stderr, "Cannot compare non integers");
            exit(-1);
        }
        bool_obj->value = ((integer_obj_t*)left_obj)->value > ((integer_obj_t*)right_obj)->value;
        bool_obj->object.type = type_boolean;
        bool_obj->object.inspect = inspect_boolean;
        return (object_t*)bool_obj;
    } else if (strcmp(infix_expr->op, "<") == 0) {
        object_t* left_obj = eval_expression(infix_expr->left);
        object_t* right_obj = eval_expression(infix_expr->right);
        boolean_obj_t* bool_obj = (boolean_obj_t*)malloc(sizeof(boolean_obj_t));
        bool are_both_ints = left_obj->type() == INTEGER && right_obj->type() == INTEGER;
        if (!are_both_ints) {
            fprintf(stderr, "Cannot compare non integers");
            exit(-1);
        }
        bool_obj->value = (((integer_obj_t*)left_obj)->value < ((integer_obj_t*)right_obj)->value);
        bool_obj->object.type = type_boolean;
        bool_obj->object.inspect = inspect_boolean;
        return (object_t*)bool_obj;
    }
    return NULL;
}

object_t* eval_prefix_expression(prefix_expression_t* prefix_expr)
{
    object_t* right = eval_expression(prefix_expr->right);
    if (strcmp(prefix_expr->op, "!") == 0) {
        //TODO: refactor: make into seperate functions
        boolean_obj_t* boolean_obj = (boolean_obj_t*)malloc(sizeof(boolean_obj_t));
        switch (right->type()) {
        case BOOLEAN: {
            boolean_obj_t* boolean_obj_right = (boolean_obj_t*)right;
            boolean_obj->value = boolean_obj_right->value == true ? false : true;
            break;
        }
        case NULL_OBJ: {
            boolean_obj->value = true;
            break;
        }
        case INTEGER: {
            integer_obj_t* integer_obj = (integer_obj_t*)right;
            boolean_obj->value = integer_obj->value == 0 ? true : false;
            break;
        }
        default: {
            boolean_obj->value = false;
            break;
        }
        }
        boolean_obj->object.type = type_boolean;
        boolean_obj->object.inspect = inspect_boolean;
        return (object_t*)boolean_obj;
    } else if (strcmp(prefix_expr->op, "-") == 0) {
        if (right->type() != INTEGER) {
            fprintf(stderr, "- should preceed only an integer literal got %s", object_type_strings[right->type()]);
            exit(-1);
        }
        integer_obj_t* integer_obj = (integer_obj_t*)right;
        integer_obj_t* return_integer_obj = (integer_obj_t*)malloc(sizeof(integer_obj_t));
        // TODO: create a function to intialize integer objects
        return_integer_obj->value = -integer_obj->value;
        return_integer_obj->object.type = type_int;
        return_integer_obj->object.inspect = inspect_int;
        return (object_t*)return_integer_obj;
    } else {
        assert("Should not be here");
    }
    return NULL;
}

object_t* eval_expression(expression_t* expression)
{
    switch (expression->type) {
    case INTEGER_LITERAL: {
        integer_t* integer_expr = (integer_t*)expression;
        integer_obj_t* integer_obj = (integer_obj_t*)malloc(sizeof(integer_obj_t));
        integer_obj->value = integer_expr->value;
        integer_obj->object.type = type_int;
        integer_obj->object.inspect = inspect_int;
        return (object_t*)integer_obj;
    }
    case IDENTIFIER: {
        identifier_t* identifier = (identifier_t*)expression;
        if (strcmp(identifier->value, "null") == 0) {
            null_obj_t* null_obj = (null_obj_t*)malloc(sizeof(null_obj_t));
            null_obj->object.type = type_null;
            null_obj->object.inspect = inspect_null;
            return (object_t*)null_obj;
        }
    }
    case BOOLEAN_LITERAL: {
        boolean_expression_t* boolean_expression = (boolean_expression_t*)expression;
        boolean_obj_t* boolean_obj = (boolean_obj_t*)malloc(sizeof(boolean_obj_t));
        boolean_obj->value = boolean_expression->value;
        boolean_obj->object.type = type_boolean;
        boolean_obj->object.inspect = inspect_boolean;
        return (object_t*)boolean_obj;
    }
    case PREFIX_EXPRESSION: {
        prefix_expression_t* prefix_expr = (prefix_expression_t*)expression;
        return eval_prefix_expression(prefix_expr);
    }
    case INFIX_EXPRESSION: {
        infix_expression_t* infix_expression = (infix_expression_t*)expression;
        return eval_infix_expression(infix_expression);
    }
    case IF_EXPRESSION: {
        if_expression_t* if_expression = (if_expression_t*)expression;
        return eval_if_expression(if_expression);
    }
    case FUNCTION_EXPRESSION: {
        unimplemented();
    }
    case CALL_EXPRESSION: {
        unimplemented();
    }
    }
    return NULL;
}

object_t* eval_block_statement(block_statement_t* block_statement){
  object_t* result;
  for(int i = 0;i< block_statement->statements_length;++i){
    result = eval_statement(block_statement->statements[i]);
    if(result != NULL && result->type() == RETURN_VALUE_OBJ){
      return result;
    }
  }
  return result;
}

object_t* eval_statements(statement_t** statements, size_t statements_length)
{
    object_t* result;
    for (int i = 0; i < statements_length; ++i) {
        result = eval_statement(statements[i]);
    }
    return result;
}
object_t* eval_statement(statement_t* statement)
{
    switch (statement->type) {
    case LET_STATEMENT: {
        unimplemented();
    }
    case RETURN_STATEMENT: {
      ret_statement_t* ret_stmt = (ret_statement_t*)statement;
      return_obj_t* ret_obj = (return_obj_t*)malloc(sizeof(return_obj_t));
      ret_obj->value = eval_expression(ret_stmt->return_value);
      ret_obj->object.type = type_return;
      ret_obj->object.inspect = inspect_return;
      return (object_t*)ret_obj;

    }
    case EXPRESSION_STATEMENT: {
        expression_statement_t* expr = (expression_statement_t*)statement;
        return eval_expression(expr->expression);
    }
    case BLOCK_STATEMENT: {
        block_statement_t* block = (block_statement_t*)statement;
        return eval_statements(block->statements, block->statements_length);
    }
    }
    return NULL;
}

object_t* eval_program(program_t* program)
{
    object_t* result;
    for(int i = 0;i < program->statements_length;++i){
      statement_t* stmt = program->statements[i];
      result = eval_statement(stmt);
    
      if(result->type() == RETURN_VALUE_OBJ){
        return ((return_obj_t*)result)->value;
      }
    }
    return result;
}

object_t* eval(node_t* node)
{
    switch (node->type) {
    case PROGRAM: {
        program_t* program_node = (program_t*)node;
        return eval_program(program_node);
    }
    case STATEMENT: {
        statement_t* statement = (statement_t*)node;
        return eval_statement(statement);
    }
    case EXPRESSION: {
        expression_t* expression = (expression_t*)node;
        return eval_expression(expression);
    }
    }
    return NULL;
}
