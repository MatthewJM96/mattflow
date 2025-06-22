#ifndef __mattflow_backend_llvm_ir_converter_h
#define __mattflow_backend_llvm_ir_converter_h

#include "ast/ast.h"
#include "ast/node.h"
#include "variable/variable.h"

namespace mattflow {
    namespace backend {
        namespace llvm {
            struct LLVM_IR_Converter {
                LLVM_IR_Converter(
                    mfast::AST&          _ast,
                    mfast::ASTVertex     _vertex,
                    mfast::NodeData&     _processed_node_data,
                    ::llvm::LLVMContext* _context,
                    ::llvm::IRBuilder<>* _builder,
                    ::llvm::Module*      _module,
                    void**               _node_data
                ) :
                    ast(_ast),
                    vertex(_vertex),
                    processed_node_data(_processed_node_data),
                    context(_context),
                    builder(_builder),
                    module(_module),
                    node_data(_node_data) {
                    // Empty.
                }

                mfast::AST&      ast;
                mfast::ASTVertex vertex;

                mfast::NodeData& processed_node_data;

                ::llvm::LLVMContext* context;
                ::llvm::IRBuilder<>* builder;
                ::llvm::Module*      module;

                void** node_data;

                void operator()(const mfast::BlockExprNode& node);
                void operator()(const mfast::ParenExprNode& node);
                void operator()(const mfast::BracketExprNode& node);
                void operator()(const mfast::IfExprNode& node);
                void operator()(const mfast::ForNode& node);
                void operator()(const mfast::RangeNode& node);
                void operator()(const mfast::WhileNode& node);
                void operator()(const mfast::StructNode& node);
                void operator()(const mfast::StructFieldNode& node);
                void operator()(const mfast::FunctionNode& node);
                void operator()(const mfast::AssignDeducedValueOperatorNode& node);
                void operator()(const mfast::AssignValueOperatorNode& node);
                void operator()(const mfast::AssignTypeOperatorNode& node);
                void operator()(const mfast::DeducedArrowOperatorNode& node);
                void operator()(const mfast::ArrowOperatorNode& node);
                void operator()(const mfast::SequenceOperatorNode& node);
                void operator()(const mfast::CommaOperatorNode& node);
                void operator()(const mfast::RangeOperatorNode& node);
                void operator()(const mfast::RangeConstraintOperatorNode& node);
                void operator()(const mfast::OrOperatorNode& node);
                void operator()(const mfast::AndOperatorNode& node);
                void operator()(const mfast::NotEqualOperatorNode& node);
                void operator()(const mfast::EqualOperatorNode& node);
                void operator()(const mfast::LesserOperatorNode& node);
                void operator()(const mfast::LesserOrEqualOperatorNode& node);
                void operator()(const mfast::GreaterOperatorNode& node);
                void operator()(const mfast::GreaterOrEqualOperatorNode& node);
                void operator()(const mfast::SubtractionOperatorNode& node);
                void operator()(const mfast::AdditionOperatorNode& node);
                void operator()(const mfast::DivisionOperatorNode& node);
                void operator()(const mfast::MultiplicationOperatorNode& node);
                void operator()(const mfast::PowerOperatorNode& node);
                void operator()(const mfast::NotOperatorNode& node);
                void operator()(const mfast::NegationOperatorNode& node);
                void operator()(const mfast::DotOperatorNode& node);
                void operator()(const mfast::BoolValNode& node);
                void operator()(const mfast::NumberValNode& node);
                void operator()(const mfast::StringValNode& node);
                void operator()(const mfast::IdentifierNode& node);
                void operator()(const mfast::NullNode& node);
                void operator()(const mfast::BoolNode& node);
                void operator()(const mfast::CharNode& node);
                void operator()(const mfast::IntNode& node);
                void operator()(const mfast::Int8Node& node);
                void operator()(const mfast::Int16Node& node);
                void operator()(const mfast::Int32Node& node);
                void operator()(const mfast::Int64Node& node);
                void operator()(const mfast::UIntNode& node);
                void operator()(const mfast::UInt8Node& node);
                void operator()(const mfast::UInt16Node& node);
                void operator()(const mfast::UInt32Node& node);
                void operator()(const mfast::UInt64Node& node);
                void operator()(const mfast::Float32Node& node);
                void operator()(const mfast::Float64Node& node);
            };
        }  // namespace llvm
    }      // namespace backend
}  // namespace mattflow
namespace mfbe = mattflow::backend;

#endif  // __mattflow_backend_llvm_ir_converter_h
