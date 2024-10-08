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
        enum class NodeProps : uint32_t {
            NONE          = 0x00000000,
            ROOT          = 0x00000001,
            SINGLE_EXPR   = 0x00000002,
            MULTI_EXPR    = 0x00000004,
            LIST_OR_RANGE = 0x00000008,
            STRUCT        = 0x00000010,
            IF            = 0x00000020,
            THEN          = 0x00000040,
            ELIF          = 0x00000080,
            ELSE          = 0x00000100,
            FOR           = 0x00000200,
            WHILE         = 0x00000400,
            DO            = 0x00000800,
            PAREN_EXPR    = 0x00001000,
            BRACKET_EXPR  = 0x00002000,
            BRACE_EXPR    = 0x00004000,
            UNOP          = 0x00008000,
            BINOP         = 0x00010000,
            NONOP         = 0x00020000,
            ASSIGN_TYPE   = 0x00040000,
            IDENTIFIER    = 0x00080000
        };

        inline NodeProps operator|(NodeProps a, NodeProps b) {
            return static_cast<NodeProps>(
                static_cast<uint16_t>(a) | static_cast<uint16_t>(b)
            );
        }

        inline NodeProps operator&(NodeProps a, NodeProps b) {
            return static_cast<NodeProps>(
                static_cast<uint16_t>(a) & static_cast<uint16_t>(b)
            );
        }

        template <typename CandidateNodeType>
        concept NodeType = std::is_base_of_v<Node, CandidateNodeType>;

        /* clang-format off */

        using NodeInfo = std::variant<
            BlockExprNode, ParenExprNode, BracketExprNode,      // Expression
            IfExprNode, ForNode, RangeNode, WhileNode,          // Control Flow
            StructNode, StructFieldNode,                        // Struct
            FunctionNode,                                       // Function
            AssignDeducedValueOperatorNode,
            AssignValueOperatorNode, AssignTypeOperatorNode,
            DeducedArrowOperatorNode, ArrowOperatorNode,        // Assignment
            SequenceOperatorNode, CommaOperatorNode,            // Sequence
            RangeOperatorNode, RangeConstraintOperatorNode,     // Range
            OrOperatorNode, AndOperatorNode,                    // Logic
            NotEqualOperatorNode, EqualOperatorNode,            // Equality
            LesserOperatorNode, LesserOrEqualOperatorNode,
            GreaterOperatorNode, GreaterOrEqualOperatorNode,    // Comparison
            SubtractionOperatorNode, AdditionOperatorNode,      // Term
            DivisionOperatorNode, MultiplicationOperatorNode,   // Factor
            PowerOperatorNode,                                  // Power
            NotOperatorNode, NegationOperatorNode,              // Unary
            DotOperatorNode,                                    // Dot
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

        struct GetReprVisitor {
            template <typename NodeType>
            std::string operator()(const NodeType& node_info) {
                if constexpr (std::is_base_of_v<Node, NodeType>) {
                    return node_info.debug_repr();
                } else {
                    return "";
                }
            }
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_h
