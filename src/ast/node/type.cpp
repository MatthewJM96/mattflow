#include "stdafx.h"

#include "ast/node/type.h"

#if defined(MATTFLOW_OS_MAC)
const mftype::Type mfast::BoolNode::TYPE    = mftype::PrimitiveType::BOOL;
const mftype::Type mfast::CharNode::TYPE    = mftype::PrimitiveType::CHAR;
const mftype::Type mfast::IntNode::TYPE     = mftype::PrimitiveType::INT;
const mftype::Type mfast::Int8Node::TYPE    = mftype::PrimitiveType::INT8;
const mftype::Type mfast::Int16Node::TYPE   = mftype::PrimitiveType::INT16;
const mftype::Type mfast::Int32Node::TYPE   = mftype::PrimitiveType::INT32;
const mftype::Type mfast::Int64Node::TYPE   = mftype::PrimitiveType::INT64;
const mftype::Type mfast::UIntNode::TYPE    = mftype::PrimitiveType::UINT;
const mftype::Type mfast::UInt8Node::TYPE   = mftype::PrimitiveType::UINT8;
const mftype::Type mfast::UInt16Node::TYPE  = mftype::PrimitiveType::UINT16;
const mftype::Type mfast::UInt32Node::TYPE  = mftype::PrimitiveType::UINT32;
const mftype::Type mfast::UInt64Node::TYPE  = mftype::PrimitiveType::UINT64;
const mftype::Type mfast::Float32Node::TYPE = mftype::PrimitiveType::FLOAT32;
const mftype::Type mfast::Float64Node::TYPE = mftype::PrimitiveType::FLOAT64;
#endif
