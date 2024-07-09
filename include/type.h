#ifndef __mattflow_type_type_h
#define __mattflow_type_type_h

#include "type/function.h"
#include "type/intrinsic.h"
#include "type/struct.h"

namespace mattflow {
    namespace type {
        enum class TypeKind {
            UNRESOLVED,
            INTRINSIC,
            STRUCT,
            FUNCTION
        };

        struct Type {
            TypeKind type_kind;

            union {
                FunctionType  function_type;
                IntrinsicType intrinsic_type;
                StructType    struct_type;
                void*         null_type;
            };
        };
    }  // namespace type
}  // namespace mattflow
namespace mftype = mattflow::type;

#endif  // __mattflow_type_type_h
