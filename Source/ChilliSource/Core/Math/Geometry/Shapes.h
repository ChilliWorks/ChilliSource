//
//  Shapes.h
//  Chilli Source
//  Created by Scott Downie on 04/10/2010.
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

#ifndef _CHILLISOURCE_CORE_MATHS_SHAPES_H_
#define _CHILLISOURCE_CORE_MATHS_SHAPES_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Quaternion.h>

namespace ChilliSource
{
	namespace Core 
	{
		//---Defaults
		const f32 kDefaultRayLength = 5000.0f;
		
		//================================================
		/// Rect
		///
		/// Container for a rectangle with it's origin 
		/// at the centre and Y-up
		//================================================
		class Rectangle
		{
		public:	
			Rectangle() = default;
			Rectangle(const Vector2 &invOrigin, const Vector2 &invSize);
            //-----------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param Left
            /// @param Top
            /// @param Right
            /// @param Bottom
            //-----------------------------------------------
            Rectangle(f32 in_left, f32 in_top, f32 in_right, f32 in_bottom);
			
			//-----------------------------------------------
			/// Left
			///
			/// @return Left side of rectangle
			//-----------------------------------------------
			f32 Left() const;
			//-----------------------------------------------
			/// Right
			///
			/// @return Right side of rectangle
			//-----------------------------------------------
			f32 Right() const;
			//-----------------------------------------------
			/// Top
			///
			/// @return Top side of rectangle
			//-----------------------------------------------
			f32 Top() const;
			//-----------------------------------------------
			/// Bottom
			///
			/// @return Bottom side of rectangle
			//-----------------------------------------------
			f32 Bottom() const;
			//-----------------------------------------------
			/// Top Left
			///
			/// @return Top left co-ordinate
			//-----------------------------------------------
            Vector2 TopLeft() const;
			//-----------------------------------------------
			/// Top Right
			///
			/// @return Top right co-ordinate
			//-----------------------------------------------
            Vector2 TopRight() const;
			//-----------------------------------------------
			/// Bottom Left
			///
			/// @return Bottom left co-ordinate
			//-----------------------------------------------
            Vector2 BottomLeft() const;
			//-----------------------------------------------
			/// Bottom Right
			///
			/// @return Bottom right co-ordinate
			//-----------------------------------------------
            Vector2 BottomRight() const;
			//-----------------------------------------------
			/// Centre
			///
			/// @return Centre point
			//-----------------------------------------------
            Vector2 Centre() const;
			//-----------------------------------------------
			/// Contains (Point)
			///
			/// @param Point in 2D space
			/// @return Whether point intersects the rect
			//-----------------------------------------------
			bool Contains(const Vector2 & invPoint) const;
			//-----------------------------------------------
			/// Contains (Rect)
			///
			/// @param Rect in 2D space
			/// @return Whether rect intersects the rect
			//-----------------------------------------------
			bool Contains(const Rectangle& inRect) const;
            
			///Top Left
			Vector2 vOrigin;
			///Width and height
			Vector2 vSize;
		};
		//================================================
		/// Circle
		///
		/// Container for a circle with it's origin 
		/// at centre
		//================================================
		class Circle
		{
		public:
			Circle(){}
			Circle(const Vector2 &invOrigin, const f32 infRadius);
			//-----------------------------------------------
			/// Contains (Point)
			///
			/// @param Point in 2D space
			/// @return Whether point intersects the circle
			//-----------------------------------------------
			bool Contains(const Vector2 & invPoint) const;
			//-----------------------------------------------
			/// Contains (Circle)
			///
			/// @param Circle in 2D space
			/// @return Whether circle intersects the circle
			//-----------------------------------------------
			bool Contains(const Circle & inCircle) const;
			
			///Centre				   
			Vector2 vOrigin;
			f32 fRadius;
		};
		//================================================
		/// Sphere
		///
		/// Container for a sphere with it's origin 
		/// at centre
		//================================================
		class Sphere
		{
        public:
			Sphere(){}
			Sphere(const Vector3 &invOrigin, const f32 infRadius);
			//-----------------------------------------------
			/// Contains (Point)
			///
			/// @param Point in 2D space
			/// @return Whether point intersects the circle
			//-----------------------------------------------
			bool Contains(const Vector3 & invPoint) const;
			//-----------------------------------------------
			/// Contains (Ray)
			///
			/// @param Point in 2D space
			/// @return Whether point intersects the sphere
			//-----------------------------------------------
			bool Contains(const Ray &inRay) const;
			//-----------------------------------------------
			/// Contains (Sphere)
			///
			/// @param Sphere in 3D space
			/// @return Whether sphere intersects the sphere
			//-----------------------------------------------
			bool Contains(const Sphere & inSphere) const;
			
