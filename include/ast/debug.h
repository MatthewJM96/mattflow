#ifndef __mattflow_ast_debug_h
#define __mattflow_ast_debug_h

#include "ast/node.h"

namespace mattflow {
    namespace ast {
        class NodeInfoWriter {
        public:
            NodeInfoWriter(NodeBuffers* _node_buffers) : node_buffers(_node_buffers) { }

            void operator()(std::ostream& out, size_t v) const {
                auto& node_info = node_buffers->get_node_info(v);

                std::visit(
                    [&](auto& n) { out << "[label=\"" + n.debug_repr() + "\"]"; },
                    node_info
                );
            }
        private:
            NodeBuffers* node_buffers;
        };
    }  // namespace ast
}  // namespace mattflow
namespace mfast = mattflow::ast;

#endif  // __mattflow_ast_debug_h
