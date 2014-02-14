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

#include <ChilliSource/Core/Math/Geometry/Shapes.h>

namespace moFlo
{
	namespace Core
	{
		class CIntersection
		{
		public:
            
            enum Result
            {
                OUTSIDE,
                INSIDE,
                INTERSECT,
            };
            
			//----------------------------------------------------------------
			/// AABB vs Ray
			//----------------------------------------------------------------
			static bool Intersects(const AABB& inAABB, const Ray& inRay, f32 &outfT1, f32 &outfT2);
			//----------------------------------------------------------------
			/// AABB vs Point
			//----------------------------------------------------------------
			static bool Intersects(const AABB& inAABB, const CVector3& invPoint);
			//----------------------------------------------------------------
			/// AABB vs AABB
			//----------------------------------------------------------------
			static bool Intersects(const AABB& inAABBLHS, const AABB& inAABBRHS);
			
			//----------------------------------------------------------------
			/// OOBB vs Ray
			//----------------------------------------------------------------
			static bool Intersects(const OOBB& inOOBB, const Ray& inRay);
			
			//----------------------------------------------------------------
			/// Sphere vs Ray
			//----------------------------------------------------------------
			static bool Intersects(const Sphere& inSphere, const Ray& inRay);
			//----------------------------------------------------------------
			/// Sphere vs Point
			//----------------------------------------------------------------
			static bool Intersects(const Sphere& inSphere, const CVector3& invPoint);
			//----------------------------------------------------------------
			/// Sphere vs Sphere
			//----------------------------------------------------------------
			static bool Intersects(const Sphere& inSphereLHS, const Sphere& inSphereRHS);
            //----------------------------------------------------------------
            /// Sphere vs Plane
            //----------------------------------------------------------------
            static CIntersection::Result Intersects(const Sphere& inSphere, const Plane& inPlane);
            //----------------------------------------------------------------
            /// Plane vs Plane
            //----------------------------------------------------------------
            static bool Intersects(const Plane& inPlaneLHS, const Plane& inPlaneRHS, Ray& outIntersection);
			//----------------------------------------------------------------
			/// Circle vs Circle
			//----------------------------------------------------------------
			static bool Intersects(const Circle& inCircleLHS, const Circle& inCircleRHS);
			//----------------------------------------------------------------
			/// Circle vs Point
			//----------------------------------------------------------------
			static bool Intersects(const Circle& inCircle, const CVector3& invPoint);
            //----------------------------------------------------------------
            /// Line vs Line
            //----------------------------------------------------------------
			static bool Intersects(const Line& inLineLHS, const Line& inLineRHS, CVector3& outvIntersection);
			//----------------------------------------------------------------
			/// Rect vs Rect
			//----------------------------------------------------------------
			static bool Intersects(const CRect& inRectLHS, const CRect& inRectRHS);
			//----------------------------------------------------------------
			/// Rect vs Point
			//----------------------------------------------------------------
			static bool Intersects(const CRect& inRect, const CVector3& invPoint);
			
		private:
			
			//----------------------------------------------------------------
			/// Ray vs Slab
			//----------------------------------------------------------------
			static bool RaySlabIntersect(f32 infStart, f32 infDir, f32 infMin, f32 infMax, f32& outfFirstT, f32& outfLastT);
		};
	}
}

#endif

