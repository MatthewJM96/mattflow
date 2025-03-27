#include "stdafx.h"

// #include "ast/node.h"

#include "backend/llvm.h"

struct LLVM_IR_Converter {
    LLVM_IR_Converter(
        llvm::LLVMContext* _context, llvm::IRBuilder<>* _builder, llvm::Module* _module
    ) :
        context(_context), builder(_builder), module(_module) {
        // Empty.
    }

    llvm::LLVMContext* context;
    llvm::IRBuilder<>* builder;
    llvm::Module*      module;

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

    // Determine module root vertex and all leaf nodes.

    // size_t module_root = *boost::vertices(ast).first;
    std::queue<mfast::ASTVertex>                queued_vertices;
    std::unordered_map<mfast::ASTVertex, void*> processed_vertices;
    for (auto vertex : boost::make_iterator_range(boost::vertices(ast))) {
        auto [beg, end] = boost::out_edges(vertex, ast);

        if (end - beg == 0) {
            queued_vertices.push(vertex);
        }
    }

    // TODO(Matthew): Can we parellelise this?

    // Set up module, builder and associated LLVM context.

    llvm::LLVMContext context;
    llvm::IRBuilder   builder(context);
    llvm::Module      module("a_module", context);

    // The plan:
    //  Iterate leaf nodes, setting up their respective LLVM data and link to this in
    //  hash map. Iterate the nodes pointing to the leaf nodes and do the same, wherever
    //  all nodes pointed to by that node are in the hash map. Place all nodes not thus
    //  processed onto a queue. Iterate all nodes in the queue and then all nodes
    //  pointed to by nodes processed in the last round in the same way. Recurse.

    while (!queued_vertices.empty()) {
        // Get next vertex to process and pop it from queue.
        mfast::ASTVertex vertex = queued_vertices.front();
        queued_vertices.pop();

        // Iterate all out edges of the vertex we are processing, if any of the
        // pointed-to vertices are not in the processed_vertices hashmap, then we need
        // to add the vertex we are processing back onto the end of the queue - a
        // necessary bit of IR hasn't been generated yet.
        bool can_process_vertex = true;
        for (auto edge : boost::make_iterator_range(boost::out_edges(vertex, ast))) {
            mfast::ASTVertex linked_to_vertex = boost::target(edge);

            auto it = processed_vertices.find(linked_to_vertex);
            if (it == processed_vertices.end()) {
                can_process_vertex = false;
                queued_vertices.push(vertex);
            }
        }

        // If vertex had to be added back onto the queue, then don't process it and test
        // the next vertex.
        if (!can_process_vertex) continue;

        // Visit node info with the LLVM IR converter.
        std::visit(
            LLVM_IR_Converter{ &context, &builder, &module },
            nodes.get_node_info(vertex)
        );
    }

    // const auto func_type = llvm::FunctionType::get(builder.getVoidTy(), false);
    // const auto main_func = llvm::Function::Create(
    //     func_type, llvm::Function::ExternalLinkage, "main", &module
    // );

    // const auto entry = llvm::BasicBlock::Create(context, "entrypoint", main_func);
    // builder.SetInsertPoint(entry);

    // const std::vector<llvm::Type*> puts_args = { builder.getInt8Ty()->getPointerTo()
    // }; const llvm::ArrayRef           puts_args_ref(puts_args);

    // const auto puts_type
    //     = llvm::FunctionType::get(builder.getInt32Ty(), puts_args_ref, false);
    // const auto puts_func = module.getOrInsertFunction("puts", puts_type);

    // auto str = builder.CreateGlobalStringPtr("Hello LLVM!\n");

    // builder.CreateCall(puts_func, { str });
    // builder.CreateRetVoid();

    // module.dump();

    // std::cout << std::endl;
}
