#include "AstNode.h"

using namespace aec;

AstNode::AstNode(ValidAstNode &&node) : _node(std::forward<ValidAstNode &&>(node))
{
}
