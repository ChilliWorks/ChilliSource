/*
 *  Shapes.h
 *  moFloTest
 *
 *  Created by Scott Downie on 04/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_CORE_MATHS_SHAPES_H_
#define _MOFLO_CORE_MATHS_SHAPES_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2Old.h>
#include <ChilliSource/Core/Math/Vector3Old.h>
#include <ChilliSource/Core/Math/QuaternionOld.h>

namespace ChilliSource
{
	namespace Core 
	{
		//---Defaults
		const f32 kDefaultRayLength = 5000.0f;
		
		//================================================
		/// UV-Rect
		///
		/// Container for a rectangle with it's origin 
		/// at top left and using shorts instead of floats
		//================================================
		class UVRect
		{
		public:	
			UVRect();
			UVRect(s16 wOx, s16 wOy, s16 wWidth, s16 wHeight);

			///Top Left
			s16 mwOriginX, mwOriginY;
			///Width and height
			s16 mwWidth, mwHeight;
		};
		//================================================
		/// Rect
		///
		/// Container for a rectangle with it's origin 
		/// at top left
		//================================================
		class Rectangle
		{
		public:	
			Rectangle(){}
			Rectangle(const Vector2Old &invOrigin, const Vector2Old &invSize);
			
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
			const Vector2Old& TopLeft() const;
			//-----------------------------------------------
			/// Top Right
			///
			/// @return Top right co-ordinate
			//-----------------------------------------------
			const Vector2Old TopRight() const;
			//-----------------------------------------------
			/// Bottom Left
			///
			/// @return Bottom left co-ordinate
			//-----------------------------------------------
			const Vector2Old BottomLeft() const;
			//-----------------------------------------------
			/// Bottom Right
			///
			/// @return Bottom right co-ordinate
			//-----------------------------------------------
			const Vector2Old BottomRight() const;
			//-----------------------------------------------
			/// Centre
			///
			/// @return Centre point
			//-----------------------------------------------
			const Vector2Old Centre() const;
			//-----------------------------------------------
			/// Contains (Point)
			///
			/// @param Point in 2D space
			/// @return Whether point intersects the rect
			//-----------------------------------------------
			bool Contains(const Vector2Old & invPoint) const;
			//-----------------------------------------------
			/// Contains (Rect)
			///
			/// @param Rect in 2D space
			/// @return Whether rect intersects the rect
			//-----------------------------------------------
			bool Contains(const Rectangle& inRect) const;
			
			
			//-----------------------------------------------
			/// Normalised
			///
			/// @return A copy of this rect which will have no negative components in its size
			//-----------------------------------------------
			Rectangle Normalised() const;
			//-----------------------------------------------
			/// Union
			///
            /// @param Rectangle to union with this
			/// @return Unions two rectangles together find the composite size.
			//-----------------------------------------------
			Rectangle Union(const Rectangle& inRect);
            
			///Top Left
			Vector2Old vOrigin;
			///Width and height
			Vector2Old vSize;
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
			Circle(const Vector2Old &invOrigin, const f32 infRadius);
			//-----------------------------------------------
			/// Contains (Point)
			///
			/// @param Point in 2D space
			/// @return Whether point intersects the circle
			//-----------------------------------------------
			bool Contains(const Vector2Old & invPoint) const;
			//-----------------------------------------------
			/// Contains (Circle)
			///
			/// @param Circle in 2D space
			/// @return Whether circle intersects the circle
			//-----------------------------------------------
			bool Contains(const Circle & inCircle) const;
			
			///Centre				   
			Vector2Old vOrigin;
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
			Sphere(const Vector3Old &invOrigin, const f32 infRadius);
			//-----------------------------------------------
			/// Contains (Point)
			///
			/// @param Point in 2D space
			/// @return Whether point intersects the circle
			//-----------------------------------------------
			bool Contains(const Vector3Old & invPoint) const;
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
			Vector3Old vOrigin;
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
			AABB(const Vector3Old &invOrigin, const Vector3Old &invSize);
			//-----------------------------------------------
			/// Front Top Left
			///
			/// @return Top left co-ordinate
			//-----------------------------------------------
			const Vector3Old FrontTopLeft() const;
			//-----------------------------------------------
			/// Front Top Right
			///
			/// @return Top right co-ordinate
			//-----------------------------------------------
			const Vector3Old FrontTopRight() const;
			//-----------------------------------------------
			/// Front Bottom Left
			///
			/// @return Lower left co-ordinate
			//-----------------------------------------------
			const Vector3Old FrontBottomLeft() const;
			//-----------------------------------------------
			/// Front Bottom Right
			///
			/// @return Lower right co-ordinate
			//-----------------------------------------------
			const Vector3Old FrontBottomRight() const;
			//-----------------------------------------------
			/// Back Top Left
			///
			/// @return Top left co-ordinate
			//-----------------------------------------------
			const Vector3Old BackTopLeft() const;
			//-----------------------------------------------
			/// Back Top Right
			///
			/// @return Top right co-ordinate
			//-----------------------------------------------
			const Vector3Old BackTopRight() const;
			//-----------------------------------------------
			/// Back Bottom Left
			///
			/// @return Lower left co-ordinate
			//-----------------------------------------------
			const Vector3Old BackBottomLeft() const;
			//-----------------------------------------------
			/// Back Bottom Right
			///
			/// @return Lower right co-ordinate
			//-----------------------------------------------
			const Vector3Old BackBottomRight() const;
			//-----------------------------------------------
			/// Centre
			///
			/// @return Centre point
			//-----------------------------------------------
			const Vector3Old& Centre() const;
            //-----------------------------------------------
			/// Get Origin
			///
			/// @return Centre point of AABB
			//-----------------------------------------------
			inline const Vector3Old& GetOrigin() const {return mvOrigin;};
			//-----------------------------------------------
			/// Set Origin
			///
			/// @param Centre point of AABB
			//-----------------------------------------------
			void SetOrigin(const Vector3Old &invOrigin);
            //-----------------------------------------------
			/// Get Size
			///
			/// @return Dimensions of AABB
			//-----------------------------------------------
			inline const Vector3Old& GetSize() const {return mvSize;};
			//-----------------------------------------------
			/// Set Size
			///
			/// @param Dimensions of AABB
			//-----------------------------------------------
			void SetSize(const Vector3Old &invSize);
            //-----------------------------------------------
			/// Get Half Size
			///
			/// @return Half Dimensions of AABB
			//-----------------------------------------------
			inline const Vector3Old& GetHalfSize() const {return mvHalfSize;};
            //-----------------------------------------------
			/// Get Min
			///
			/// @return Mininmum coords of AABB
			//-----------------------------------------------
			inline const Vector3Old& GetMin() const {return mvMin;};
            //-----------------------------------------------
			/// Get Max
			///
			/// @return Maximum coords of AABB
			//-----------------------------------------------
			inline const Vector3Old& GetMax() const {return mvMax;};
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
			bool Contains(const Vector3Old &invPoint) const;
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
			Vector3Old mvOrigin;
			///Dimensions (width, height, breadth)
			Vector3Old mvSize;
			Vector3Old mvHalfSize;
			Vector3Old mvMin;
			Vector3Old mvMax;
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
			OOBB(const Vector3Old &invOrigin, const Vector3Old &invSize);
			
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
			void SetOrigin(const Vector3Old &invPos);
			//----------------------------------------------- 
			/// Get Origin
			///
			/// @return Local origin of the object
			//-----------------------------------------------
			const Vector3Old& GetOrigin() const;
			//----------------------------------------------- 
			/// Set Size
			///
			/// @param Local dimensions of the object
			//-----------------------------------------------
			void SetSize(const Vector3Old& invSize);
			//----------------------------------------------- 
			/// Get Size
			///
			/// @return Local dimensions of the object
			//-----------------------------------------------
			const Vector3Old& GetSize() const;			
			//----------------------------------------------- 
			/// Get Transform
			///
			/// Get the model matrix that transforms this
			/// object to world space
			/// @return To world matrix
			//-----------------------------------------------
			const Matrix4x4Old & GetTransform() const;
			//----------------------------------------------- 
			/// Set Transform
			///
			/// Set the matrix that transforms this object
			/// to world space
			/// @param To world matrix
			//-----------------------------------------------
			void SetTransform(const Matrix4x4Old& inmatTransform);
			
			//-----------------------------------------------
			/// Contains (Point)
			///
			/// @param Point in 3D space
			/// @return Whether point intersects the OOBB
			//-----------------------------------------------
			bool Contains(const Vector3Old &invPoint) const;
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
			
            Matrix4x4Old mmatLocal;
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
			Ray(const Vector3Old &invOrigin, const Vector3Old &invDirection, const f32 infLength = kDefaultRayLength);
			
			Vector3Old vOrigin;
			Vector3Old vDirection;
			
			//----------------------------------------------- 
			/// Get Point
			///
			/// Gets the position of a point t units 
			/// along the ray.
			/// @param A value t along parametric ray
			/// @return Point on ray
			//-----------------------------------------------
			Vector3Old GetPoint(f32 t) const;
			
			f32 DistanceFromPoint(const Vector3Old &invOrigin) const;
			
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
            Line(const Vector3Old &invStartPos, const Vector3Old &invEndPos)
            : vStartPos(invStartPos), vEndPos(invEndPos)
            {}
            Vector3Old vStartPos;
            Vector3Old vEndPos;
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
			Plane(const Vector3Old& incOrigin, const Vector3Old& incNormal);
			Plane(f32 a, f32 b, f32 c, f32 d);
			Plane(const Vector3Old& incNormal, f32 d);
			
			f32 DistanceFromPoint(const Vector3Old& invPoint) const;
			bool Intersects(const Ray& incRay) const;

			bool GetIsRayIntersecting(const Ray& incRay, Vector3Old& outcIntersect) const;
	
			Vector3Old mvNormal;
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
			void CalculateClippingPlanes(const Core::Matrix4x4Old& inmatViewProj);
			//----------------------------------------------------------
			/// Sphere Cull Test
			///
			/// Test if the bounding sphere lies within the frustum
			/// and determine whether it should be culled
			///
			/// @param Sphere
			/// @return Whether it lies within the bounds
			//-----------------------------------------------------------
			bool SphereCullTest(const Core::Sphere& inBoundingSphere) const;

		public:

			Core::Plane mLeftClipPlane;
			Core::Plane mRightClipPlane;

			Core::Plane mTopClipPlane;
			Core::Plane mBottomClipPlane;

			Core::Plane mNearClipPlane;
			Core::Plane mFarClipPlane;
		};
	}
}

#endif //_MOFLO_CORE_MATHS_SHAPES_H_
