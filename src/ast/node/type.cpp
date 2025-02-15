#include "stdafx.h"

#include "ast/node/type.h"

#if defined(MATTFLOW_OS_MAC)
mftype::Type mfast::BoolNode::TYPE    = mftype::PrimitiveType::BOOL;
mftype::Type mfast::CharNode::TYPE    = mftype::PrimitiveType::CHAR;
mftype::Type mfast::IntNode::TYPE     = mftype::PrimitiveType::INT;
mftype::Type mfast::Int8Node::TYPE    = mftype::PrimitiveType::INT8;
mftype::Type mfast::Int16Node::TYPE   = mftype::PrimitiveType::INT16;
mftype::Type mfast::Int32Node::TYPE   = mftype::PrimitiveType::INT32;
mftype::Type mfast::Int64Node::TYPE   = mftype::PrimitiveType::INT64;
mftype::Type mfast::UIntNode::TYPE    = mftype::PrimitiveType::UINT;
mftype::Type mfast::UInt8Node::TYPE   = mftype::PrimitiveType::UINT8;
mftype::Type mfast::UInt16Node::TYPE  = mftype::PrimitiveType::UINT16;
mftype::Type mfast::UInt32Node::TYPE  = mftype::PrimitiveType::UINT32;
mftype::Type mfast::UInt64Node::TYPE  = mftype::PrimitiveType::UINT64;
mftype::Type mfast::Float32Node::TYPE = mftype::PrimitiveType::Float32;
mftype::Type mfast::Float64Node::TYPE = mftype::PrimitiveType::Float64;
#endif
