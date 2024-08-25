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

namespace mattflow {
    namespace ast {
        /* clang-format off */

        enum class NodeType {
            BLOCK,                                              // Expression
            IF, FOR, RANGE, WHILE,                              // Control Flow
            STRUCT, STRUCT_FIELD,                               // Struct
            FUNCTION,                                           // Function
            ASSIGNMENT,                                         // Assignment
            OR, AND,                                            // Logical
            EQUAL, NOT_EQUAL,                                   // Equality
            GREATER, GREATER_OR_EQUAL, LESSER_OR_EQUAL, LESSER, // Comparison
            SUBTRACTION, ADDITION,                              // Term
            DIVISION, MULTIPLICATION,                           // Factor
            NOT, NEGATION,                                      // Unary
            BOOL, NUMBER, STRING, IDENTIFIER, NIL               // Primary
        };

        /* clang-format on */

        enum class Precedence : int16_t {
            NONE       = -1,
            ASSIGNMENT = 100,
            LOGICAL    = 200,
            EQUALITY   = 300,
            COMPARISON = 400,
            TERM       = 500,
            FACTOR     = 600,
            UNARY      = 700
        };

        enum class Associativity : int16_t {
            NONE = -1,
            LEFT,
            RIGHT
        };

        struct NodeProperties {
            Precedence    precedence;
            Associativity associativity;
        };

        /* clang-format off */

        const std::unordered_map<NodeType, NodeProperties> NODE_PROPERTIES = {
            {            NodeType::BLOCK,       { Precedence::NONE, Associativity::NONE  }},
            {               NodeType::IF,       { Precedence::NONE, Associativity::LEFT  }},
            {              NodeType::FOR,       { Precedence::NONE, Associativity::LEFT  }},
            {            NodeType::RANGE,       { Precedence::NONE, Associativity::NONE  }},
            {            NodeType::WHILE,       { Precedence::NONE, Associativity::LEFT  }},
            {           NodeType::STRUCT,       { Precedence::NONE, Associativity::NONE  }},
            {     NodeType::STRUCT_FIELD,       { Precedence::NONE, Associativity::NONE  }},
            {         NodeType::FUNCTION,       { Precedence::NONE, Associativity::NONE  }},
            {       NodeType::ASSIGNMENT, { Precedence::ASSIGNMENT, Associativity::LEFT  }},
            {            NodeType::EQUAL,   { Precedence::EQUALITY, Associativity::LEFT  }},
            {        NodeType::NOT_EQUAL,   { Precedence::EQUALITY, Associativity::LEFT  }},
            {          NodeType::GREATER, { Precedence::COMPARISON, Associativity::LEFT  }},
            { NodeType::GREATER_OR_EQUAL, { Precedence::COMPARISON, Associativity::LEFT  }},
            {  NodeType::LESSER_OR_EQUAL, { Precedence::COMPARISON, Associativity::LEFT  }},
            {           NodeType::LESSER, { Precedence::COMPARISON, Associativity::LEFT  }},
            {         NodeType::ADDITION,       { Precedence::TERM, Associativity::LEFT  }},
            {      NodeType::SUBTRACTION,       { Precedence::TERM, Associativity::LEFT  }},
            {   NodeType::MULTIPLICATION,     { Precedence::FACTOR, Associativity::LEFT  }},
            {         NodeType::DIVISION,     { Precedence::FACTOR, Associativity::LEFT  }},
            {              NodeType::NOT,      { Precedence::UNARY, Associativity::RIGHT }},
            {         NodeType::NEGATION,      { Precedence::UNARY, Associativity::RIGHT }},
            {             NodeType::BOOL,       { Precedence::NONE, Associativity::NONE  }},
            {           NodeType::NUMBER,       { Precedence::NONE, Associativity::NONE  }},
            {           NodeType::STRING,       { Precedence::NONE, Associativity::NONE  }},
            {       NodeType::IDENTIFIER,       { Precedence::NONE, Associativity::NONE  }},
        };

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
            NotOperatorNode, NegationOperatorNode,              // Unary
            BoolNode, NumberNode, StringNode, IdentifierNode,
            NullNode                                            // Primary
        >;

        /* clang-format on */

        struct NodeBuffers {
            std::vector<NodeInfo> node_info;
            NodeInfo              entry_node;

            ASTVertexNodeMap vertex_node_map;
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_h
