#pragma once
#ifndef NAVIGATIONMESH_H
#define NAVIGATIONMESH_H

#include "../Maths/MathVector3.hpp"

#include "NavigationPolygon.h"

namespace AI
{
	class INavigationMesh
	{

	public:

		virtual ~INavigationMesh( ) { };

		virtual Maths::MathVector3::MathVector3List FindPath( const Maths::MathVector3& start, const Maths::MathVector3& finish ) = 0;

		virtual void Render( ) = 0;

		virtual void AddPolygon( NavigationPolygon* polygon ) = 0;
	};
};

#endif