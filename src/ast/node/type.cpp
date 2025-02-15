#include "stdafx.h"

#include "ast/node/type.h"

#if defined(MATTFLOW_OS_MAC)
mfast::BoolNode::TYPE    = mftype::PrimitiveType::BOOL;
mfast::CharNode::TYPE    = mftype::PrimitiveType::CHAR;
mfast::IntNode::TYPE     = mftype::PrimitiveType::INT;
mfast::Int8Node::TYPE    = mftype::PrimitiveType::INT8;
mfast::Int16Node::TYPE   = mftype::PrimitiveType::INT16;
mfast::Int32Node::TYPE   = mftype::PrimitiveType::INT32;
mfast::Int64Node::TYPE   = mftype::PrimitiveType::INT64;
mfast::UIntNode::TYPE    = mftype::PrimitiveType::UINT;
mfast::UInt8Node::TYPE   = mftype::PrimitiveType::UINT8;
mfast::UInt16Node::TYPE  = mftype::PrimitiveType::UINT16;
mfast::UInt32Node::TYPE  = mftype::PrimitiveType::UINT32;
mfast::UInt64Node::TYPE  = mftype::PrimitiveType::UINT64;
mfast::Float32Node::TYPE = mftype::PrimitiveType::Float32;
mfast::Float64Node::TYPE = mftype::PrimitiveType::Float64;
#endif
