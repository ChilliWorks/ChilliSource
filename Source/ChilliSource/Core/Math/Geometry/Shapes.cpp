/*
 * File: Shapes.cpp
 * Date: 21/10/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */


#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/Math/Geometry/ShapeIntersection.h>
#include <limits>

namespace ChilliSource
{
	namespace Core 
	{
		//================================================
		/// UV-Rect
		///
		/// Container for a rectangle with it's origin 
		/// at top left and using shorts instead of floats
		//================================================
		UVRect::UVRect() : mwOriginX(0), mwOriginY(0), mwWidth(0), mwHeight(0)
		{
		}
		UVRect::UVRect(s16 wOx, s16 wOy, s16 wWidth, s16 wHeight) : mwOriginX(wOx), mwOriginY(wOy), mwWidth(wWidth), mwHeight(wHeight)
		{
		}
		//================================================
		/// Rect
		///
		/// Container for a rectangle with it's origin 
		/// at top left
		//================================================
		Rectangle::Rectangle(const Vector2 &invOrigin, const Vector2 &invSize) : vOrigin(invOrigin), vSize(invSize)
		{
		}
		//-----------------------------------------------
		/// Left
		///
		/// @return Left side of rectangle
		//-----------------------------------------------
		f32 Rectangle::Left() const{
			return vOrigin.x;
		}
		//-----------------------------------------------
		/// Right
		///
		/// @return Right side of rectangle
		//-----------------------------------------------
		f32 Rectangle::Right() const{
			return vOrigin.x + vSize.x;
		}
		//-----------------------------------------------
		/// Top
		///
		/// @return Top side of rectangle
		//-----------------------------------------------
		f32 Rectangle::Top() const{
			return vOrigin.y;
		}
		//-----------------------------------------------
		/// Bottom
		///
		/// @return Bottom side of rectangle
		//-----------------------------------------------
		f32 Rectangle::Bottom() const{
			return vOrigin.y + vSize.y;
		}
		
