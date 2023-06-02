#include "environment.h"
#include "nodes.h"
#include "object.h"
#include "utils.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>

object_t *eval(node_t *, environment_t *);
object_t *eval_expression(expression_t *, environment_t *);
object_t *eval_statement(statement_t *, environment_t *);

error_obj_t *create_error_object() {
  error_obj_t *error_obj = (error_obj_t *)malloc(sizeof(error_obj_t));
  error_obj->object.type = type_error;
  error_obj->object.inspect = inspect_error;
  return error_obj;
}

static integer_obj_t *new_integer_obj(long long value) {
  integer_obj_t *integer_obj = (integer_obj_t *)malloc(sizeof(integer_obj_t));
  integer_obj->value = value;
  integer_obj->object.type = type_int;
  integer_obj->object.inspect = inspect_int;
  return integer_obj;
}

object_t *eval_if_expression(if_expression_t *if_expr, environment_t *env) {
  object_t *condition_obj = eval_expression(if_expr->condition, env);
  if (condition_obj->type() != BOOLEAN) {
    assert("Condition should evaluate to a boolean");
  }
  bool is_condition_true = ((boolean_obj_t *)condition_obj)->value;

  if (is_condition_true) {
    object_t *eval_consequence = eval_statement(if_expr->consequence, env);
    return eval_consequence;
  } else {
    if (if_expr->alternative == NULL) {
      null_obj_t *null_obj = (null_obj_t *)malloc(sizeof(null_obj_t));
      null_obj->object.type = type_null;
      null_obj->object.inspect = inspect_null;
      return (object_t *)null_obj;
    }
    object_t *eval_alternative = eval_statement(if_expr->alternative, env);
    return eval_alternative;
  }
  return NULL;
}

