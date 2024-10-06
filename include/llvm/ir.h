#ifndef __mattflow_llvm_ir_h
#define __mattflow_llvm_ir_h

#include "ast/ast.h"
#include "ast/node.h"

namespace mattflow {
    namespace llvm {
        void convert_module_to_ir(
            VALIN mfast::AST& ast,
            VALIN mfast::NodeBuffers& nodes,
            VALIN mftype::IdentifierTypeTable& type_table
        );
    }  // namespace llvm
}  // namespace mattflow
namespace mfllvm = mattflow::llvm;

#endif  // __mattflow_llvm_ir_h
