#pragma once
#include <array>
#include <memory>
#include "Seperator.h"
#include "QVector3D"

namespace tree
{
	//四叉树节点
	class Node
	{
	public:

		using Vector3d = QVector3D;
		using NodePtr = std::unique_ptr<Node>;

		Node(Seperator s);
		~Node(){}

		//增加点
		Node* addPoint(const Vector3d* point);
		Node* findNode(const Vector3d& point);
		const Node* findNode(const Vector3d& point)const{
			return const_cast<Node *>(this)->findNode(point);
		}
		bool removePoint(const Vector3d& point);
		bool exists(const Vector3d& point) const{return !!findNode(point);}
		size_t count()const;

		//是否分割
		bool isSeperated()const{ 
			for (auto& node : _nodes)
			{
				if (node) return true;
			}
			return false;
		}

		//主动进行分割
		void seperate();

		std::vector<const Vector3d*> findPoints(const Vector3d& pointMin, const Vector3d& pointMax)const;

		const std::vector<const Vector3d*>& getValues() const { return _values; }
		const std::array<NodePtr, 4>& getNodes() const { return _nodes; }
		const Seperator& getSeperator() const { return _seperator; }

	protected:
		Node* getOrCreateNode(int local, const Vector3d& point);

	protected:
		//空间分割器
		Seperator _seperator;

		//四叉树节点
		std::array<NodePtr, 4> _nodes;
		std::vector<const Vector3d*> _values;

		//是否都是无位置的点
		bool _isAllNoLocal = false;

		//节点最大数量，若数量超越最大数量，则会创建四叉树细分
		size_t _maxCount = 100;
	};


	
}