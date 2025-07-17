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
         * @brief Refers to any case where the type is unresolved and we have yet no
         * information to aid in resolution.
         */
        struct UnresolvedType {
            friend constexpr bool
            operator==(const UnresolvedType&, const UnresolvedType&) {
                return true;
            }
        };

        /**
         * @brief Refers to any case where the type is unresolved and will be equal to
         * the resolved type of some other identifier.
         */
        struct UnresolvedTypeOf {
            friend constexpr bool
            operator==(const UnresolvedTypeOf&, const UnresolvedTypeOf&) {
                return true;
            }

            mflit::IdentifierIdx identifier;
        };

        /**
         * @brief Refers to any case where the type is unresolved and will be equal to
         * the type held by some other identifier. Therefore the other identifier, held
         * here, should turn out to be of type Type.
         */
        struct UnresolvedTypeHeldBy {
            friend constexpr bool
            operator==(const UnresolvedTypeHeldBy&, const UnresolvedTypeHeldBy&) {
                return true;
            }

            mflit::IdentifierIdx identifier;
        };

        using UserType = std::variant<FunctionType, StructType>;
        using Type     = std::variant<
            UserType,
            PrimitiveType,
            TypeType,
            UnresolvedType,
            UnresolvedTypeOf,
            UnresolvedTypeHeldBy>;
    }  // namespace type
}  // namespace mattflow
namespace mftype = mattflow::type;

#endif  // __mattflow_type_type_h
