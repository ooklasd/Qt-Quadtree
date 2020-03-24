#pragma once
#include "QVector3D"
#include <array>

namespace tree
{
	//空间分割器
	struct Seperator
	{
	public:
		using Vector3d = QVector3D;
		Seperator(Vector3d point);

		//定位该值在节点的位置，应该返回0~3则为象限序,-1则为夸四个象限
		int local(const Vector3d& point)const;

		//中间部分
		Vector3d _point;
		//方向，和_point构成面，从而进行空间划分
		std::array<Vector3d, 2> _dirs;
	};
}