		//-----------------------------------------------
		/// Top Left
		///
		/// @return Top left co-ordinate
		//-----------------------------------------------
		const Core::Vector2& Rectangle::TopLeft() const
		{
			return vOrigin;
		}
		//-----------------------------------------------
		/// Top Right
		///
		/// @return Top right co-ordinate
		//-----------------------------------------------
		const Core::Vector2 Rectangle::TopRight() const
		{
			return Vector2(vOrigin.x + vSize.x, vOrigin.y); 
		}
		//-----------------------------------------------
		/// Bottom Left
		///
		/// @return Bottom left co-ordinate
		//-----------------------------------------------
		const Core::Vector2 Rectangle::BottomLeft() const
		{
			return Vector2(vOrigin.x, vOrigin.y + vSize.y);
		}
		//-----------------------------------------------
		/// Bottom Right
		///
		/// @return Bottom right co-ordinate
		//-----------------------------------------------
		const Core::Vector2 Rectangle::BottomRight() const
		{
			return Vector2(vOrigin.x + vSize.x, vOrigin.y + vSize.y);
		}
		//-----------------------------------------------
		/// Centre
		///
		/// @return Centre point
		//-----------------------------------------------
		const Core::Vector2 Rectangle::Centre() const
		{
			return vOrigin + (vSize * 0.5f);
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
		//-----------------------------------------------
		/// Normalised
		///
		/// @return A copy of this rect which will have no negative components in its size
		//-----------------------------------------------
		Rectangle Rectangle::Normalised() const 
        {
			if (vSize.x < 0.0f || vSize.y < 0.0f)
            {
				Vector2 vFixedOrigin = vOrigin;
				Vector2 vFixedSize = vSize;
				
				if (vSize.x < 0.0f )
                {
					vFixedOrigin.x += vSize.x;
					vFixedSize.x *= -1.0f;
				}
				
				if (vSize.y < 0.0f) 
                {
					vFixedOrigin.y += vSize.y;
					vFixedSize.y *= -1.0f;
				}
				return Rectangle(vFixedOrigin, vFixedSize);
			} 
            else 
            {
				return *this;
			}
		}
		//-----------------------------------------------
		/// Union
		///
        /// Determines the bounding box for this merged with inRect
        /// @param Rectangle to union with this
        /// @return Unions two rectangles together find the composite size.
		//-----------------------------------------------
		Rectangle Rectangle::Union(const Rectangle& inRect)
        {
            Rectangle cUnion;
            
            // Determine rectangle extremities
            Vector2 vTopLeft1 = TopLeft();
            Vector2 vTopLeft2 = inRect.TopLeft();
            Vector2 vBottomRight1 = BottomRight();
            Vector2 vBottomRight2 = inRect.BottomRight();
            
            // Adjust origin
            if (vTopLeft2.x < vTopLeft1.x)
                cUnion.vOrigin.x = vTopLeft2.x;
            else
                cUnion.vOrigin.x = vOrigin.x;
            
            if (vTopLeft2.y < vTopLeft1.y)
                cUnion.vOrigin.y = vTopLeft2.y;
            else
                cUnion.vOrigin.y = vOrigin.y;
            
            // Adjust size
            if (vBottomRight2.x > vBottomRight1.x)
                cUnion.vSize.x = vBottomRight2.x - vOrigin.x;
            else
                cUnion.vSize.x = vSize.x;
            
            if (vBottomRight2.y > vBottomRight1.y)
                cUnion.vSize.y = vBottomRight2.y - vOrigin.y;
            else
                cUnion.vSize.y = vSize.y;
            
            return cUnion;
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
		AABB::AABB(const Core::Vector3 &invOrigin, const Core::Vector3 &invSize) : mvOrigin(invOrigin), mvSize(invSize), mvHalfSize(invSize*0.5f)
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
		void AABB::SetOrigin(const Core::Vector3 &invOrigin)
		{
			mvOrigin = invOrigin;
			
			CalculateMinandMax();
		}
		//-----------------------------------------------
		/// Set Size
		///
		/// @param Dimensions of AABB
		//-----------------------------------------------
		void AABB::SetSize(const Core::Vector3 &invSize)
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
		const Core::Vector3 AABB::FrontTopLeft() const
		{
			return Core::Vector3(mvMin.x, mvMax.y, mvMin.z);
		}
		//-----------------------------------------------
		/// Front Top Right
		///
		/// @return Top right co-ordinate
		//-----------------------------------------------
		const Core::Vector3 AABB::FrontTopRight() const
		{
			return Core::Vector3(mvMax.x, mvMax.y, mvMin.z);
		}
		//-----------------------------------------------
		/// Front Bottom Left
		///
		/// @return Lower left co-ordinate
		//-----------------------------------------------
		const Core::Vector3 AABB::FrontBottomLeft() const
		{
			return Core::Vector3(mvMin.x, mvMin.y, mvMin.z);
		}
		//-----------------------------------------------
		/// Front Bottom Right
		///
		/// @return Lower right co-ordinate
		//-----------------------------------------------
		const Core::Vector3 AABB::FrontBottomRight() const
		{
			return Core::Vector3(mvMax.x, mvMin.y, mvMin.z);
		}
		//-----------------------------------------------
		/// Back Top Left
		///
		/// @return Top left co-ordinate
		//-----------------------------------------------
		const Core::Vector3 AABB::BackTopLeft() const
		{
			return Core::Vector3(mvMin.x, mvMax.y, mvMax.z);
		}
		//-----------------------------------------------
		/// Back Top Right
		///
		/// @return Top right co-ordinate
		//-----------------------------------------------
		const Core::Vector3 AABB::BackTopRight() const
		{
			return Core::Vector3(mvMax.x, mvMax.y, mvMax.z);
		}
		//-----------------------------------------------
		/// Back Bottom Left
		///
		/// @return Lower left co-ordinate
		//-----------------------------------------------
		const Core::Vector3 AABB::BackBottomLeft() const
		{
			return Core::Vector3(mvMin.x, mvMin.y, mvMax.z);
		}
		//-----------------------------------------------
		/// Back Bottom Right
		///
		/// @return Lower right co-ordinate
		//-----------------------------------------------
		const Core::Vector3 AABB::BackBottomRight() const
		{
			return Core::Vector3(mvMax.x, mvMin.y, mvMax.z);
		}
		//-----------------------------------------------
		/// Centre
		///
		/// @return Centre point
		//-----------------------------------------------
		const Core::Vector3& AABB::Centre() const
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
		OOBB::OOBB() :  mHitBox(Vector3(1,1,1), Vector3::ZERO), mmatLocal(Matrix4x4::IDENTITY)
		{
		}
		OOBB::OOBB(const Core::Vector3 &invOrigin, const Core::Vector3 &invSize) : mHitBox(invOrigin,invSize), mmatLocal(Matrix4x4::IDENTITY)
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
			Matrix4x4 matToLocal = GetTransform().Inverse();
			
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
			Matrix4x4 matToLocal = GetTransform().Inverse();

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
		void OOBB::SetTransform(const Matrix4x4& inmatTransform)
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
		const Matrix4x4& OOBB::GetTransform() const
		{			
			return mmatLocal;
		}
		//===============================================
		/// Ray
		///
		/// Container for a ray
		//===============================================
		Ray::Ray(const Core::Vector3 &invOrigin, const Core::Vector3 &invDirection, const f32 infLength) : vOrigin(invOrigin), vDirection(invDirection), fLength(infLength)
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
		Core::Vector3 Ray::GetPoint(f32 t) const 
		{ 
			return Core::Vector3(vOrigin + (vDirection * t) * fLength);
		}
		f32 Ray::DistanceFromPoint(const Core::Vector3 &invPoint) const
        {
			return vDirection.CrossProduct(invPoint).Length();
		}

		//===============================================
		/// Plane
		///
		/// Container for a plane
		//===============================================
		Plane::Plane(const Core::Vector3& invOrigin, const Core::Vector3& invNormal)
		:mvNormal(invNormal)
		{
			mfD = -invNormal.DotProduct(invOrigin);
		}
		Plane::Plane(f32 a, f32 b, f32 c, f32 d)
		: mvNormal(a,b,c), mfD(d)
		{

		}
		Plane::Plane(const Core::Vector3& incNormal, f32 d)
		:mvNormal(incNormal), mfD(d)
		{
			
		}
		f32 Plane::DistanceFromPoint(const Core::Vector3& invPoint) const{			
			return mvNormal.DotProduct(invPoint) + mfD;
		}
		
		bool Plane::Intersects(const Ray& incRay) const{
			f32 denom = mvNormal.DotProduct(incRay.vDirection);											 
			
			
			if (fabsf(denom) < std::numeric_limits<f32>::epsilon())
			{
				// Parallel
				return false;
			}
			else
			{
				f32 nom = mvNormal.DotProduct(incRay.vOrigin) + mfD;
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
		bool Plane::GetIsRayIntersecting(const Ray& incRay, Core::Vector3& outcIntersect) const
		{
			
			f32 denom = mvNormal.DotProduct(incRay.vDirection);											 
											 
											 
			if (fabsf(denom) < std::numeric_limits<f32>::epsilon())
			{
				// Parallel
				return false;
			}
			else
			{
				f32 nom = mvNormal.DotProduct(incRay.vOrigin) + mfD;
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
		void Frustum::CalculateClippingPlanes(const Core::Matrix4x4& inmatViewProj)
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
		bool Frustum::SphereCullTest(const Core::Sphere& inBoundingSphere) const
		{
            Core::Sphere BoundingSphere = inBoundingSphere;
            
            //This padding value is the sqrt(2). The value has been chosen to counter act the inaccuracy of the
            //bounding sphere approx calculation in the render components; which uses the max axis rather than
            //the diagonal
            BoundingSphere.fRadius *= 1.412f;
            
			if(Core::ShapeIntersection::Intersects(BoundingSphere, mLeftClipPlane) == Core::ShapeIntersection::Result::k_outside)
                return false;

			if(Core::ShapeIntersection::Intersects(BoundingSphere, mRightClipPlane) == Core::ShapeIntersection::Result::k_outside)
                return false;

			if(Core::ShapeIntersection::Intersects(BoundingSphere, mTopClipPlane) == Core::ShapeIntersection::Result::k_outside)
                return false;

			if(Core::ShapeIntersection::Intersects(BoundingSphere, mBottomClipPlane) == Core::ShapeIntersection::Result::k_outside)
                return false;
        
            if(Core::ShapeIntersection::Intersects(BoundingSphere, mNearClipPlane) == Core::ShapeIntersection::Result::k_outside)
                return false;
    
			if(Core::ShapeIntersection::Intersects(BoundingSphere, mFarClipPlane) == Core::ShapeIntersection::Result::k_outside)
                return false;

			return true;
		}
	}
}
