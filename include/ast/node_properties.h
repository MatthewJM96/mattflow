#ifndef __mattflow_ast_node_properties_h
#define __mattflow_ast_node_properties_h

namespace mattflow {
    namespace ast {
        using Precedence                = int16_t;
        const Precedence NO_PRECENDENCE = -1;

        enum class Associativity {
            NONE,
            LEFT,
            RIGHT
        };

        template <typename NodeType>
        struct NodeProperties {
            // Empty.
        };

        /**
         * Control Flow
         */

        template <>
        struct NodeProperties<IfNode> {
            const Precedence    precedence    = NO_PRECENDENCE;
            const Associativity associativity = Associativity::LEFT;
        };

        template <>
        struct NodeProperties<ForNode> {
            const Precedence    precedence    = NO_PRECENDENCE;
            const Associativity associativity = Associativity::LEFT;
        };

        template <>
        struct NodeProperties<RangeNode> {
            const Precedence    precedence    = NO_PRECENDENCE;
            const Associativity associativity = Associativity::NONE;
        };

        template <>
        struct NodeProperties<WhileNode> {
            const Precedence    precedence    = NO_PRECENDENCE;
            const Associativity associativity = Associativity::LEFT;
        };

        /**
         * Struct
         */

        template <>
        struct NodeProperties<StructNode> {
            const Precedence    precedence    = NO_PRECENDENCE;
            const Associativity associativity = Associativity::NONE;
        };

        template <>
        struct NodeProperties<StructFieldNode> {
            const Precedence    precedence    = NO_PRECENDENCE;
            const Associativity associativity = Associativity::NONE;
        };

        /**
         * Function
         */

        template <>
        struct NodeProperties<FunctionNode> {
            const Precedence    precedence    = NO_PRECENDENCE;
            const Associativity associativity = Associativity::NONE;
        };

        /**
         * Operators
         */

        template <>
        struct NodeProperties<AssignmentOperatorNode> {
            const Precedence    precedence    = 100;
            const Associativity associativity = Associativity::LEFT;
        };

        template <>
        struct NodeProperties<AndOperatorNode> {
            const Precedence    precedence    = 200;
            const Associativity associativity = Associativity::LEFT;
        };

        template <>
        struct NodeProperties<OrOperatorNode> {
            const Precedence    precedence    = 200;
            const Associativity associativity = Associativity::LEFT;
        };

        template <>
        struct NodeProperties<NotEqualOperatorNode> {
            const Precedence    precedence    = 300;
            const Associativity associativity = Associativity::LEFT;
        };

        template <>
        struct NodeProperties<EqualOperatorNode> {
            const Precedence    precedence    = 300;
            const Associativity associativity = Associativity::LEFT;
        };

        template <>
        struct NodeProperties<LesserOrEqualOperatorNode> {
            const Precedence    precedence    = 400;
            const Associativity associativity = Associativity::LEFT;
        };

        template <>
        struct NodeProperties<LesserOperatorNode> {
            const Precedence    precedence    = 400;
            const Associativity associativity = Associativity::LEFT;
        };

        template <>
        struct NodeProperties<GreaterOrEqualOperatorNode> {
            const Precedence    precedence    = 400;
            const Associativity associativity = Associativity::LEFT;
        };

        template <>
        struct NodeProperties<GreaterOperatorNode> {
            const Precedence    precedence    = 400;
            const Associativity associativity = Associativity::LEFT;
        };

        template <>
        struct NodeProperties<SubtractionOperatorNode> {
            const Precedence    precedence    = 500;
            const Associativity associativity = Associativity::LEFT;
        };

        template <>
        struct NodeProperties<AdditionOperatorNode> {
            const Precedence    precedence    = 500;
            const Associativity associativity = Associativity::LEFT;
        };

        template <>
        struct NodeProperties<DivisionOperatorNode> {
            const Precedence    precedence    = 600;
            const Associativity associativity = Associativity::LEFT;
        };

        template <>
        struct NodeProperties<MultiplicationOperatorNode> {
            const Precedence    precedence    = 600;
            const Associativity associativity = Associativity::LEFT;
        };

        template <>
        struct NodeProperties<NegationOperatorNode> {
            const Precedence    precedence    = 700;
            const Associativity associativity = Associativity::RIGHT;
        };

        template <>
        struct NodeProperties<NotOperatorNode> {
            const Precedence    precedence    = 700;
            const Associativity associativity = Associativity::RIGHT;
        };

        /**
         * Primary
         */

        template <>
        struct NodeProperties<NumberNode> {
            const Precedence    precedence    = 1000;
            const Associativity associativity = Associativity::NONE;
        };

        template <>
        struct NodeProperties<StringNode> {
            const Precedence    precedence    = 1000;
            const Associativity associativity = Associativity::NONE;
        };

        template <>
        struct NodeProperties<IdentifierNode> {
            const Precedence    precedence    = 1000;
            const Associativity associativity = Associativity::NONE;
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_node_properties_h
