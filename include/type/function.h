#ifndef __mattflow_type_function_h
#define __mattflow_type_function_h

namespace mattflow {
    namespace type {
        struct FunctionType {
            friend constexpr bool
            operator==(const FunctionType& lhs, const FunctionType& rhs) {
                auto view = std::views::zip(lhs.arguments, rhs.arguments);

                return std::all_of(
                           view.begin(),
                           view.end(),
                           [](auto it) { return std::get<0>(it) == std::get<1>(it); }
                       )
                       && (lhs.return_type == rhs.return_type);
            }

            void*              return_type;
            std::vector<void*> arguments;
        };
    }  // namespace type
}  // namespace mattflow
namespace mftype = mattflow::type;

#endif  // __mattflow_type_function_h
