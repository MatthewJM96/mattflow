#ifndef __mattflow_type_struct_h
#define __mattflow_type_struct_h

#include "identifier.h"

namespace mattflow {
    namespace type {
        struct Type;

        // TODO(Matthew): disentangle field names from type? allows for entirely
        // anonymous structs.
        struct StructType {
            std::vector<std::tuple<IdentifierIdx, Type*>> fields;
        };
    }  // namespace type
}  // namespace mattflow
namespace mftype = mattflow::type;

#endif  // __mattflow_type_struct_h