			///Centre				   
			Vector3 vOrigin;
			f32 fRadius;
		};
		//================================================
		/// AABB
		///
		/// Container for a axis-aligned bounding box 
		/// with it's origin at centre
		//================================================
		class AABB
		{
		public:
			AABB(){}
			AABB(const Vector3 &invOrigin, const Vector3 &invSize);
			//-----------------------------------------------
			/// Front Top Left
			///
			/// @return Top left co-ordinate
			//-----------------------------------------------
			const Vector3 FrontTopLeft() const;
			//-----------------------------------------------
			/// Front Top Right
			///
			/// @return Top right co-ordinate
			//-----------------------------------------------
			const Vector3 FrontTopRight() const;
			//-----------------------------------------------
			/// Front Bottom Left
			///
			/// @return Lower left co-ordinate
			//-----------------------------------------------
			const Vector3 FrontBottomLeft() const;
			//-----------------------------------------------
			/// Front Bottom Right
			///
			/// @return Lower right co-ordinate
			//-----------------------------------------------
			const Vector3 FrontBottomRight() const;
			//-----------------------------------------------
			/// Back Top Left
			///
			/// @return Top left co-ordinate
			//-----------------------------------------------
			const Vector3 BackTopLeft() const;
			//-----------------------------------------------
			/// Back Top Right
			///
			/// @return Top right co-ordinate
			//-----------------------------------------------
			const Vector3 BackTopRight() const;
			//-----------------------------------------------
			/// Back Bottom Left
			///
			/// @return Lower left co-ordinate
			//-----------------------------------------------
			const Vector3 BackBottomLeft() const;
			//-----------------------------------------------
			/// Back Bottom Right
			///
			/// @return Lower right co-ordinate
			//-----------------------------------------------
			const Vector3 BackBottomRight() const;
			//-----------------------------------------------
			/// Centre
			///
			/// @return Centre point
			//-----------------------------------------------
			const Vector3& Centre() const;
            //-----------------------------------------------
			/// Get Origin
			///
			/// @return Centre point of AABB
			//-----------------------------------------------
			inline const Vector3& GetOrigin() const {return mvOrigin;};
			//-----------------------------------------------
			/// Set Origin
			///
			/// @param Centre point of AABB
			//-----------------------------------------------
			void SetOrigin(const Vector3 &invOrigin);
            //-----------------------------------------------
			/// Get Size
			///
			/// @return Dimensions of AABB
			//-----------------------------------------------
			inline const Vector3& GetSize() const {return mvSize;};
			//-----------------------------------------------
			/// Set Size
			///
			/// @param Dimensions of AABB
			//-----------------------------------------------
			void SetSize(const Vector3 &invSize);
            //-----------------------------------------------
			/// Get Half Size
			///
			/// @return Half Dimensions of AABB
			//-----------------------------------------------
			inline const Vector3& GetHalfSize() const {return mvHalfSize;};
            //-----------------------------------------------
			/// Get Min
			///
			/// @return Mininmum coords of AABB
			//-----------------------------------------------
			inline const Vector3& GetMin() const {return mvMin;};
            //-----------------------------------------------
			/// Get Max
			///
			/// @return Maximum coords of AABB
			//-----------------------------------------------
			inline const Vector3& GetMax() const {return mvMax;};
			//-----------------------------------------------
			/// Calculate Min and Max
			///
			/// Refreshes the bounds
			//-----------------------------------------------
			void CalculateMinandMax();
			//-----------------------------------------------
			/// Contains (Point)
			///
			/// @param Point in 3D space
			/// @return Whether point intersects the AABB
			//-----------------------------------------------
			bool Contains(const Vector3 &invPoint) const;
			//-----------------------------------------------
			/// Contains (AABB)
			///
			/// @param AABB in 3D space
			/// @return Whether AABB intersects the AABB
			//-----------------------------------------------
			bool Contains(const AABB &inAABB) const;
			//-----------------------------------------------
			/// Contains (Ray)
			///
			/// @param Ray in 3D space
			/// @param Near point of intersection
			/// @param Far point of intersection
			/// @return Whether ray intersects the AABB
			//-----------------------------------------------
			bool Contains(const Ray &inRay, f32 &outfT1, f32 &outfT2) const;
            //-----------------------------------------------
			/// Union
			///
            /// @param AABB to union with this
			/// @return Unions two AABBs together find the composite size.
			//-----------------------------------------------
			AABB Union(const AABB& inAABB);
            
            private:
			///Centre
			Vector3 mvOrigin;
			///Dimensions (width, height, breadth)
			Vector3 mvSize;
			Vector3 mvHalfSize;
			Vector3 mvMin;
			Vector3 mvMax;
		};
		//================================================
		/// OOBB
		///
		/// Container for a object oriented bounding box 
		/// with it's origin at centre
		//================================================
		class OOBB
		{
		public:
			OOBB();
			OOBB(const Vector3 &invOrigin, const Vector3 &invSize);
			
