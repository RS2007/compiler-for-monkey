#include "nodes.h"
#include "object.h"
#include "utils.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>

object_t* eval(node_t*);
object_t* eval_expression(expression_t*);

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
        unimplemented();
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
        unimplemented();
    }
    case IF_EXPRESSION: {
        unimplemented();
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

object_t* eval_statement(statement_t* statement)
{
    switch (statement->type) {
    case LET_STATEMENT: {
        unimplemented();
    }
    case RETURN_STATEMENT: {
        unimplemented();
    }
    case EXPRESSION_STATEMENT: {
        expression_statement_t* expr = (expression_statement_t*)statement;
        return eval_expression(expr->expression);
    }
    case BLOCK_STATEMENT: {
        unimplemented();
    }
    }
    return NULL;
}

object_t* eval_program(program_t* program)
{
    statement_t* stmt = program->statements[0];
    return eval_statement(stmt);
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
