#include "stdafx.h"

#include "backend/llvm/ir/converter.h"

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::NumberValNode& node) {
    // NOTE: For now we just make everything highest precision we support, this
    //       might not really be sufficient and we might need to do implicit and
    //       explicit casting of literals before representing the literal in LLVM
    //       IR.

    // Create an LLVM constant based on the number type (integer or floating-point)
    ::llvm::Constant* constant_value = nullptr;

    if (node.value.is_floating_point()) {
        // Handle floating-point numbers
        constant_value = ::llvm::ConstantFP::get(
            *context, ::llvm::APFloat(node.value.template as<double>())
        );
    } else {
        // Handle integers
        constant_value = ::llvm::ConstantInt::get(
            ::llvm::IntegerType::get(*context, 64), node.value.template as<uint64_t>()
        );
    }

    // Store the generated value in node_data for potential future use
    if (node_data) {
        *node_data = reinterpret_cast<void*>(constant_value);
    }
}
