#ifndef __mattflow_type_h
#define __mattflow_type_h

#include "lex/token.h"
#include "type/function.h"
#include "type/primitive.h"
#include "type/struct.h"
#include "type/type.h"

namespace mattflow {
    namespace type {
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

        class IdentifierTypeTable {
        public:
            using Map      = std::unordered_map<mflit::IdentifierIdx, Type>;
            using MapEntry = Map::const_iterator;

            IdentifierTypeTable() {
                // Empty.
            }

            MATTFLOW_NON_COPYABLE(IdentifierTypeTable);

            MapEntry register_identifier(mflit::IdentifierIdx identifier);

            std::tuple<bool, mftype::IdentifierTypeTable::MapEntry>
            associate_type(mflit::IdentifierIdx identifier, const mftype::Type& type);
        protected:
            Map m_ident_type_map;
        };
    }  // namespace type
}  // namespace mattflow
namespace mftype = mattflow::type;

#endif  // __mattflow_type_h
