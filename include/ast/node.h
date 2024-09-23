#ifndef __mattflow_ast_node_h
#define __mattflow_ast_node_h

#include "ast/ast.h"

#include "ast/node/node.h"

#include "ast/node/control_flow.h"
#include "ast/node/expression.h"
#include "ast/node/function.h"
#include "ast/node/operator.h"
#include "ast/node/primary.h"
#include "ast/node/struct.h"
#include "ast/node/type.h"

namespace mattflow {
    namespace ast {
        enum class NodeCategory {
            NONE,
            BINOP,
            UNOP,
            PRIMARY
        };

        enum class EnclosingCategory {
            ROOT,
            BLOCK,
            PARENTHESES,
            LIST_OR_RANGE,
            STRUCT,
            FUNCTION_PARAMS,
            FUNCTION_BODY
        };

        /* clang-format off */

        using NodeInfo = std::variant<
            BlockExprNode,                                      // Expression
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
            PowerOperatorNode,                                  // Power
            NotOperatorNode, NegationOperatorNode,              // Unary
            BoolValNode, NumberValNode, StringValNode,
            IdentifierNode, NullNode,                           // Primary
            BoolNode, CharNode, IntNode, Int8Node, Int16Node,
            Int32Node, Int64Node, UIntNode, UInt8Node,
            UInt16Node, UInt32Node, UInt64Node, Float32Node,
            Float64Node                                         // Type
        >;

        /* clang-format on */

        struct NodeBuffers {
            std::vector<NodeInfo> node_info;

            ASTVertexNodeMap vertex_node_map;

            NodeInfo& get_node_info(ASTVertex vertex) {
                return node_info[vertex_node_map[vertex]];
            }
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_h
