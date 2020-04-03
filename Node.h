#pragma once
#include <array>
#include <memory>
#include "Seperator.h"
#include "QVector3D"

namespace tree
{
	//�Ĳ����ڵ�
	class Node
	{
	public:

		using Vector3d = QVector3D;
		using NodePtr = std::unique_ptr<Node>;

		Node(Seperator s);
		~Node(){}

		//���ӵ�
		Node* addPoint(const Vector3d* point);
		Node* findNode(const Vector3d& point);
		const Node* findNode(const Vector3d& point)const{
			return const_cast<Node *>(this)->findNode(point);
		}
		bool removePoint(const Vector3d& point);
		bool exists(const Vector3d& point) const{return !!findNode(point);}
		size_t count()const;

		//�Ƿ�ָ�
		bool isSeperated()const{ 
			for (auto& node : _nodes)
			{
				if (node) return true;
			}
			return false;
		}

		//�������зָ�
		void seperate();

		std::vector<const Vector3d*> findPoints(const Vector3d& pointMin, const Vector3d& pointMax)const;

		const std::vector<const Vector3d*>& getValues() const { return _values; }
		const std::array<NodePtr, 4>& getNodes() const { return _nodes; }
		const Seperator& getSeperator() const { return _seperator; }

	protected:
		Node* getOrCreateNode(int local, const Vector3d& point);

	protected:
		//�ռ�ָ���
		Seperator _seperator;

		//�Ĳ����ڵ�
		std::array<NodePtr, 4> _nodes;
		std::vector<const Vector3d*> _values;

		//�Ƿ�����λ�õĵ�
		bool _isAllNoLocal = false;

		//�ڵ������������������Խ�����������ᴴ���Ĳ���ϸ��
		size_t _maxCount = 100;
	};


	
}