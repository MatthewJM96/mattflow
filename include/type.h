#ifndef __mattflow_type_type_h
#define __mattflow_type_type_h

#include "type/function.h"
#include "type/intrinsic.h"
#include "type/struct.h"

namespace mattflow {
    namespace type {
        enum class BasicType {
            INTRINSIC,
            STRUCT,
            FUNCTION
        };

        struct Type {
            BasicType basic_type;

            union {
                FunctionType  function_type;
                IntrinsicType intrinsic_type;
                StructType    struct_type;
            }
        };
    }  // namespace type
}  // namespace mattflow
namespace mftype = mattflow::type;

#endif  // __mattflow_type_type_h
