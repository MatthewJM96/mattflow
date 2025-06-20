#include "stdafx.h"

// #include "ast/node.h"

#include "backend/llvm.h"


// TODO(Matthew): Can we parellelise this?. Possible ways include:
//                  subtree-level parallelism, e.g. separating top-level functions
//                  task-based parellelism by expressing dependencies across nodes
//                  leaf-node parellelism via work stealing (useful for expression heavy
//                      functions)

struct LLVM_IR_Converter {
    LLVM_IR_Converter(
        llvm::LLVMContext* _context, llvm::IRBuilder<>* _builder, llvm::Module* _module, void** _node_data
    ) :
        context(_context), builder(_builder), module(_module), node_data(_node_data) {
        // Empty.
    }

    llvm::LLVMContext* context;
    llvm::IRBuilder<>* builder;
    llvm::Module*      module;
    void**             node_data;

    template <typename NodeType>
    void operator()(const NodeType&) { }

    void operator()(const mfast::NumberValNode&) { }
};

void mfbe::convert_module_to_llvm_ir(
    VALIN mfast::AST& ast,
    VALIN mfast::NodeBuffers& nodes,
    VALIN mfvar::VariableTypeTable& var_table
) {
    (void)var_table;

    ////////////////////////////////////////////////////////////////////////////////////
    // The plan:
    //  Iterate leaf nodes, setting up their respective LLVM data and link to this in
    //  hash map. Iterate the nodes pointing to the leaf nodes and do the same, wherever
    //  all nodes pointed to by that node are in the hash map. Place all nodes not thus
    //  processed onto a queue. Iterate all nodes in the queue and then all nodes
    //  pointed to by nodes processed in the last round in the same way. Recurse.

    ////////////////////////////////////////////////////////////////////////////////////
    // Set up module, builder and associated LLVM context.

    llvm::LLVMContext context;
    llvm::IRBuilder   builder(context);
    llvm::Module      module("a_module", context);

    // User data pointers for specific contents of each node.
    //   Note that different nodes will need to store different data and we will want
    //   some nice way of allocating space for this. Likely following the pattern of
    //   allocation of node data in our own AST structure.
    //   TODO(Matthew): either make this note happen as described or update it to
    //                  reflect reality.
    std::unordered_map<mfast::ASTVertex, void*> processed_node_data;

    ////////////////////////////////////////////////////////////////////////////////////
    // Determine initial number of out-edges of each vertex and queue vertices that
    // have no out-edges (a.k.a. are leaf nodes).

    std::queue<mfast::ASTVertex> queued_vertices;
    std::unordered_map<mfast::ASTVertex, uint64_t> unprocessed_out_degrees;
    for (auto vertex : boost::make_iterator_range(boost::vertices(ast))) {
        auto [beg, end] = boost::out_edges(vertex, ast);

        unprocessed_out_degrees[vertex] = end - beg;

        if ((end - beg) == 0) {
            queued_vertices.emplace(vertex);
        }
    }

    while (!queued_vertices.empty()) {
        // Get next vertex to process and pop it from queue.
        mfast::ASTVertex vertex = queued_vertices.front();
        queued_vertices.pop();


        // Visit node info with the LLVM IR converter.
        void* processed_data = nullptr;
        std::visit(
            LLVM_IR_Converter{ &context, &builder, &module, &processed_data },
            nodes.get_node_info(vertex)
        );
        
        processed_node_data[vertex] = processed_data;

        // Decrement out-edges that are unprocessed for all vertices pointing to this
        // processed vertex.
        for (auto edge : boost::make_iterator_range(boost::in_edges(vertex, ast))) {
            mfast::ASTVertex linked_vertex = boost::source(edge, ast);
            if (--unprocessed_out_degrees[linked_vertex] == 0) {
                queued_vertices.push(linked_vertex);
            }
        }
    }
}
