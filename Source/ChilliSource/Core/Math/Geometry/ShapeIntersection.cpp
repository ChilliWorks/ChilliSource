/*
 * File: ShapeIntersection.cpp
 * Date: 03/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Core/Math/Geometry/ShapeIntersection.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/Math/MathUtils.h>

namespace ChilliSource
{
	namespace Core
	{
		//----------------------------------------------------------------
		/// AABB vs Ray
		//----------------------------------------------------------------
		bool CIntersection::Intersects(const AABB& inAABB, const Ray& inRay, f32 &outfT1, f32 &outfT2)
		{
			//Using the slab intersection method we check for intersection
			//against the 3 planes (slabs). If the ray fails itersection with any
			//slab then it does not intersect the box
			f32 t2 = std::numeric_limits<f32>::infinity();
			f32 t1 = -t2;
			
			Core::CVector3 vRayOrigin = inRay.vOrigin;
			Core::CVector3 vRayDir = inRay.vDirection * inRay.fLength;
			
			Core::CVector3 vAABBMin = inAABB.GetMin();
			Core::CVector3 vAABBMax = inAABB.GetMax();
			
			//----X Slab 
			if(!RaySlabIntersect(vRayOrigin.x, vRayDir.x, vAABBMin.x, vAABBMax.x, t1, t2)) return false;
			
			//----Y Slab 
			if(!RaySlabIntersect(vRayOrigin.y, vRayDir.y, vAABBMin.y, vAABBMax.y, t1, t2)) return false;
			
			//----Z Slab 
			if(!RaySlabIntersect(vRayOrigin.z, vRayDir.z, vAABBMin.z, vAABBMax.z, t1, t2)) return false;
  
			//We haven't failed intersection against any slab therefore we must have hit
			//t1 and t2 will give us our entry and exit point on the parametric ray
			outfT1 = t1;
			outfT2 = t2;
			
			return true; 
		}
		//----------------------------------------------------------------
		/// AABB vs Point
		//----------------------------------------------------------------
		bool CIntersection::Intersects(const AABB& inAABB, const CVector3& invPoint)
		{
			Core::CVector3 vOrigin = inAABB.GetOrigin();
			Core::CVector3 vHalfSize = inAABB.GetHalfSize();
			
			return	(invPoint.x >= (vOrigin.x - vHalfSize.x)) && (invPoint.x <= (vOrigin.x + vHalfSize.x)) &&
					(invPoint.y >= (vOrigin.y - vHalfSize.y)) && (invPoint.y <= (vOrigin.y + vHalfSize.y)) &&
					(invPoint.z >= (vOrigin.z - vHalfSize.z)) && (invPoint.z <= (vOrigin.z + vHalfSize.z));
		}
		//----------------------------------------------------------------
		/// AABB vs AABB
		//----------------------------------------------------------------
		bool CIntersection::Intersects(const AABB& inAABBLHS, const AABB& inAABBRHS)
		{
			return	(inAABBLHS.GetMax().x > inAABBRHS.GetMin().x && inAABBLHS.GetMin().x < inAABBRHS.GetMax().x) &&
					(inAABBLHS.GetMax().y > inAABBRHS.GetMin().y && inAABBLHS.GetMin().y < inAABBRHS.GetMax().y) &&
					(inAABBLHS.GetMax().z > inAABBRHS.GetMin().z && inAABBLHS.GetMin().z < inAABBRHS.GetMax().z);
		}
		//----------------------------------------------------------------
		/// OOBB vs Ray
		//----------------------------------------------------------------
		bool CIntersection::Intersects(const OOBB& inOOBB, const Ray& inRay)
		{
//			f32 t1 = 0.0f, t2 = 1.0f;
//			
//			CVector3 vBoxOrigin = inOOBB.GetOrigin();
//			CVector3 vBoxExtents = inOOBB.GetSize();
//			
//			if(!RaySlabIntersect(inRay.vOrigin.x * H[0], inRay.vDirection.x * H[0], vBoxOrigin.x * H[0] - vBoxExtents.x, vBoxOrigin.x * H[0] + vBoxExtents.x, t1, t2)) return false;
//			if(!RaySlabIntersect(inRay.vOrigin.y * H[1], inRay.vDirection.y * H[1], vBoxOrigin.y * H[1] - vBoxExtents.y, vBoxOrigin.y * H[1] + vBoxExtents.y, t1, t2)) return false;
//			if(!RaySlabIntersect(inRay.vOrigin.z * H[2], inRay.vDirection.z * H[2], vBoxOrigin.z * H[2] - vBoxExtents.z, vBoxOrigin.z * H[2] + vBoxExtents.z, t1, t2)) return false;
//			
//			return true;
			
			return false;			
		}
		//----------------------------------------------------------------
		/// Sphere vs Ray
		//----------------------------------------------------------------
		bool CIntersection::Intersects(const Sphere& inSphere, const Ray& inRay)
		{
			Core::CVector3 vDelta = inRay.vOrigin - inSphere.vOrigin;
			f32 A = inRay.vDirection.DotProduct(inRay.vDirection);
			f32 B = 2.0f * vDelta.DotProduct(inRay.vDirection);
			f32 C = vDelta.DotProduct(vDelta) - inSphere.fRadius * inSphere.fRadius;
			f32 D = B * B - 4.0f * A * C;
			return D >= 0.0f ? (-B - sqrt(D))/(2.0f * A) : std::numeric_limits<f32>::infinity();
		}
		//----------------------------------------------------------------
		/// Sphere vs Point
		//----------------------------------------------------------------
		bool CIntersection::Intersects(const Sphere& inSphere, const CVector3& invPoint)
		{
			return (inSphere.vOrigin - CVector2(invPoint)).LengthSquared() <= (inSphere.fRadius * inSphere.fRadius);
		}
        //----------------------------------------------------------------
        /// Sphere vs Plane
        //----------------------------------------------------------------
        CIntersection::Result CIntersection::Intersects(const Sphere& inSphere, const Plane& inPlane)
        {
            f32 fDist = CVector3::DotProduct(&inSphere.vOrigin, &inPlane.mvNormal) + inPlane.mfD;
            
            if(fDist < -inSphere.fRadius)
                return Result::k_outside;
            
            if(std::fabs(fDist) < inSphere.fRadius)
                return Result::k_intersect;
            
            return Result::k_inside;
        }
		//----------------------------------------------------------------
		/// Sphere vs Sphere
		//----------------------------------------------------------------
		bool CIntersection::Intersects(const Sphere& inSphereLHS, const Sphere& inSphereRHS)
		{
			f32 fSigmaRadius = inSphereLHS.fRadius + inSphereRHS.fRadius;
			
			return	fabsf(inSphereLHS.vOrigin.x - inSphereRHS.vOrigin.x) < (fSigmaRadius) &&
					fabsf(inSphereLHS.vOrigin.y - inSphereRHS.vOrigin.y) < (fSigmaRadius) &&
					fabsf(inSphereLHS.vOrigin.z - inSphereRHS.vOrigin.z) < (fSigmaRadius);
		}
		//----------------------------------------------------------------
		/// Circle vs Circle
		//----------------------------------------------------------------
		bool CIntersection::Intersects(const Circle& inCircleLHS, const Circle& inCircleRHS)
		{
			f32 fSigmaRadius = inCircleLHS.fRadius + inCircleRHS.fRadius;
			
			return	fabsf(inCircleLHS.vOrigin.x - inCircleRHS.vOrigin.x) < (fSigmaRadius) &&
					fabsf(inCircleLHS.vOrigin.y - inCircleRHS.vOrigin.y) < (fSigmaRadius);
		}
		//----------------------------------------------------------------
		/// Circle vs Point
		//----------------------------------------------------------------
		bool CIntersection::Intersects(const Circle& inCircle, const CVector3& invPoint)
		{
			return (inCircle.vOrigin - (CVector2)invPoint).LengthSquared() <= (inCircle.fRadius * inCircle.fRadius);
		}
		//----------------------------------------------------------------
		/// Rect vs Rect
		//----------------------------------------------------------------
		bool CIntersection::Intersects(const CRect& inRectLHS, const CRect& inRectRHS)
		{
			Core::CVector2 vMinLS(inRectLHS.vOrigin);
			Core::CVector2 vMaxLS(inRectLHS.vOrigin.x + inRectLHS.vSize.x, inRectLHS.vOrigin.y + inRectLHS.vSize.y);
			
			Core::CVector2 vMinRS(inRectRHS.vOrigin);
			Core::CVector2 vMaxRS(inRectRHS.vOrigin.x + inRectRHS.vSize.x, inRectRHS.vOrigin.y + inRectRHS.vSize.y);
			
			return	(vMaxLS.x > vMinRS.x && vMinLS.x < vMaxRS.x) &&
					(vMaxLS.y > vMinRS.y && vMinLS.y < vMaxRS.y);
		}
		//----------------------------------------------------------------
		/// Rect vs Point
		//----------------------------------------------------------------
		bool CIntersection::Intersects(const CRect& inRect, const CVector3& invPoint)
		{
			return (invPoint.x >= inRect.vOrigin.x) && (invPoint.y >= inRect.vOrigin.y) && (inRect.vOrigin.x + inRect.vSize.x >= invPoint.x) && (inRect.vOrigin.y + inRect.vSize.y >= invPoint.y);
		}
        //----------------------------------------------------------------
        /// Plane vs Plane
        //----------------------------------------------------------------
        bool CIntersection::Intersects(const Plane& inPlaneLHS, const Plane& inPlaneRHS, Ray& outIntersection)
        {
            CVector3::CrossProduct(&inPlaneLHS.mvNormal, &inPlaneRHS.mvNormal, &outIntersection.vDirection);
            
            f32 fDenom = outIntersection.vDirection.DotProduct(outIntersection.vDirection);
            
            if(fDenom < kApproxZero)
            {
                return false;
            }
            
            CVector3 v1(inPlaneLHS.mfD * inPlaneRHS.mvNormal);
            CVector3 v2(inPlaneRHS.mfD * inPlaneLHS.mvNormal);
            CVector3 vDelta = v1 - v2;
            CVector3::CrossProduct(&vDelta, &outIntersection.vDirection, &outIntersection.vOrigin);
            outIntersection.vOrigin /= fDenom;
            outIntersection.vDirection.Normalise();
            
            return true;
        }
        //----------------------------------------------------------------
        /// Line vs Line
        //----------------------------------------------------------------
        bool CIntersection::Intersects(const Line& inLineLHS, const Line& inLineRHS, CVector3& outvIntersection)
        {
            //Due to floating point errors we will compute the smallest distance between the two lines
            //and if this is less than epsilon we will consider that an intersection. We will then pick one of the closest
            //points to return as the intersection point.
            
            //The following is taken from the Orange Physics Book - "Realtime Collision Detection" by Christer Ericson. p. 149
            
            CVector3 vAB = inLineLHS.vEndPos - inLineLHS.vStartPos;
            CVector3 vCD = inLineRHS.vEndPos - inLineRHS.vStartPos;
            CVector3 vAC = inLineLHS.vStartPos - inLineRHS.vStartPos;
            
            f32 fA = CVector3::DotProduct(&vAB, &vAB);
            f32 fE = CVector3::DotProduct(&vCD, &vCD);
            f32 fF = CVector3::DotProduct(&vCD, &vAC);
            f32 fS = 0.0f;
            f32 fT = 0.0f;
            
            CVector3 vIntersection1, vIntersection2;
            
            if(fabsf(fA) <= kApproxZero && fabsf(fE) <= kApproxZero)
            {
                vIntersection1 = inLineLHS.vStartPos;
                vIntersection2 = inLineRHS.vStartPos;
                
                CVector3 vDir = vIntersection1 - vIntersection2;
                return (fabsf(CVector3::DotProduct(&vDir, &vDir)) <= kApproxZero);
            }
            
            if(fA <= kApproxZero)
            {
                fS = 0.0f;
                fT = CMathUtils::Clamp(fF/fE, 0.0f, 1.0f);
            }
            else
            {
                f32 fC = CVector3::DotProduct(&vAB, &vAC);
                if(fabsf(fE) <= kApproxZero)
                {
                    fT = 0.0f;
                    fS = CMathUtils::Clamp(-fC/fA, 0.0f, 1.0f);
                }
                else
                {
                    f32 fB = CVector3::DotProduct(&vAB, &vCD);
                    f32 fDenom = fA*fE - fB*fB;
                    if(fDenom != 0.0f)
                    {
                        fS = CMathUtils::Clamp((fB*fF - fC*fE)/fDenom, 0.0f, 1.0f);
                    }
                    else
                    {
                        fS = 0.0f;
                    }
                    
                    fT = (fB*fS + fF)/fE;
                    
                    if(fT < 0.0f)
                    {
                        fT = 0.0f;
                        fS = CMathUtils::Clamp(-fC/fA, 0.0f, 1.0f);
                    }
                    else if(fT > 1.0f)
                    {
                        fT = 1.0f;
                        fS = CMathUtils::Clamp((fB - fC)/fA, 0.0f, 1.0f);
                    }
                }
            }
            
            vIntersection1 = inLineLHS.vStartPos + vAB * fS;
            vIntersection2 = inLineRHS.vStartPos + vCD * fT;
            
            //Pick the first one as the point of intersection
            outvIntersection = vIntersection1;
            
            CVector3 vDir = vIntersection1 - vIntersection2;
            return (fabsf(CVector3::DotProduct(&vDir, &vDir)) <= kApproxZero);
        }
		//----------------------------------------------------------------
		/// Ray vs Slab
		//----------------------------------------------------------------
		bool CIntersection::RaySlabIntersect(f32 infStart, f32 infDir, f32 infMin, f32 infMax, f32& outfFirstT, f32& outfLastT)
		{
			if(infDir == 0.0f)
			{
				//The ray direction is parallel to the slab therfore will only collide if it starts within
				//Check if the ray begins inside the slab
				return (infStart < infMax && infStart > infMin);
			}
			
			//Work out the near and far intersection planes of the slab
			f32 fTMin = (infMin - infStart) / infDir;
			f32 fTMax = (infMax - infStart) / infDir;
			
			//TMin should be intersection with the near plane
			if(fTMin > fTMax) 
			{
				std::swap(fTMin, fTMax);
			}
			
			//Calculate the entry and exit point
			outfFirstT = std::max(fTMin, outfFirstT);
			outfLastT = std::min(fTMax, outfLastT);
			
			if(outfFirstT > outfLastT || outfLastT < 0.0f)
			{
				//We have missed the slab
				return false;
			}

			return true;
		}
	}
}

