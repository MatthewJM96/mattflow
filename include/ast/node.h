#ifndef __mattflow_ast_node_h
#define __mattflow_ast_node_h

#include "ast/ast.h"

#include "ast/node/node.h"

#include "ast/node/control_flow.h"
#include "ast/node/function.h"
#include "ast/node/operator.h"
#include "ast/node/primary.h"
#include "ast/node/struct.h"

#include "ast/node_properties.h"

namespace mattflow {
    namespace ast {
        /* clang-format off */

        using NodeInfo = std::variant<
            IfNode, ForNode, RangeNode, WhileNode,              // Control Flow
            StructNode, StructFieldNode,                        // Struct
            FunctionNode,                                       // Function
            AssignmentOperatorNode,                             // Assignment
            OrOperatorNode, AndOperatorNode,                    // Logic
            NotEqualOperatorNode, EqualOperatorNode,            // Equality
            LesserOperatorNode, LesserOrEqualOperatorNode,
            GreaterOperatorNode, GreaterOrEqualOperatorNode,    // Comparison
            SubtractionOperatorNode, AdditionOperatorNode,      // Term
            DivisionOperatorNode, MultiplicationOperatorNode,   // Factor
            NotOperatorNode, NegationOperatorNode,              // Unary
            NumberNode, StringNode, IdentifierNode              // Primary
        >;

        /* clang-format on */

        struct NodeBuffers {
            std::vector<NodeInfo> node_info;
            Node                  nil_node;

            ASTVertexNodeMap vertex_node_map;
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_h
