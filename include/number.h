#ifndef __mattflow_number_h
#define __mattflow_number_h

namespace mattflow {
    // TODO(Matthew): take string_view and immediately parse... requires a decision on
    //                widest types we'll support.
    class Number {
    public:
        Number();
        Number(std::string&& repr);

        std::string_view repr();

        // TODO(Matthew): functions to decode this type.
    protected:
        std::string m_repr;
    };
}  // namespace mattflow
namespace mf = mattflow;

#endif  // __mattflow_number_h
