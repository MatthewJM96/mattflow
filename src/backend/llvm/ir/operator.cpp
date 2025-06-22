#include "stdafx.h"

#include "backend/llvm/ir/converter.h"

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::DotOperatorNode& node) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::NotOperatorNode& node) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::NegationOperatorNode& node
) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::PowerOperatorNode& node) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(
    const mfast::MultiplicationOperatorNode& node
) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::DivisionOperatorNode& node
) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::AdditionOperatorNode& node
) {
    auto [beg, end] = boost::out_edges(vertex, ast);

    mfassert(
        std::distance(beg, end) == 2,
        "LLVM_IR_Converter expects 2 children of AdditionOperatorNode."
    );

    auto lhs = boost::target(*beg, ast);
    auto rhs = boost::target(*(beg + 1), ast);

    auto lhs_data = processed_node_data[lhs];
    auto rhs_data = processed_node_data[rhs];

    // TODO(Matthew): Need a way of knowing what is in the type-erased data here. I
    //                think trivially another visitor to extract the appropriate info
    //                is possible but also we could start simple and assume constants
    //                to get to some trivial language that compiles.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(
    const mfast::SubtractionOperatorNode& node
) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::GreaterOperatorNode& node) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(
    const mfast::GreaterOrEqualOperatorNode& node
) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::LesserOperatorNode& node) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(
    const mfast::LesserOrEqualOperatorNode& node
) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::EqualOperatorNode& node) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::NotEqualOperatorNode& node
) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::OrOperatorNode& node) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::AndOperatorNode& node) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::RangeOperatorNode& node) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(
    const mfast::RangeConstraintOperatorNode& node
) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::SequenceOperatorNode& node
) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::CommaOperatorNode& node) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(
    const mfast::AssignDeducedValueOperatorNode& node
) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(
    const mfast::AssignValueOperatorNode& node
) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::AssignTypeOperatorNode& node
) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(
    const mfast::DeducedArrowOperatorNode& node
) {
    // TODO(Matthew): Implement.
}

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::ArrowOperatorNode& node) {
    // TODO(Matthew): Implement.
}
