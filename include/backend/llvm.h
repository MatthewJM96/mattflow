#ifndef __mattflow_backend_llvm_h
#define __mattflow_backend_llvm_h

#include "ast/ast.h"
#include "ast/node.h"
#include "variable/variable.h"

namespace mattflow {
    namespace backend {
        void convert_module_to_llvm_ir(
            VALIN mfast::AST& ast,
            VALIN mfast::NodeBuffers& nodes,
            VALIN mfvar::VariableTypeTable& var_table
        );
    }  // namespace backend
}  // namespace mattflow
namespace mfbe = mattflow::backend;

#endif  // __mattflow_backend_llvm_h
