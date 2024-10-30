#ifndef __mattflow_type_type_h
#define __mattflow_type_type_h

#include "type/function.h"
#include "type/primitive.h"
#include "type/struct.h"

namespace mattflow {
    namespace type {
        struct TypeType {
            friend constexpr bool operator==(const TypeType& lhs, const TypeType& rhs) {
                return lhs.type == rhs.type;
            }

            void* type;
        };

        /**
         * @brief Refers to any unresolved type.
         *
         * TODO: do we need to store any state to aid resolution?
         */
        struct UnresolvedType {
            friend constexpr bool
            operator==(const UnresolvedType&, const UnresolvedType&) {
                return true;
            }
        };

        using UserType = std::variant<FunctionType, StructType, UnresolvedType>;
        using Type     = std::variant<UserType, PrimitiveType, TypeType>;
    }  // namespace type
}  // namespace mattflow
namespace mftype = mattflow::type;

#endif  // __mattflow_type_type_h
