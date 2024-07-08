#ifndef __mattflow_type_function_h
#define __mattflow_type_function_h

namespace mattflow {
    namespace type {
        struct FunctionType {
            Type*              return_type;
            std::vector<Type*> arguments;
        };
    }  // namespace type
}  // namespace mattflow
namespace mftype = mattflow::type;

#endif  // __mattflow_type_function_h
