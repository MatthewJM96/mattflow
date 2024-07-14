#ifndef __mattflow_type_h
#define __mattflow_type_h

#include "lex/token.h"
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
            ~Type() {
                // Empty.
            }

            TypeKind type_kind;

            union {
                FunctionType function_type;
                NumberType   number_type;
                StructType   struct_type;
                TypeType     type_type;
                void*        null_type;
            };
        };

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
