#include "stdafx.h"
#include "Seperator.h"

namespace tree
{

	Seperator::Seperator(Vector3d point) :_point(point)
	{
		_dirs[0] = Vector3d(1, 0, 0);
		_dirs[1] = Vector3d(0, 1, 0);
	}

	int Seperator::local(const Vector3d& point)const
	{
		int ret = 0;

		auto vpoint = point - _point;
		
		//标志位,若000  可能存在四种情况00 01 10 11
		int mark = 1;
		bool isOverAll = true;
		for (auto& dir : _dirs)
		{
			//计算点到面的距离
			auto distance = point.distanceToPlane(_point, dir);

			//若在反面 位置则+1
			if (distance < 0)
				ret |= mark;

			isOverAll = isOverAll && fabs(distance) < 10e-6;

			mark = mark << 1;
		}
		return isOverAll?-1:ret;
	}
}

