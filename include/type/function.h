#ifndef __mattflow_type_function_h
#define __mattflow_type_function_h

namespace mattflow {
    namespace type {
        struct FunctionType {
            void*              return_type;
            std::vector<void*> arguments;
        };
    }  // namespace type
}  // namespace mattflow
namespace mftype = mattflow::type;

#endif  // __mattflow_type_function_h
