//
//  Shapes.cpp
//  Chilli Source
//  Created by Scott Downie on 21/10/2010.
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

#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/Math/Geometry/ShapeIntersection.h>

#include <limits>
#include <algorithm>

namespace ChilliSource
{
	namespace Core 
	{
		//================================================
		//================================================
		Rectangle::Rectangle(const Vector2 &invOrigin, const Vector2 &invSize)
        : vOrigin(invOrigin), vSize(invSize)
		{
		}
        //-----------------------------------------------
        //-----------------------------------------------
        Rectangle::Rectangle(f32 in_left, f32 in_top, f32 in_right, f32 in_bottom)
        {
            CS_ASSERT(in_right >= in_left && in_top >= in_bottom, "Cannot create inverted rect");
            
            vSize.x = in_right - in_left;
            vSize.y = in_top - in_bottom;
            
            vOrigin.x = in_left + (vSize.x * 0.5f);
            vOrigin.y = in_bottom + (vSize.y * 0.5f);
        }
		//-----------------------------------------------
		/// Left
		///
		/// @return Left side of rectangle
		//-----------------------------------------------
		f32 Rectangle::Left() const
        {
			return vOrigin.x - (vSize.x * 0.5f);
		}
		//-----------------------------------------------
		/// Right
		///
		/// @return Right side of rectangle
		//-----------------------------------------------
		f32 Rectangle::Right() const
        {
			return vOrigin.x + (vSize.x * 0.5f);
		}
		//-----------------------------------------------
		/// Top
		///
		/// @return Top side of rectangle
		//-----------------------------------------------
		f32 Rectangle::Top() const
        {
			return vOrigin.y + (vSize.y * 0.5f);
		}
		//-----------------------------------------------
		/// Bottom
		///
		/// @return Bottom side of rectangle
		//-----------------------------------------------
		f32 Rectangle::Bottom() const
        {
			return vOrigin.y - (vSize.y * 0.5f);
		}
		//-----------------------------------------------
		/// Top Left
		///
		/// @return Top left co-ordinate
		//-----------------------------------------------
        Vector2 Rectangle::TopLeft() const
		{
			return Vector2(Left(), Top());
		}
		//-----------------------------------------------
		/// Top Right
		///
		/// @return Top right co-ordinate
		//-----------------------------------------------
        Vector2 Rectangle::TopRight() const
		{
			return Vector2(Right(), Top());
		}
		//-----------------------------------------------
		/// Bottom Left
		///
		/// @return Bottom left co-ordinate
		//-----------------------------------------------
        Vector2 Rectangle::BottomLeft() const
		{
            return Vector2(Left(), Bottom());
		}
		//-----------------------------------------------
		/// Bottom Right
		///
		/// @return Bottom right co-ordinate
		//-----------------------------------------------
        Vector2 Rectangle::BottomRight() const
		{
            return Vector2(Right(), Bottom());
		}
		//-----------------------------------------------
		/// Centre
		///
		/// @return Centre point
		//-----------------------------------------------
		Vector2 Rectangle::Centre() const
		{
			return vOrigin;
		}
		//-----------------------------------------------
		/// Contains (Point)
		///
		/// @param Point in 2D space
		/// @return Whether point intersects the rect
		//-----------------------------------------------
		bool Rectangle::Contains(const Vector2& invPoint) const
		{
			return ShapeIntersection::Intersects(*this, invPoint);
		}
		//-----------------------------------------------
		/// Contains (Rect)
		///
		/// @param Rect in 2D space
		/// @return Whether rect intersects the rect
		//-----------------------------------------------
		bool Rectangle::Contains(const Rectangle& inRect) const
		{
			return ShapeIntersection::Intersects(*this, inRect);
		}
		//================================================
		/// Circle
		///
		/// Container for a circle with it's origin 
		/// at centre
		//================================================
		Circle::Circle(const Vector2 &invOrigin, const f32 infRadius) : vOrigin(invOrigin), fRadius(infRadius)
		{
		}
		//-----------------------------------------------
		/// Contains (Point)
		///
		/// @param Point in 2D space
		/// @return Whether point intersects the circle
		//-----------------------------------------------
		bool Circle::Contains(const Vector2 & invPoint) const
		{
			return ShapeIntersection::Intersects(*this, invPoint);
		}
		//-----------------------------------------------
		/// Contains (Circle)
		///
		/// @param Circle in 2D space
		/// @return Whether circle intersects the circle
		//-----------------------------------------------
		bool Circle::Contains(const Circle & inCircle) const
		{
			return ShapeIntersection::Intersects(*this, inCircle);
		}
		//================================================
		/// Sphere
		///
		/// Container for a sphere with it's origin 
		/// at centre
		//================================================
		Sphere::Sphere(const Vector3 &invOrigin, const f32 infRadius) : vOrigin(invOrigin), fRadius(infRadius)
		{
		}
		//-----------------------------------------------
		/// Contains (Point)
		///
		/// @param Point in 3D space
		/// @return Whether point intersects the sphere
		//-----------------------------------------------
		bool Sphere::Contains(const Vector3 & invPoint) const
		{
			return ShapeIntersection::Intersects(*this, invPoint);
		}
		//-----------------------------------------------
		/// Contains (Sphere)
		///
		/// @param Sphere in 3D space
		/// @return Whether sphere intersects the sphere
		//-----------------------------------------------
		bool Sphere::Contains(const Sphere & inSphere) const
		{
			return ShapeIntersection::Intersects(*this, inSphere);
		}
		//-----------------------------------------------
		/// Contains (Ray)
		///
		/// @param Ray in 3D space
		/// @return Whether point intersects the sphere
		//-----------------------------------------------
		bool Sphere::Contains(const Ray &inRay) const
		{
			return ShapeIntersection::Intersects(*this, inRay);
		}
		//================================================
		/// AABB
		///
		/// Container for a axis-aligned bounding box 
		/// with it's origin at centre
		//================================================
		AABB::AABB(const Vector3 &invOrigin, const Vector3 &invSize) : mvOrigin(invOrigin), mvSize(invSize), mvHalfSize(invSize*0.5f)
		{
			CalculateMinandMax();
		}
		//-----------------------------------------------
		/// Calculate Min and Max
		///
		/// Refreshes the bounds
		//-----------------------------------------------
		void AABB::CalculateMinandMax()
		{
			mvMin.x = mvOrigin.x - mvHalfSize.x;
			mvMin.y = mvOrigin.y - mvHalfSize.y;
			mvMin.z = mvOrigin.z - mvHalfSize.z;
			
			mvMax.x = mvOrigin.x + mvHalfSize.x;
			mvMax.y = mvOrigin.y + mvHalfSize.y;
			mvMax.z = mvOrigin.z + mvHalfSize.z;
		}
		//-----------------------------------------------
		/// Set Origin
		///
		/// @param Centre point of AABB
		//-----------------------------------------------
		void AABB::SetOrigin(const Vector3 &invOrigin)
		{
			mvOrigin = invOrigin;
			
			CalculateMinandMax();
		}
		//-----------------------------------------------
		/// Set Size
		///
		/// @param Dimensions of AABB
		//-----------------------------------------------
		void AABB::SetSize(const Vector3 &invSize)
		{
			mvSize = invSize;
            mvSize.x = fabsf(mvSize.x);
            mvSize.y = fabsf(mvSize.y);
            mvSize.z = fabsf(mvSize.z);
            
			mvHalfSize = mvSize * 0.5f;
			
			CalculateMinandMax();
		}
		//-----------------------------------------------
		/// Front Top Left
		///
		/// @return Top left co-ordinate
		//-----------------------------------------------
		const Vector3 AABB::FrontTopLeft() const
		{
			return Vector3(mvMin.x, mvMax.y, mvMin.z);
		}
		//-----------------------------------------------
		/// Front Top Right
		///
		/// @return Top right co-ordinate
		//-----------------------------------------------
		const Vector3 AABB::FrontTopRight() const
		{
			return Vector3(mvMax.x, mvMax.y, mvMin.z);
		}
		//-----------------------------------------------
		/// Front Bottom Left
		///
		/// @return Lower left co-ordinate
		//-----------------------------------------------
		const Vector3 AABB::FrontBottomLeft() const
		{
			return Vector3(mvMin.x, mvMin.y, mvMin.z);
		}
		//-----------------------------------------------
		/// Front Bottom Right
		///
		/// @return Lower right co-ordinate
		//-----------------------------------------------
		const Vector3 AABB::FrontBottomRight() const
		{
			return Vector3(mvMax.x, mvMin.y, mvMin.z);
		}
		//-----------------------------------------------
		/// Back Top Left
		///
		/// @return Top left co-ordinate
		//-----------------------------------------------
		const Vector3 AABB::BackTopLeft() const
		{
			return Vector3(mvMin.x, mvMax.y, mvMax.z);
		}
		//-----------------------------------------------
		/// Back Top Right
		///
		/// @return Top right co-ordinate
		//-----------------------------------------------
		const Vector3 AABB::BackTopRight() const
		{
			return Vector3(mvMax.x, mvMax.y, mvMax.z);
		}
		//-----------------------------------------------
		/// Back Bottom Left
		///
		/// @return Lower left co-ordinate
		//-----------------------------------------------
		const Vector3 AABB::BackBottomLeft() const
		{
			return Vector3(mvMin.x, mvMin.y, mvMax.z);
		}
		//-----------------------------------------------
		/// Back Bottom Right
		///
		/// @return Lower right co-ordinate
		//-----------------------------------------------
		const Vector3 AABB::BackBottomRight() const
		{
			return Vector3(mvMax.x, mvMin.y, mvMax.z);
		}
		//-----------------------------------------------
		/// Centre
		///
		/// @return Centre point
		//-----------------------------------------------
		const Vector3& AABB::Centre() const
		{
			return mvOrigin;
		}
		//-----------------------------------------------
		/// Contains (Point)
		///
		/// @param Point in 3D space
		/// @return Whether point intersects the AABB
		//-----------------------------------------------
		bool AABB::Contains(const Vector3 &invPoint) const
		{
			return	ShapeIntersection::Intersects(*this, invPoint);
		}
		//-----------------------------------------------
		/// Contains (AABB)
		///
		/// @param AABB in 3D space
		/// @return Whether AABB intersects the AABB
		//-----------------------------------------------
		bool AABB::Contains(const AABB &inAABB) const
		{
			return	ShapeIntersection::Intersects(*this, inAABB);
		}
		//-----------------------------------------------
		/// Contains (Ray)
		///
		/// @param Ray in 3D space
		/// @param Near point of intersection
		/// @param Far point of intersection
		/// @return Whether ray intersects the AABB
		//-----------------------------------------------
		bool AABB::Contains(const Ray &inRay, f32 &outfT1, f32 &outfT2) const
		{
			return ShapeIntersection::Intersects(*this, inRay, outfT1, outfT2);
		}
        //-----------------------------------------------
        /// Union
        ///
        /// @param AABB to union with this
        /// @return Unions two AABBs together find the composite size.
        //-----------------------------------------------
        AABB AABB::Union(const AABB& inAABB)
        {
            AABB cUnion;
                        
            f32 fMinX = std::min(mvMin.x, inAABB.mvMin.x);
            f32 fMinY = std::min(mvMin.y, inAABB.mvMin.y);
            f32 fMinZ = std::min(mvMin.z, inAABB.mvMin.z);
            
            f32 fMaxX = std::max(mvMax.x, inAABB.mvMax.x);
            f32 fMaxY = std::max(mvMax.y, inAABB.mvMax.y);
            f32 fMaxZ = std::max(mvMax.z, inAABB.mvMax.z);
            
            cUnion.SetOrigin(Vector3((fMinX + fMaxX) * 0.5f,(fMinY + fMaxY) * 0.5f,(fMinZ + fMaxZ) * 0.5f));
            cUnion.SetSize(Vector3(fMaxX - fMinX,fMaxY - fMinY,fMaxZ - fMinZ));
            
            return cUnion;
        }
		//================================================
		/// OOBB
		///
		/// Container for an object oriented bounding box 
		/// with it's origin at centre
		//================================================
		OOBB::OOBB() :  mHitBox(Vector3(1,1,1), Vector3::k_zero), mmatLocal(Matrix4::k_identity)
		{
		}
		OOBB::OOBB(const Vector3 &invOrigin, const Vector3 &invSize) : mHitBox(invOrigin, invSize), mmatLocal(Matrix4::k_identity)
		{
		}
		//-----------------------------------------------
		/// Contains (Point)
		///
		/// @param Point in 3D space
		/// @return Whether point intersects the OOBB
		//-----------------------------------------------
		bool OOBB::Contains(const Vector3 &invPoint) const
		{
			//Convert the point into our local space
			//allowing us to do an AABB check
			Matrix4 matToLocal = Matrix4::Inverse(GetTransform());
			
			Vector3 LocalPoint = invPoint * matToLocal;
			
			return mHitBox.Contains(LocalPoint);
		}
		//-----------------------------------------------
		/// Contains (Ray)
		///
		/// @param Ray in world space
		/// @param Near point of intersection
		/// @param Far point of intersection
		/// @return Whether ray intersects the OOBB
		//------------------------------------------------
		bool OOBB::Contains(const Ray &inRay, f32 &outfT1, f32 &outfT2) const
		{
			//Matrix to transform from world space to this objects local space
			//In local space the OOBB is simply an AABB
			Matrix4 matToLocal = Matrix4::Inverse(GetTransform());

			//Convert the ray into our local space
			//allowing us to do an AABB check
			Ray LocalRay = inRay;
			LocalRay.vOrigin = LocalRay.vOrigin * matToLocal;
			LocalRay.vDirection = (LocalRay.vDirection * matToLocal) - matToLocal.GetTranslation();
			LocalRay.fLength *= LocalRay.vDirection.Length();
			LocalRay.vDirection.Normalise();
			
			return mHitBox.Contains(LocalRay, outfT1, outfT2);
		}
		//----------------------------------------------- 
		/// Set Origin
		///
		/// @param X
		/// @param Y
		/// @param Z
		//-----------------------------------------------
		void OOBB::SetOrigin(const f32 infX, const f32 infY, const f32 infZ)
		{
			mHitBox.SetOrigin(Vector3(infX,infY,infZ));
		}
		//----------------------------------------------- 
		/// Set Origin
		///
		/// @param Local origin of the object
		//-----------------------------------------------
		void OOBB::SetOrigin(const Vector3 &invPos)
		{
			mHitBox.SetOrigin(invPos);
		}
		//----------------------------------------------- 
		/// Get Origin
		///
		/// @return Local origin of the object
		//-----------------------------------------------
		const Vector3& OOBB::GetOrigin() const
		{
			return mHitBox.GetOrigin();
		}
		//----------------------------------------------- 
		/// Set Size
		///
		/// @param Local dimensions of the object
		//-----------------------------------------------
		void OOBB::SetSize(const Vector3& invSize)
		{
			mHitBox.SetSize(invSize);
		}
		//----------------------------------------------- 
		/// Get Size
		///
		/// @return Local dimensions of the object
		//-----------------------------------------------
		const Vector3& OOBB::GetSize() const
		{
			return mHitBox.GetSize();
		}
		//----------------------------------------------- 
		/// Set Transform
		///
		/// Set the matrix that transforms this object
		/// to world space
		/// @param To world matrix
		//-----------------------------------------------
		void OOBB::SetTransform(const Matrix4& inmatTransform)
		{
			mmatLocal = inmatTransform;
		}
		//----------------------------------------------- 
		/// Get Transform
		///
		/// Get the model matrix that transforms this
		/// object to world space
		/// @return To world matrix
		//-----------------------------------------------
		const Matrix4& OOBB::GetTransform() const
		{			
			return mmatLocal;
		}
		//===============================================
		/// Ray
		///
		/// Container for a ray
		//===============================================
		Ray::Ray(const Vector3 &invOrigin, const Vector3 &invDirection, const f32 infLength) : vOrigin(invOrigin), vDirection(invDirection), fLength(infLength)
		{
		}
		//----------------------------------------------- 
		/// Get Point
		///
		/// Gets the position of a point t units 
		/// along the ray.
		/// @param A value t along parametric ray
		/// @return Point on ray
		//-----------------------------------------------
		Vector3 Ray::GetPoint(f32 t) const 
		{ 
			return Vector3(vOrigin + (vDirection * t) * fLength);
		}
		f32 Ray::DistanceFromPoint(const Vector3 &invPoint) const
        {
			return Vector3::CrossProduct(vDirection, invPoint).Length();
		}

