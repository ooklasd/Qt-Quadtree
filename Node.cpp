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

	tree::Node* Node::addPoint(const Vector3d*  point)
	{
		auto local = _seperator.local(*point);
		if ((!isSeperated() && _values.size() < _maxCount)
			|| local == -1)
		{
			if (local != -1)
				_isAllNoLocal = false;

			//加入到自身
			_values.push_back(point);
			return this;
		}
		else
		{
			if (!isSeperated() && !_isAllNoLocal)
			{
				seperate();
			}

			auto node = getOrCreateNode(local, *point);
			return node->addPoint(point);;
		}
	}

	Node* Node::findNode(const Vector3d& point)
	{
		auto local = _seperator.local(point);

		if (local == -1 || !isSeperated())
		{
			for (auto& p : _values)
			{
				if (*p == point)
				{
					return this;
				}
			}
		}
		else
		{
			if (auto node = _nodes[local].get())
			{
				return node->findNode(point);
			}
		}

		return nullptr;
	}

	bool Node::removePoint(const Vector3d& point)
	{
		auto local = _seperator.local(point);

		if (local == -1)
		{
			for (auto it = _values.begin(); it != _values.end(); ++it)
			{
				if (**it == point)
				{
					_values.erase(it);
					return true;
				}
			}
		}
		else
		{
			if (auto node = _nodes[local].get())
			{
				return node->removePoint(point);
			}
		}

		return nullptr;
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

	void Node::seperate()
	{
		for (auto it = _values.begin(); it != _values.end(); )
		{
			auto& point = *it;
			auto local = _seperator.local(*point);
			if (local != -1)
			{
				getOrCreateNode(local, *point)->addPoint(point);
				it = _values.erase(it);
			}
			else
			{
				++it;
			}
		}
		_isAllNoLocal = true;
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


