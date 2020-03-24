#include "stdafx.h"
#include "NodeVisitor.h"
#include "Node.h"



void tree::NodeVisitor::apply(Node& node)
{
	traverse(node);
}

void tree::NodeVisitor::traverse(Node& node)
{
	node.traverse(this);
}