		//===============================================
		/// Plane
		///
		/// Container for a plane
		//===============================================
		Plane::Plane(const Vector3& invOrigin, const Vector3& invNormal)
		:mvNormal(invNormal)
		{
			mfD = -Vector3::DotProduct(invNormal, invOrigin);
		}
		Plane::Plane(f32 a, f32 b, f32 c, f32 d)
		: mvNormal(a,b,c), mfD(d)
		{

		}
		Plane::Plane(const Vector3& incNormal, f32 d)
		:mvNormal(incNormal), mfD(d)
		{
			
		}
		f32 Plane::DistanceFromPoint(const Vector3& invPoint) const{			
			return Vector3::DotProduct(mvNormal, invPoint) + mfD;
		}
		
		bool Plane::Intersects(const Ray& incRay) const{
			f32 denom = Vector3::DotProduct(mvNormal, incRay.vDirection);
			
			
			if (fabsf(denom) < std::numeric_limits<f32>::epsilon())
			{
				// Parallel
				return false;
			}
			else
			{
				f32 nom = Vector3::DotProduct(mvNormal, incRay.vOrigin) + mfD;
				f32 t = -(nom/denom);
				return t >= 0;
			}
		}
		
		//----------------------------------------------- 
		/// GetIsRayIntersectiong
		///
		/// Determines if specified ray is colliding with this plane 
		/// if only one collision point on this plane then 
		/// @param A value t along parametric ray
		/// @return Point on ray
		//-----------------------------------------------
		bool Plane::GetIsRayIntersecting(const Ray& incRay, Vector3& outcIntersect) const
		{
			
			f32 denom = Vector3::DotProduct(mvNormal, incRay.vDirection);
											 
											 
			if (fabsf(denom) < std::numeric_limits<f32>::epsilon())
			{
				// Parallel
				return false;
			}
			else
			{
				f32 nom = Vector3::DotProduct(mvNormal, incRay.vOrigin) + mfD;
				f32 t = -(nom/denom);
				
				if (t >= 0)
				{
					outcIntersect = incRay.vOrigin + incRay.vDirection * t;
					
					return true;
				}
				else
					return false;
			}
			
		}
		//===============================================
		/// Frustum
		///
		/// Container for a frustum
		//===============================================
		//----------------------------------------------------------
		/// Calculate Clipping Planes
		///
		/// Build the 6 planes of the frustum based on the camera
		/// view projection matrix
		///
		/// @param View projection matrix
		//----------------------------------------------------------
		void Frustum::CalculateClippingPlanes(const Matrix4& inmatViewProj)
		{
			f32 t = 0.0f;

			// Left clipping plane
			mLeftClipPlane.mvNormal.x	= inmatViewProj.m[3] + inmatViewProj.m[0];
			mLeftClipPlane.mvNormal.y	= inmatViewProj.m[7] + inmatViewProj.m[4];
			mLeftClipPlane.mvNormal.z	= inmatViewProj.m[11] + inmatViewProj.m[8];
			mLeftClipPlane.mfD			= inmatViewProj.m[15] + inmatViewProj.m[12];

			t = 1/(sqrtf(mLeftClipPlane.mvNormal.x * mLeftClipPlane.mvNormal.x + mLeftClipPlane.mvNormal.y * mLeftClipPlane.mvNormal.y + mLeftClipPlane.mvNormal.z * mLeftClipPlane.mvNormal.z));
			mLeftClipPlane.mvNormal.x	*= t;
			mLeftClipPlane.mvNormal.y	*= t;
			mLeftClipPlane.mvNormal.z	*= t;
			mLeftClipPlane.mfD			*= t;

			// Right clipping plane
			mRightClipPlane.mvNormal.x	= inmatViewProj.m[3] - inmatViewProj.m[0];
			mRightClipPlane.mvNormal.y	= inmatViewProj.m[7] - inmatViewProj.m[4];
			mRightClipPlane.mvNormal.z	= inmatViewProj.m[11] - inmatViewProj.m[8];
			mRightClipPlane.mfD			= inmatViewProj.m[15] - inmatViewProj.m[12];

			t = 1/(sqrtf(mRightClipPlane.mvNormal.x * mRightClipPlane.mvNormal.x + mRightClipPlane.mvNormal.y * mRightClipPlane.mvNormal.y + mRightClipPlane.mvNormal.z * mRightClipPlane.mvNormal.z));
			mRightClipPlane.mvNormal.x	*= t;
			mRightClipPlane.mvNormal.y	*= t;
			mRightClipPlane.mvNormal.z	*= t;
			mRightClipPlane.mfD			*= t;

			// Top clipping plane
			mTopClipPlane.mvNormal.x	= inmatViewProj.m[3] - inmatViewProj.m[1];
			mTopClipPlane.mvNormal.y	= inmatViewProj.m[7] - inmatViewProj.m[5];
			mTopClipPlane.mvNormal.z	= inmatViewProj.m[11] - inmatViewProj.m[9];
			mTopClipPlane.mfD			= inmatViewProj.m[15] - inmatViewProj.m[13];

			t = 1/(sqrtf(mTopClipPlane.mvNormal.x * mTopClipPlane.mvNormal.x + mTopClipPlane.mvNormal.y * mTopClipPlane.mvNormal.y + mTopClipPlane.mvNormal.z * mTopClipPlane.mvNormal.z));
			mTopClipPlane.mvNormal.x	*= t;
			mTopClipPlane.mvNormal.y	*= t;
			mTopClipPlane.mvNormal.z	*= t;
			mTopClipPlane.mfD			*= t;

			// Bottom clipping plane
			mBottomClipPlane.mvNormal.x	= inmatViewProj.m[3] + inmatViewProj.m[1];
			mBottomClipPlane.mvNormal.y	= inmatViewProj.m[7] + inmatViewProj.m[5];
			mBottomClipPlane.mvNormal.z	= inmatViewProj.m[11] + inmatViewProj.m[9];
			mBottomClipPlane.mfD		= inmatViewProj.m[15] + inmatViewProj.m[13];

			t = 1/(sqrtf(mBottomClipPlane.mvNormal.x * mBottomClipPlane.mvNormal.x + mBottomClipPlane.mvNormal.y * mBottomClipPlane.mvNormal.y + mBottomClipPlane.mvNormal.z * mBottomClipPlane.mvNormal.z));
			mBottomClipPlane.mvNormal.x	*= t;
			mBottomClipPlane.mvNormal.y	*= t;
			mBottomClipPlane.mvNormal.z	*= t;
			mBottomClipPlane.mfD		*= t;

			// Near clipping plane
			mFarClipPlane.mvNormal.x	= inmatViewProj.m[3] - inmatViewProj.m[2];
			mFarClipPlane.mvNormal.y	= inmatViewProj.m[7] - inmatViewProj.m[6];
			mFarClipPlane.mvNormal.z	= inmatViewProj.m[11] - inmatViewProj.m[10];
			mFarClipPlane.mfD			= inmatViewProj.m[15] - inmatViewProj.m[14];

			t = 1/(sqrtf(mFarClipPlane.mvNormal.x * mFarClipPlane.mvNormal.x + mFarClipPlane.mvNormal.y * mFarClipPlane.mvNormal.y + mFarClipPlane.mvNormal.z * mFarClipPlane.mvNormal.z));
			mFarClipPlane.mvNormal.x	*= t;
			mFarClipPlane.mvNormal.y	*= t;
			mFarClipPlane.mvNormal.z	*= t;
			mFarClipPlane.mfD			*= t;

			// Far clipping plane
			mNearClipPlane.mvNormal.x	= inmatViewProj.m[3] + inmatViewProj.m[2];
			mNearClipPlane.mvNormal.y	= inmatViewProj.m[7] + inmatViewProj.m[6];
			mNearClipPlane.mvNormal.z	= inmatViewProj.m[11] + inmatViewProj.m[10];
			mNearClipPlane.mfD			= inmatViewProj.m[15] + inmatViewProj.m[14];

			t = 1/(sqrtf(mNearClipPlane.mvNormal.x * mNearClipPlane.mvNormal.x + mNearClipPlane.mvNormal.y * mNearClipPlane.mvNormal.y + mNearClipPlane.mvNormal.z * mNearClipPlane.mvNormal.z));
			mNearClipPlane.mvNormal.x	*= t;
			mNearClipPlane.mvNormal.y	*= t;
			mNearClipPlane.mvNormal.z	*= t;
			mNearClipPlane.mfD			*= t;
		}
		//----------------------------------------------------------
		/// Sphere Cull Test
		///
		/// Test if the bounding sphere lies within the frustum
		/// and determine whether it should be culled
		///
		/// @param Sphere
		/// @return Whether it lies within the bounds
		//-----------------------------------------------------------
		bool Frustum::SphereCullTest(const Sphere& inBoundingSphere) const
		{
			if(ShapeIntersection::Intersects(inBoundingSphere, mLeftClipPlane) == ShapeIntersection::Result::k_outside)
                return false;

			if(ShapeIntersection::Intersects(inBoundingSphere, mRightClipPlane) == ShapeIntersection::Result::k_outside)
                return false;

			if(ShapeIntersection::Intersects(inBoundingSphere, mTopClipPlane) == ShapeIntersection::Result::k_outside)
                return false;

			if(ShapeIntersection::Intersects(inBoundingSphere, mBottomClipPlane) == ShapeIntersection::Result::k_outside)
                return false;
        
            if(ShapeIntersection::Intersects(inBoundingSphere, mNearClipPlane) == ShapeIntersection::Result::k_outside)
                return false;
    
			if(ShapeIntersection::Intersects(inBoundingSphere, mFarClipPlane) == ShapeIntersection::Result::k_outside)
                return false;

			return true;
		}
	}
}
