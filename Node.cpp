#include "stdafx.h"
#include "Node.h"
#include <assert.h>
#include "QRect"

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
		//将点根据区域归类，
		std::array<std::vector<const Vector3d*>, 4> areas;

		//进行分割，若无法区分区域，则保留在列表
		for (auto it = _values.begin(); it != _values.end(); )
		{
			auto& point = *it;
			auto local = _seperator.local(*point);
			if (local != -1)
			{
				areas.at(local).push_back(*it);
				it = _values.erase(it);
			}
			else
			{
				++it;
			}
		}


		for (size_t i = 0, Length = areas.size(); i < Length; ++i)
		{
			if (areas[i].empty())
				continue;
			//计算出多个点的中心位置，以此作为分割点
			QRectF rect;
			for (auto& it : areas[i])
			{
				rect |= QRectF(it->toPointF(),QSize() );
			}
			auto center = rect.center();
			auto node = getOrCreateNode(i, Vector3d(center));
			for (auto& it : areas[i])
			{
				node->addPoint(it);
			}
		}



		_isAllNoLocal = true;
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


