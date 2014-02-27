/*
 * File: ShapeIntersection.h
 * Date: 03/11/2010 2010 
 * Description: Holds intersection tests for the various shapes in Shapes.h
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_CORE_MATH_SHAPE_INTERSECTION_H_
#define _MO_FLO_CORE_MATH_SHAPE_INTERSECTION_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>

namespace ChilliSource
{
	namespace Core
	{
		namespace ShapeIntersection
		{
            enum class Result
            {
                k_outside,
                k_inside,
                k_intersect
            };
            
			//----------------------------------------------------------------
			/// AABB vs Ray
			//----------------------------------------------------------------
			bool Intersects(const AABB& inAABB, const Ray& inRay, f32 &outfT1, f32 &outfT2);
			//----------------------------------------------------------------
			/// AABB vs Point
			//----------------------------------------------------------------
			bool Intersects(const AABB& inAABB, const Vector3& invPoint);
			//----------------------------------------------------------------
			/// AABB vs AABB
			//----------------------------------------------------------------
			bool Intersects(const AABB& inAABBLHS, const AABB& inAABBRHS);
			//----------------------------------------------------------------
			/// Sphere vs Ray
			//----------------------------------------------------------------
			bool Intersects(const Sphere& inSphere, const Ray& inRay);
			//----------------------------------------------------------------
			/// Sphere vs Point
			//----------------------------------------------------------------
			bool Intersects(const Sphere& inSphere, const Vector3& invPoint);
			//----------------------------------------------------------------
			/// Sphere vs Sphere
			//----------------------------------------------------------------
			bool Intersects(const Sphere& inSphereLHS, const Sphere& inSphereRHS);
            //----------------------------------------------------------------
            /// Sphere vs Plane
            //----------------------------------------------------------------
            Result Intersects(const Sphere& inSphere, const Plane& inPlane);
            //----------------------------------------------------------------
            /// Plane vs Plane
            //----------------------------------------------------------------
            bool Intersects(const Plane& inPlaneLHS, const Plane& inPlaneRHS, Ray& outIntersection);
			//----------------------------------------------------------------
			/// Circle vs Circle
			//----------------------------------------------------------------
			bool Intersects(const Circle& inCircleLHS, const Circle& inCircleRHS);
			//----------------------------------------------------------------
			/// Circle vs Point
			//----------------------------------------------------------------
			bool Intersects(const Circle& inCircle, const Vector3& invPoint);
            //----------------------------------------------------------------
            /// Line vs Line
            //----------------------------------------------------------------
			bool Intersects(const Line& inLineLHS, const Line& inLineRHS, Vector3& outvIntersection);
			//----------------------------------------------------------------
			/// Rect vs Rect
			//----------------------------------------------------------------
			bool Intersects(const CRect& inRectLHS, const CRect& inRectRHS);
			//----------------------------------------------------------------
			/// Rect vs Point
			//----------------------------------------------------------------
			bool Intersects(const CRect& inRect, const Vector3& invPoint);
			//----------------------------------------------------------------
			/// Ray vs Slab
			//----------------------------------------------------------------
			bool RaySlabIntersect(f32 infStart, f32 infDir, f32 infMin, f32 infMax, f32& outfFirstT, f32& outfLastT);
		};
	}
}

#endif

