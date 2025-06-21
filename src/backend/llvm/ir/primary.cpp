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

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::NullNode& node) {
    // Create an LLVM constant for null
    ::llvm::Constant* constant_value
        = ::llvm::ConstantPointerNull::get(::llvm::PointerType::get(*context, 0));

    // Store the generated value in node_data for potential future use
    if (node_data) {
        *node_data = reinterpret_cast<void*>(constant_value);
    }
}

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::BoolValNode& node) {
    // Create an LLVM constant for boolean value
    ::llvm::Constant* constant_value
        = ::llvm::ConstantInt::get(::llvm::Type::getInt1Ty(*context), node.value);

    // Store the generated value in node_data for potential future use
    if (node_data) {
        *node_data = reinterpret_cast<void*>(constant_value);
    }
}

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::StringValNode& node) {
    // Get the string value from the string table
    static auto& str_table = mflit::StringTable::get();
    std::string  str_value = std::string(str_table.get(node.value));

    // Create an LLVM global string constant
    ::llvm::Constant* constant_value
        = ::llvm::ConstantDataArray::getString(*context, str_value);

    // Store the generated value in node_data for potential future use
    if (node_data) {
        *node_data = reinterpret_cast<void*>(constant_value);
    }
}

void mfbe::llvm::LLVM_IR_Converter::operator()(const mfast::IdentifierNode& node) {
    // TODO(Matthew): We probably actually want to implement the calls to getNamedGlobal
    //                and ::llvm::GlobalVariable(...) as lazily as possible. Ultimately
    //                things to think about here are when will we know what scope this
    //                identifier is being used in, and how far up the scopes will we
    //                need to look to find the type of the referrant of the identifier.
    //                  We could do all the identifier chasing prior to LLVM IR
    //                  generation, and then we can generate the necessary variable
    //                  references for each disconnected instance of each identifier
    //                  (disconnected here referring to if e.g. an identifier "x" is
    //                  used in one function as type int and in another function as type
    //                  float - actually even if two functions used "x" as the same type
    //                  but locally declared the two variables). We can then simply get
    //                  the appropriate variable. However intuitively it feels like
    //                  performance would ask us to instead do creation lazily too and
    //                  somehow here work out the scope at which the identifier's
    //                  referrant is declared and get the appropriate variable for that.

    // Get the identifier name from the identifier table
    static auto& ident_table = mflit::IdentifierTable::get();
    std::string  ident_name  = std::string(ident_table.get(node.name));

    // Create an LLVM global variable for the identifier
    ::llvm::Constant* constant_value = module->getNamedGlobal(ident_name);

    if (!constant_value) {
        // If the global variable does not exist, create a new one
        constant_value = new ::llvm::GlobalVariable(
            *module,
            ::llvm::Type::getInt32Ty(*context),
            false,
            ::llvm::GlobalValue::ExternalLinkage,
            nullptr,
            ident_name
        );
    }

    // Store the generated value in node_data for potential future use
    if (node_data) {
        *node_data = reinterpret_cast<void*>(constant_value);
    }
}
