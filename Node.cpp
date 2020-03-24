#include "stdafx.h"
#include "Node.h"
#include "NodeVisitor.h"
#include <assert.h>

namespace tree
{

	Node::Node(Seperator s)
		:_seperator(s)
	{

	}

	tree::Node* Node::addPoint(Vector3d point)
	{
		auto local = _seperator.local(point);
		if ((!isSeperated() && _values.size() < _maxCount)
			|| local == -1)
		{
			//加入到自身
			_values.push_back(point);
			return this;
		}
		else
		{
			if (!isSeperated())
			{
				//若数量过大，则先分割本身
				auto temp = std::move(_values);
				for (auto& p : temp)
				{
					addPoint(p);
				}
			}

			auto node = getOrCreateNode(local, point);
			return node->addPoint(point);
		}
	}

	Node* Node::findNode(const Vector3d& point)
	{
		for (auto it = _values.begin(); it != _values.end(); ++it)
		{
			if (*it == point)
			{
				_values.erase(it);
				return this;
			}
		}

		for (auto& node : _nodes)
		{
			if (node == nullptr) continue;
			if (node->findNode(point))
				return node.get();
		}

		return nullptr;
	}

	bool Node::removePoint(const Vector3d& point)
	{
		for (auto it = _values.begin(); it != _values.end(); ++it)
		{
			if (*it == point)
			{
				_values.erase(it);
				return true;
			}
		}

		for (auto& node : _nodes)
		{
			if (node == nullptr) continue;
			if (node->removePoint(point))
				return true;
		}

		return false;
	}

	size_t Node::count() const
	{
		size_t  sum = 0;
		for (auto& node : _nodes)
		{
			if (node == nullptr) continue;
			sum += node->count();
		}
		return sum + _values.size();
	}

	void Node::accept(NodeVisitor* visitor)
	{
		visitor->apply(*this);
	}

	void Node::traverse(NodeVisitor* visitor)
	{
		//遍历子节点
		for (auto& node : _nodes)
		{
			if (node != nullptr)
			{
				node->accept(visitor);
			}
		}
	}

	Node* Node::getOrCreateNode(int local, const Vector3d& point)
	{
		if (_nodes[local] == nullptr)
		{
			_nodes[local].swap(std::make_unique<Node>(Seperator(point)));
		}
		return _nodes[local].get();
	}

}