object_t *eval_infix_expression(infix_expression_t *infix_expr,
                                environment_t *env) {
  if (strcmp(infix_expr->op, "+") == 0) {
    object_t *left_obj = eval_expression(infix_expr->left, env);
    object_t *right_obj = eval_expression(infix_expr->right, env);
    if (left_obj->type() != INTEGER || right_obj->type() != INTEGER) {
      error_obj_t *error_obj = create_error_object();
      char *error_string = (char *)malloc(STRING_MAX_SIZE);
      snprintf(error_string, STRING_MAX_SIZE,
               "Monkey only supports addition of integers");
      error_obj->message = error_string;
      return (object_t *)error_obj;
    }
    integer_obj_t *left_int = (integer_obj_t *)left_obj;
    integer_obj_t *right_int = (integer_obj_t *)right_obj;
    integer_obj_t *return_obj = (integer_obj_t *)malloc(sizeof(integer_obj_t));
    return_obj->value = left_int->value + right_int->value;
    return_obj->object.type = type_int;
    return_obj->object.inspect = inspect_int;
    return (object_t *)return_obj;
  } else if (strcmp(infix_expr->op, "-") == 0) {
    object_t *left_obj = eval_expression(infix_expr->left, env);
    object_t *right_obj = eval_expression(infix_expr->right, env);
    if (left_obj->type() != INTEGER || right_obj->type() != INTEGER) {
      error_obj_t *error_obj = create_error_object();
      char *error_string = (char *)malloc(STRING_MAX_SIZE);
      snprintf(error_string, STRING_MAX_SIZE,
               "Monkey only supports subtraction of integers");
      error_obj->message = error_string;
      return (object_t *)error_obj;
    }
    integer_obj_t *left_int = (integer_obj_t *)left_obj;
    integer_obj_t *right_int = (integer_obj_t *)right_obj;
    integer_obj_t *return_obj = (integer_obj_t *)malloc(sizeof(integer_obj_t));
    return_obj->value = left_int->value - right_int->value;
    return_obj->object.type = type_int;
    return_obj->object.inspect = inspect_int;
    return (object_t *)return_obj;
  } else if (strcmp(infix_expr->op, "*") == 0) {
    object_t *left_obj = eval_expression(infix_expr->left, env);
    object_t *right_obj = eval_expression(infix_expr->right, env);
    if (left_obj->type() != INTEGER || right_obj->type() != INTEGER) {
      error_obj_t *error_obj = create_error_object();
      char *error_string = (char *)malloc(STRING_MAX_SIZE);
      snprintf(error_string, STRING_MAX_SIZE,
               "Monkey only supports multiplication of integers");
      error_obj->message = error_string;
      return (object_t *)error_obj;
    }
    integer_obj_t *left_int = (integer_obj_t *)left_obj;
    integer_obj_t *right_int = (integer_obj_t *)right_obj;
    integer_obj_t *return_obj = (integer_obj_t *)malloc(sizeof(integer_obj_t));
    return_obj->value = left_int->value * right_int->value;
    return_obj->object.type = type_int;
    return_obj->object.inspect = inspect_int;
    return (object_t *)return_obj;
  } else if (strcmp(infix_expr->op, "/") == 0) {
    object_t *left_obj = eval_expression(infix_expr->left, env);
    object_t *right_obj = eval_expression(infix_expr->right, env);
    if (left_obj->type() != INTEGER || right_obj->type() != INTEGER) {
      error_obj_t *error_obj = create_error_object();
      char *error_string = (char *)malloc(STRING_MAX_SIZE);
      snprintf(error_string, STRING_MAX_SIZE,
               "Monkey only supports division of integers");
      error_obj->message = error_string;
      return (object_t *)error_obj;
    }
    integer_obj_t *left_int = (integer_obj_t *)left_obj;
    integer_obj_t *right_int = (integer_obj_t *)right_obj;
    integer_obj_t *return_obj = (integer_obj_t *)malloc(sizeof(integer_obj_t));
    return_obj->value = left_int->value / right_int->value;
    return_obj->object.type = type_int;
    return_obj->object.inspect = inspect_int;
    return (object_t *)return_obj;
  } else if (strcmp(infix_expr->op, "==") == 0) {
    object_t *left_obj = eval_expression(infix_expr->left, env);
    object_t *right_obj = eval_expression(infix_expr->right, env);
    bool are_both_ints =
        left_obj->type() == INTEGER && right_obj->type() == INTEGER;
    bool are_both_bools =
        right_obj->type() == BOOLEAN && right_obj->type() == BOOLEAN;
    if (!are_both_ints && !are_both_bools) {
      error_obj_t *error_obj = create_error_object();
      char *error_string = (char *)malloc(STRING_MAX_SIZE);
      snprintf(error_string, STRING_MAX_SIZE,
               "Incompatible types for comparision");
      error_obj->message = error_string;
      return (object_t *)error_obj;
    }
    boolean_obj_t *bool_obj = (boolean_obj_t *)malloc(sizeof(boolean_obj_t));
    if (are_both_ints) {
      integer_obj_t *left_num = (integer_obj_t *)left_obj;
      integer_obj_t *right_num = (integer_obj_t *)right_obj;
      bool_obj->value = (left_num->value == right_num->value);
    } else {
      boolean_obj_t *left_bool = (boolean_obj_t *)left_obj;
      boolean_obj_t *right_bool = (boolean_obj_t *)right_obj;
      bool_obj->value = (left_bool->value == right_bool->value);
    }
    bool_obj->object.type = type_boolean;
    bool_obj->object.inspect = inspect_boolean;
    return (object_t *)bool_obj;
  } else if (strcmp(infix_expr->op, "!=") == 0) {
    object_t *left_obj = eval_expression(infix_expr->left, env);
    object_t *right_obj = eval_expression(infix_expr->right, env);
    bool are_both_ints =
        left_obj->type() == INTEGER && right_obj->type() == INTEGER;
    bool are_both_bools =
        right_obj->type() == BOOLEAN && right_obj->type() == BOOLEAN;
    if (!are_both_ints && !are_both_bools) {
      error_obj_t *error_obj = create_error_object();
      char *error_string = (char *)malloc(STRING_MAX_SIZE);
      snprintf(error_string, STRING_MAX_SIZE,
               "Incompatible types for comparision");
      error_obj->message = error_string;
      return (object_t *)error_obj;
    }
    boolean_obj_t *bool_obj = (boolean_obj_t *)malloc(sizeof(boolean_obj_t));
    if (are_both_ints) {
      integer_obj_t *left_num = (integer_obj_t *)left_obj;
      integer_obj_t *right_num = (integer_obj_t *)right_obj;
      bool_obj->value = (left_num->value != right_num->value);
    } else {
      boolean_obj_t *left_bool = (boolean_obj_t *)left_obj;
      boolean_obj_t *right_bool = (boolean_obj_t *)right_obj;
      bool_obj->value = (left_bool->value != right_bool->value);
    }
    bool_obj->object.type = type_boolean;
    bool_obj->object.inspect = inspect_boolean;
    return (object_t *)bool_obj;
  } else if (strcmp(infix_expr->op, ">") == 0) {
    object_t *left_obj = eval_expression(infix_expr->left, env);
    object_t *right_obj = eval_expression(infix_expr->right, env);
    boolean_obj_t *bool_obj = (boolean_obj_t *)malloc(sizeof(boolean_obj_t));
    bool are_both_ints =
        (left_obj->type() == INTEGER && right_obj->type() == INTEGER);
    if (!are_both_ints) {
      error_obj_t *error_obj = create_error_object();
      char *error_string = (char *)malloc(STRING_MAX_SIZE);
      snprintf(error_string, STRING_MAX_SIZE, "Cannot compare non integers");
      error_obj->message = error_string;
      return (object_t *)error_obj;
    }
    bool_obj->value = ((integer_obj_t *)left_obj)->value >
                      ((integer_obj_t *)right_obj)->value;
    bool_obj->object.type = type_boolean;
    bool_obj->object.inspect = inspect_boolean;
    return (object_t *)bool_obj;
  } else if (strcmp(infix_expr->op, "<") == 0) {
    object_t *left_obj = eval_expression(infix_expr->left, env);
    object_t *right_obj = eval_expression(infix_expr->right, env);
    boolean_obj_t *bool_obj = (boolean_obj_t *)malloc(sizeof(boolean_obj_t));
    bool are_both_ints =
        left_obj->type() == INTEGER && right_obj->type() == INTEGER;
    if (!are_both_ints) {
      error_obj_t *error_obj = create_error_object();
      char *error_string = (char *)malloc(STRING_MAX_SIZE);
      snprintf(error_string, STRING_MAX_SIZE, "Cannot compare non integers");
      error_obj->message = error_string;
      return (object_t *)error_obj;
    }
    bool_obj->value = (((integer_obj_t *)left_obj)->value <
                       ((integer_obj_t *)right_obj)->value);
    bool_obj->object.type = type_boolean;
    bool_obj->object.inspect = inspect_boolean;
    return (object_t *)bool_obj;
  }
  return NULL;
}

