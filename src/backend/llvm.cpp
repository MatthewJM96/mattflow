#include "stdafx.h"

#include "backend/llvm.h"

void mfbe::convert_module_to_llvm_ir(
    VALIN mfast::AST& ast,
    VALIN mfast::NodeBuffers& nodes,
    VALIN mftype::IdentifierTypeTable& type_table
) {
    (void)ast;
    (void)nodes;
    (void)type_table;

    llvm::LLVMContext context;
    llvm::IRBuilder   builder(context);
    llvm::Module      module("a_module", context);

    const auto func_type = llvm::FunctionType::get(builder.getVoidTy(), false);
    const auto main_func = llvm::Function::Create(
        func_type, llvm::Function::ExternalLinkage, "main", &module
    );

    const auto entry = llvm::BasicBlock::Create(context, "entrypoint", main_func);
    builder.SetInsertPoint(entry);

    const std::vector<llvm::Type*> puts_args = { builder.getInt8Ty()->getPointerTo() };
    const llvm::ArrayRef           puts_args_ref(puts_args);

    const auto puts_type
        = llvm::FunctionType::get(builder.getInt32Ty(), puts_args_ref, false);
    const auto puts_func = module.getOrInsertFunction("puts", puts_type);

    auto str = builder.CreateGlobalStringPtr("Hello LLVM!\n");

    builder.CreateCall(puts_func, { str });
    builder.CreateRetVoid();

    module.dump();

    std::cout << std::endl;
}
