#ifndef __mattflow_backend_llvm_h
#define __mattflow_backend_llvm_h

#include "ast/ast.h"
#include "ast/node.h"

namespace mattflow {
    namespace backend {
        void convert_module_to_llvm_ir(
            VALIN mfast::AST& ast,
            VALIN mfast::NodeBuffers& nodes,
            VALIN mftype::IdentifierTypeTable& type_table
        );
    }  // namespace backend
}  // namespace mattflow
namespace mfbe = mattflow::backend;

#endif  // __mattflow_backend_llvm_h
