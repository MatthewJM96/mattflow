#ifndef __mattflow_type_h
#define __mattflow_type_h

#include "type/function.h"
#include "type/number.h"
#include "type/struct.h"
#include "type/type.h"

namespace mattflow {
    namespace type {
        enum class TypeKind {
            UNRESOLVED,
            NUMBER,
            STRUCT,
            FUNCTION,
            TYPE,
            NULL_TYPE
        };

        struct Type {
            TypeKind type_kind;

            union {
                FunctionType function_type;
                NumberType   number_type;
                StructType   struct_type;
                TypeType     type_type;
                void*        null_type;
            };
        };
    }  // namespace type
}  // namespace mattflow
namespace mftype = mattflow::type;

#endif  // __mattflow_type_h