			//----------------------------------------------- 
			/// Set Origin
			///
			/// @param X
			/// @param Y
			/// @param Z
			//-----------------------------------------------
			void SetOrigin(const f32 infX, const f32 infY, const f32 infZ = 0.0f);
			//----------------------------------------------- 
			/// Set Origin
			///
			/// @param Local origin of the object
			//-----------------------------------------------
			void SetOrigin(const Vector3 &invPos);
			//----------------------------------------------- 
			/// Get Origin
			///
			/// @return Local origin of the object
			//-----------------------------------------------
			const Vector3& GetOrigin() const;
			//----------------------------------------------- 
			/// Set Size
			///
			/// @param Local dimensions of the object
			//-----------------------------------------------
			void SetSize(const Vector3& invSize);
			//----------------------------------------------- 
			/// Get Size
			///
			/// @return Local dimensions of the object
			//-----------------------------------------------
			const Vector3& GetSize() const;			
			//----------------------------------------------- 
			/// Get Transform
			///
			/// Get the model matrix that transforms this
			/// object to world space
			/// @return To world matrix
			//-----------------------------------------------
			const Matrix4 & GetTransform() const;
			//----------------------------------------------- 
			/// Set Transform
			///
			/// Set the matrix that transforms this object
			/// to world space
			/// @param To world matrix
			//-----------------------------------------------
			void SetTransform(const Matrix4& inmatTransform);
			
			//-----------------------------------------------
			/// Contains (Point)
			///
			/// @param Point in 3D space
			/// @return Whether point intersects the OOBB
			//-----------------------------------------------
			bool Contains(const Vector3 &invPoint) const;
			//-----------------------------------------------
			/// Contains (Ray)
			///
			/// @param Ray in 3D space
			/// @param Near point of intersection
			/// @param Far point of intersection
			/// @return Whether ray intersects the OOBB
			//-----------------------------------------------
			bool Contains(const Ray &inRay, f32 &outfT1, f32 &outfT2) const;
			
		private:
			
			///AABB in Local Space
            AABB mHitBox;
			
            Matrix4 mmatLocal;
		};
		//===============================================
		/// Ray
		///
		/// Container for a ray
		//===============================================
		class Ray
		{
		public:
			Ray(){}
			Ray(const Vector3 &invOrigin, const Vector3 &invDirection, const f32 infLength = kDefaultRayLength);
			
			Vector3 vOrigin;
			Vector3 vDirection;
			
			//----------------------------------------------- 
			/// Get Point
			///
			/// Gets the position of a point t units 
			/// along the ray.
			/// @param A value t along parametric ray
			/// @return Point on ray
			//-----------------------------------------------
			Vector3 GetPoint(f32 t) const;
			
			f32 DistanceFromPoint(const Vector3 &invOrigin) const;
			
			f32 fLength;
		};
        //===============================================
        /// Line
        ///
        /// Shape described by a start and end position
        //===============================================
		class Line
        {
		public:
            Line(){}
            Line(const Vector3 &invStartPos, const Vector3 &invEndPos)
            : vStartPos(invStartPos), vEndPos(invEndPos)
            {}
            Vector3 vStartPos;
            Vector3 vEndPos;
        };
		//===============================================
		/// Plane
		///
		/// Container for a plane
		//===============================================
		class Plane
		{
		public:
			Plane(){}
			Plane(const Vector3& incOrigin, const Vector3& incNormal);
			Plane(f32 a, f32 b, f32 c, f32 d);
			Plane(const Vector3& incNormal, f32 d);
			
			f32 DistanceFromPoint(const Vector3& invPoint) const;
			bool Intersects(const Ray& incRay) const;

			bool GetIsRayIntersecting(const Ray& incRay, Vector3& outcIntersect) const;
	
			Vector3 mvNormal;
			f32 mfD;
		};

		//===============================================
		/// Frustum
		///
		/// Container for a frustum
		//===============================================
		class Frustum
		{
		public:

			//----------------------------------------------------------
			/// Calculate Clipping Planes
			///
			/// Build the 6 planes of the frustum based on the camera
			/// view projection matrix
			///
			/// @param View projection matrix
			//----------------------------------------------------------
			void CalculateClippingPlanes(const Matrix4& inmatViewProj);
			//----------------------------------------------------------
			/// Sphere Cull Test
			///
			/// Test if the bounding sphere lies within the frustum
			/// and determine whether it should be culled
			///
			/// @param Sphere
			/// @return Whether it lies within the bounds
			//-----------------------------------------------------------
			bool SphereCullTest(const Sphere& inBoundingSphere) const;

		public:

			Plane mLeftClipPlane;
			Plane mRightClipPlane;

			Plane mTopClipPlane;
			Plane mBottomClipPlane;

			Plane mNearClipPlane;
			Plane mFarClipPlane;
		};
	}
}

#endif //_CHILLISOURCE_CORE_MATHS_SHAPES_H_
