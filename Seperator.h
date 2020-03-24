#pragma once
#include "QVector3D"
#include <array>

namespace tree
{
	//�ռ�ָ���
	struct Seperator
	{
	public:
		using Vector3d = QVector3D;
		Seperator(Vector3d point);

		//��λ��ֵ�ڽڵ��λ�ã�Ӧ�÷���0~3��Ϊ������,-1��Ϊ���ĸ�����
		int local(const Vector3d& point)const;

		//�м䲿��
		Vector3d _point;
		//���򣬺�_point�����棬�Ӷ����пռ仮��
		std::array<Vector3d, 2> _dirs;
	};
}

