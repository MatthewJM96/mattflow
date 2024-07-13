#ifndef __mattflow_ast_node_h
#define __mattflow_ast_node_h

#include "ast/ast.h"

#include "ast/node/node.h"

#include "ast/node/function.h"
#include "ast/node/number.h"
#include "ast/node/operator.h"
#include "ast/node/string.h"
#include "ast/node/variable.h"

namespace mattflow {
    namespace ast {
        struct NodeBuffers {
            std::vector<FunctionNode> functions;
            std::vector<NumberNode>   numbers;
            std::vector<OperatorNode> operators;
            std::vector<VariableNode> variables;
            Node                      nil_node;

            ASTVertexNodeMap vertex_node_map;
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_h
