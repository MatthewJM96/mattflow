#include "stdafx.h"

#include "llvm/ir.h"

void mfllvm::convert_module_to_ir(
    VALIN mfast::AST& ast,
    VALIN mfast::NodeBuffers& nodes,
    VALIN mftype::IdentifierTypeTable& type_table
) {
    (void)ast;
    (void)nodes;
    (void)type_table;

    ::llvm::LLVMContext context;
    ::llvm::IRBuilder   builder(context);
    ::llvm::Module      module("a_module", context);
}
