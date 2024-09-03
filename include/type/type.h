#ifndef __mattflow_type_type_h
#define __mattflow_type_type_h

namespace mattflow {
    namespace type {
        struct TypeType {
            friend constexpr bool operator==(const TypeType& lhs, const TypeType& rhs) {
                return lhs.type == rhs.type;
            }

            void* type;
        };
    }  // namespace type
}  // namespace mattflow
namespace mftype = mattflow::type;

#endif  // __mattflow_type_type_h
