#ifndef __mattflow_type_struct_h
#define __mattflow_type_struct_h

#include "literal/identifier.h"

namespace mattflow {
    namespace type {
        struct StructType {
            std::vector<void*> fields;
        };
    }  // namespace type
}  // namespace mattflow
namespace mftype = mattflow::type;

#endif  // __mattflow_type_struct_h
