//
//  ShapeIntersection.h
//  Chilli Source
//  Created by Scott Downie on 03/11/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifndef _CHILLISOURCE_CORE_MATH_SHAPE_INTERSECTION_H_
#define _CHILLISOURCE_CORE_MATH_SHAPE_INTERSECTION_H_

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
			bool Intersects(const Circle& inCircle, const Vector2& invPoint);
            //----------------------------------------------------------------
            /// Line vs Line
            //----------------------------------------------------------------
			bool Intersects(const Line& inLineLHS, const Line& inLineRHS, Vector3& outvIntersection);
			//----------------------------------------------------------------
			/// Rect vs Rect
			//----------------------------------------------------------------
			bool Intersects(const Rectangle& inRectLHS, const Rectangle& inRectRHS);
			//----------------------------------------------------------------
			/// Rect vs Point
			//----------------------------------------------------------------
			bool Intersects(const Rectangle& inRect, const Vector2& invPoint);
			//----------------------------------------------------------------
			/// Ray vs Slab
			//----------------------------------------------------------------
			bool RaySlabIntersect(f32 infStart, f32 infDir, f32 infMin, f32 infMax, f32& outfFirstT, f32& outfLastT);
		};
	}
}

#endif

