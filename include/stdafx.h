#ifndef __mattflow_stdafx_h
#define __mattflow_stdafx_h

// Our compatibility definitions.
#include "compat.h"

// Basics
#include <cstdint>
#include <cstdlib>

// Containers
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graphviz.hpp>
#include <map>
#include <moodycamel/blockingconcurrentqueue.h>
#include <moodycamel/concurrentqueue.h>
#include <queue>
#include <set>
#include <span>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

// Views
#include <iterator>
#include <ranges>

// Strings
#include <cstring>
#include <regex>
#include <string>

// Generics
#include <algorithm>
#include <functional>
#include <limits>
#include <memory>
#include <type_traits>
#include <typeinfo>
#include <utility>

// Thread Handling
#include <mutex>
#include <shared_mutex>
#include <thread>

// Error Handling
#include <stdexcept>

// File Handling
#include <cstdio>
#include <filesystem>

// Streams
#include <fstream>
#include <iostream>
#include <sstream>

// Random
#include <random>

// LLVM
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

// Our Decorators.
#include "decorators.h"
using std::size_t;

// Our Types and Other Hints
// #include "basic_concepts.hpp"

// Our Timers.
// #include "timing.h"

// Our Thread Handling
// #include "thread/thread_pool.hpp"

#endif  // __mattflow_stdafx_h
