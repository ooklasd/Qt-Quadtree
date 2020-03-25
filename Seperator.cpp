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
		
		//��־λ,��000  ���ܴ����������00 01 10 11
		int mark = 1;
		bool isOverAll = true;
		for (auto& dir : _dirs)
		{
			//����㵽��ľ���
			auto distance = point.distanceToPlane(_point, dir);

			//���ڷ��� λ����+1
			if (distance < 0)
				ret |= mark;

			isOverAll = isOverAll && fabs(distance) < 10e-6;

			mark = mark << 1;
		}
		return isOverAll?-1:ret;
	}
}

