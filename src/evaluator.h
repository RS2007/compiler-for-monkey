#include "environment.h"
#include "nodes.h"
#include "object.h"
#include "utils.h"

object_t *eval(node_t *node, environment_t *env);
void free_program_node(program_t *program_node);
