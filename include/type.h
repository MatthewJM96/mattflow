#ifndef __mattflow_type_h
#define __mattflow_type_h

#include "lex/token.h"
#include "type/function.h"
#include "type/number.h"
#include "type/struct.h"
#include "type/type.h"

namespace mattflow {
    namespace type {
        using Type
            = std::variant<FunctionType, NumberType, StructType, TypeType, void*>;

        class TypeTable {
        public:
            TypeTable() {
                // Empty.
            }

            MATTFLOW_NON_COPYABLE(TypeTable);

            Type* register_type_from_token(const mflex::Token& tok);
        protected:
            std::vector<Type>                               m_types;
            std::unordered_map<mflit::IdentifierIdx, Type*> m_ident_type_map;
        };
    }  // namespace type
}  // namespace mattflow
namespace mftype = mattflow::type;

#endif  // __mattflow_type_h