object_t *eval_prefix_expression(prefix_expression_t *prefix_expr,
                                 environment_t *env) {
  object_t *right = eval_expression(prefix_expr->right, env);
  if (strcmp(prefix_expr->op, "!") == 0) {
    // TODO: refactor: make into seperate functions
    boolean_obj_t *boolean_obj = (boolean_obj_t *)malloc(sizeof(boolean_obj_t));
    switch (right->type()) {
    case BOOLEAN: {
      boolean_obj_t *boolean_obj_right = (boolean_obj_t *)right;
      boolean_obj->value = boolean_obj_right->value == true ? false : true;
      break;
    }
    case NULL_OBJ: {
      boolean_obj->value = true;
      break;
    }
    case INTEGER: {
      integer_obj_t *integer_obj = (integer_obj_t *)right;
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
    return (object_t *)boolean_obj;
  } else if (strcmp(prefix_expr->op, "-") == 0) {
    if (right->type() != INTEGER) {
      error_obj_t *error_obj = create_error_object();
      char *error_string = (char *)malloc(STRING_MAX_SIZE);
      snprintf(error_string, STRING_MAX_SIZE,
               "- should preceed only an integer literal got %s",
               object_type_strings[right->type()]);
      error_obj->message = error_string;
      return (object_t *)error_obj;
    }
    integer_obj_t *integer_obj = (integer_obj_t *)right;
    integer_obj_t *return_integer_obj =
        (integer_obj_t *)malloc(sizeof(integer_obj_t));
    // TODO: create a function to intialize integer objects
    return_integer_obj->value = -integer_obj->value;
    return_integer_obj->object.type = type_int;
    return_integer_obj->object.inspect = inspect_int;
    return (object_t *)return_integer_obj;
  } else {
    assert("Should not be here");
  }
  return NULL;
}

object_t *eval_expression(expression_t *expression, environment_t *env) {
  switch (expression->type) {
  case INTEGER_LITERAL: {
    integer_t *integer_expr = (integer_t *)expression;
    integer_obj_t *integer_obj = (integer_obj_t *)malloc(sizeof(integer_obj_t));
    integer_obj->value = integer_expr->value;
    integer_obj->object.type = type_int;
    integer_obj->object.inspect = inspect_int;
    return (object_t *)integer_obj;
  }
  case IDENTIFIER: {
    identifier_t *identifier = (identifier_t *)expression;
    if (strcmp(identifier->value, "null") == 0) {
      null_obj_t *null_obj = (null_obj_t *)malloc(sizeof(null_obj_t));
      null_obj->object.type = type_null;
      null_obj->object.inspect = inspect_null;
      return (object_t *)null_obj;
    }
    object_t *val = get_environment(env, identifier->value);
    if (val == NULL) {
      error_obj_t *err_obj = create_error_object();
      err_obj->message = (char *)malloc(STRING_MAX_SIZE);
      snprintf(err_obj->message, STRING_MAX_SIZE, "Identifier not found: %s",
               identifier->value);
      return (object_t *)err_obj;
    }
    return val;
  }
  case BOOLEAN_LITERAL: {
    boolean_expression_t *boolean_expression =
        (boolean_expression_t *)expression;
    boolean_obj_t *boolean_obj = (boolean_obj_t *)malloc(sizeof(boolean_obj_t));
    boolean_obj->value = boolean_expression->value;
    boolean_obj->object.type = type_boolean;
    boolean_obj->object.inspect = inspect_boolean;
    return (object_t *)boolean_obj;
  }
  case PREFIX_EXPRESSION: {
    prefix_expression_t *prefix_expr = (prefix_expression_t *)expression;
    return eval_prefix_expression(prefix_expr, env);
  }
  case INFIX_EXPRESSION: {
    infix_expression_t *infix_expression = (infix_expression_t *)expression;
    return eval_infix_expression(infix_expression, env);
  }
  case IF_EXPRESSION: {
    if_expression_t *if_expression = (if_expression_t *)expression;
    return eval_if_expression(if_expression, env);
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

object_t *eval_block_statement(block_statement_t *block_statement,
                               environment_t *env) {
  object_t *result;
  for (int i = 0; i < block_statement->statements_length; ++i) {
    result = eval_statement(block_statement->statements[i], env);
    if (result != NULL && result->type() == RETURN_VALUE_OBJ) {
      object_type rt = result->type();
      if (rt == RETURN_VALUE_OBJ || rt == ERROR_OBJ) {
        return result;
      }
    }
  }
  return result;
}

object_t *eval_statements(statement_t **statements, size_t statements_length,
                          environment_t *env) {
  object_t *result;
  for (int i = 0; i < statements_length; ++i) {
    result = eval_statement(statements[i], env);
  }
  return result;
}
object_t *eval_statement(statement_t *statement, environment_t *env) {
  switch (statement->type) {
  case LET_STATEMENT: {
    let_statement_t *let_stmt = (let_statement_t *)statement;
    object_t *val = eval_expression(let_stmt->value, env);
    if (val->type() == ERROR_OBJ) {
      return val;
    }
    set_environment(env, let_stmt->name->value, val);
    return NULL;
  }
  case RETURN_STATEMENT: {
    ret_statement_t *ret_stmt = (ret_statement_t *)statement;
    return_obj_t *ret_obj = (return_obj_t *)malloc(sizeof(return_obj_t));
    ret_obj->value = eval_expression(ret_stmt->return_value, env);
    ret_obj->object.type = type_return;
    ret_obj->object.inspect = inspect_return;
    return (object_t *)ret_obj;
  }
  case EXPRESSION_STATEMENT: {
    expression_statement_t *expr = (expression_statement_t *)statement;
    return eval_expression(expr->expression, env);
  }
  case BLOCK_STATEMENT: {
    block_statement_t *block = (block_statement_t *)statement;
    return eval_statements(block->statements, block->statements_length, env);
  }
  }
  return NULL;
}

object_t *eval_program(program_t *program, environment_t *env) {
  object_t *result;
  for (int i = 0; i < program->statements_length; ++i) {
    statement_t *stmt = program->statements[i];
    result = eval_statement(stmt, env);
    if (result == NULL) {
      return NULL;
    }

    if (result->type() == RETURN_VALUE_OBJ) {
      return ((return_obj_t *)result)->value;
    }
    if (result->type() == ERROR_OBJ) {
      return result;
    }
  }
  return result;
}

object_t *eval(node_t *node, environment_t *env) {
  switch (node->type) {
  case PROGRAM: {
    program_t *program_node = (program_t *)node;
    return eval_program(program_node, env);
  }
  case STATEMENT: {
    statement_t *statement = (statement_t *)node;
    return eval_statement(statement, env);
  }
  case EXPRESSION: {
    expression_t *expression = (expression_t *)node;
    return eval_expression(expression, env);
  }
  }
  return NULL;
}
