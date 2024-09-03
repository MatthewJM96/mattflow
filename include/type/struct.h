#ifndef __mattflow_type_struct_h
#define __mattflow_type_struct_h

#include "literal/identifier.h"

namespace mattflow {
    namespace type {
        struct StructType {
            friend constexpr bool
            operator==(const StructType& lhs, const StructType& rhs) {
                auto view = std::views::zip(lhs.fields, rhs.fields);

                return std::all_of(view.begin(), view.end(), [](auto it) {
                    return std::get<0>(it) == std::get<1>(it);
                });
            }

            std::vector<void*> fields;
        };
    }  // namespace type
}  // namespace mattflow
namespace mftype = mattflow::type;

#endif  // __mattflow_type_struct_h
