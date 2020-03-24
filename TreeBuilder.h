#pragma once
#include "QVector"
#include "QVector3D"

namespace tree
{

	class TreeBuilder
	{
	public:
		TreeBuilder();

		using Vector3d = QVector3D;


		QVector<Vector3d> _points;
	};
}