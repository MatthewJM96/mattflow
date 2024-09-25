#ifndef __mattflow_debug_h
#define __mattflow_debug_h

namespace mattflow {
    class Debug {
    public:
        Debug(const Debug& rhs)            = delete;
        Debug& operator=(const Debug& rhs) = delete;

        Debug(Debug&& rhs)            = delete;
        Debug& operator=(Debug&& rhs) = delete;

        static Debug& get() {
            static Debug instance;

            return instance;
        }

        static void set_throw(bool _throw = true) { get().m_throw = _throw; }

        template <typename... Args>
        void print_or_throw(std::string&& message, Args... args) {
            const size_t msg_len       = 1000;
            char*        formatted_msg = new char[msg_len];
            std::snprintf(formatted_msg, msg_len, message.c_str(), args...);

            if (m_throw) {
                throw std::runtime_error(formatted_msg);
            } else {
                std::printf(formatted_msg);
                assert(false);
            }
        }
    protected:
        Debug() : m_throw(false) {
            // Empty.
        }

        bool m_throw;
    };
}  // namespace mattflow
namespace mf = mattflow;

#endif  // __mattflow_debug_h
