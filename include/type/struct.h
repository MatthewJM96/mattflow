#ifndef __mattflow_type_struct_h
#define __mattflow_type_struct_h

#include "literal/identifier.h"

namespace mattflow {
    namespace type {
        struct Type;

        struct StructType {
            std::vector<Type*> fields;
        };
    }  // namespace type
}  // namespace mattflow
namespace mftype = mattflow::type;

#endif  // __mattflow_type_struct_h
