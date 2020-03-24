#pragma once


namespace tree
{
	class Node;

	class NodeVisitor
	{
	public:
		NodeVisitor();
		~NodeVisitor(){}

		virtual void apply(Node& node);

	protected:
		void traverse(Node& node);
	};
}
