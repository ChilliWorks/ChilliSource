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
        namespace ShapeIntersection
        {
            //----------------------------------------------------------------
            /// AABB vs Ray
            //----------------------------------------------------------------
            bool Intersects(const AABB& inAABB, const Ray& inRay, f32 &outfT1, f32 &outfT2)
            {
                //Using the slab intersection method we check for intersection
                //against the 3 planes (slabs). If the ray fails itersection with any
                //slab then it does not intersect the box
                f32 t2 = std::numeric_limits<f32>::infinity();
                f32 t1 = -t2;
                
                Core::Vector3Old vRayOrigin = inRay.vOrigin;
                Core::Vector3Old vRayDir = inRay.vDirection * inRay.fLength;
                
                Core::Vector3Old vAABBMin = inAABB.GetMin();
                Core::Vector3Old vAABBMax = inAABB.GetMax();
                
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
            bool Intersects(const AABB& inAABB, const Vector3Old& invPoint)
            {
                Core::Vector3Old vOrigin = inAABB.GetOrigin();
                Core::Vector3Old vHalfSize = inAABB.GetHalfSize();
                
                return	(invPoint.x >= (vOrigin.x - vHalfSize.x)) && (invPoint.x <= (vOrigin.x + vHalfSize.x)) &&
                        (invPoint.y >= (vOrigin.y - vHalfSize.y)) && (invPoint.y <= (vOrigin.y + vHalfSize.y)) &&
                        (invPoint.z >= (vOrigin.z - vHalfSize.z)) && (invPoint.z <= (vOrigin.z + vHalfSize.z));
            }
            //----------------------------------------------------------------
            /// AABB vs AABB
            //----------------------------------------------------------------
            bool Intersects(const AABB& inAABBLHS, const AABB& inAABBRHS)
            {
                return	(inAABBLHS.GetMax().x > inAABBRHS.GetMin().x && inAABBLHS.GetMin().x < inAABBRHS.GetMax().x) &&
                        (inAABBLHS.GetMax().y > inAABBRHS.GetMin().y && inAABBLHS.GetMin().y < inAABBRHS.GetMax().y) &&
                        (inAABBLHS.GetMax().z > inAABBRHS.GetMin().z && inAABBLHS.GetMin().z < inAABBRHS.GetMax().z);
            }
            //----------------------------------------------------------------
            /// Sphere vs Ray
            //----------------------------------------------------------------
            bool Intersects(const Sphere& inSphere, const Ray& inRay)
            {
                Core::Vector3Old vDelta = inRay.vOrigin - inSphere.vOrigin;
                f32 A = inRay.vDirection.DotProduct(inRay.vDirection);
                f32 B = 2.0f * vDelta.DotProduct(inRay.vDirection);
                f32 C = vDelta.DotProduct(vDelta) - inSphere.fRadius * inSphere.fRadius;
                f32 D = B * B - 4.0f * A * C;
                return (D >= 0.0f); //returning true if equation has real roots.
            }
            //----------------------------------------------------------------
            /// Sphere vs Point
            //----------------------------------------------------------------
            bool Intersects(const Sphere& inSphere, const Vector3Old& invPoint)
            {
                return (inSphere.vOrigin - invPoint).LengthSquared() <= (inSphere.fRadius * inSphere.fRadius);
            }
            //----------------------------------------------------------------
            /// Sphere vs Plane
            //----------------------------------------------------------------
            ShapeIntersection::Result Intersects(const Sphere& inSphere, const Plane& inPlane)
            {
                f32 fDist = Vector3Old::DotProduct(&inSphere.vOrigin, &inPlane.mvNormal) + inPlane.mfD;
                
                if(fDist < -inSphere.fRadius)
                    return Result::k_outside;
                
                if(std::fabs(fDist) < inSphere.fRadius)
                    return Result::k_intersect;
                
                return Result::k_inside;
            }
            //----------------------------------------------------------------
            /// Sphere vs Sphere
            //----------------------------------------------------------------
            bool Intersects(const Sphere& inSphereLHS, const Sphere& inSphereRHS)
            {
                f32 fSigmaRadius = inSphereLHS.fRadius + inSphereRHS.fRadius;
                
                return	fabsf(inSphereLHS.vOrigin.x - inSphereRHS.vOrigin.x) < (fSigmaRadius) &&
                        fabsf(inSphereLHS.vOrigin.y - inSphereRHS.vOrigin.y) < (fSigmaRadius) &&
                        fabsf(inSphereLHS.vOrigin.z - inSphereRHS.vOrigin.z) < (fSigmaRadius);
            }
            //----------------------------------------------------------------
            /// Circle vs Circle
            //----------------------------------------------------------------
            bool Intersects(const Circle& inCircleLHS, const Circle& inCircleRHS)
            {
                f32 fSigmaRadius = inCircleLHS.fRadius + inCircleRHS.fRadius;
                
                return	fabsf(inCircleLHS.vOrigin.x - inCircleRHS.vOrigin.x) < (fSigmaRadius) &&
                        fabsf(inCircleLHS.vOrigin.y - inCircleRHS.vOrigin.y) < (fSigmaRadius);
            }
            //----------------------------------------------------------------
            /// Circle vs Point
            //----------------------------------------------------------------
			bool Intersects(const Circle& inCircle, const Vector2& invPoint)
            {
                return (inCircle.vOrigin - invPoint).LengthSquared() <= (inCircle.fRadius * inCircle.fRadius);
            }
            //----------------------------------------------------------------
            /// Rect vs Rect
            //----------------------------------------------------------------
            bool Intersects(const Rectangle& inRectLHS, const Rectangle& inRectRHS)
            {
                Core::Vector2 vMinLS(inRectLHS.vOrigin);
                Core::Vector2 vMaxLS(inRectLHS.vOrigin.x + inRectLHS.vSize.x, inRectLHS.vOrigin.y + inRectLHS.vSize.y);
                
                Core::Vector2 vMinRS(inRectRHS.vOrigin);
                Core::Vector2 vMaxRS(inRectRHS.vOrigin.x + inRectRHS.vSize.x, inRectRHS.vOrigin.y + inRectRHS.vSize.y);
                
                return	(vMaxLS.x > vMinRS.x && vMinLS.x < vMaxRS.x) &&
                        (vMaxLS.y > vMinRS.y && vMinLS.y < vMaxRS.y);
            }
            //----------------------------------------------------------------
            /// Rect vs Point
            //----------------------------------------------------------------
            bool Intersects(const Rectangle& inRect, const Vector3Old& invPoint)
            {
                return (invPoint.x >= inRect.vOrigin.x) && (invPoint.y >= inRect.vOrigin.y) && (inRect.vOrigin.x + inRect.vSize.x >= invPoint.x) && (inRect.vOrigin.y + inRect.vSize.y >= invPoint.y);
            }
            //----------------------------------------------------------------
            /// Plane vs Plane
            //----------------------------------------------------------------
            bool Intersects(const Plane& inPlaneLHS, const Plane& inPlaneRHS, Ray& outIntersection)
            {
                Vector3Old::CrossProduct(&inPlaneLHS.mvNormal, &inPlaneRHS.mvNormal, &outIntersection.vDirection);
                
                f32 fDenom = outIntersection.vDirection.DotProduct(outIntersection.vDirection);
                
                if(fDenom < MathUtils::kApproxZero)
                {
                    return false;
                }
                
                Vector3Old v1(inPlaneLHS.mfD * inPlaneRHS.mvNormal);
                Vector3Old v2(inPlaneRHS.mfD * inPlaneLHS.mvNormal);
                Vector3Old vDelta = v1 - v2;
                Vector3Old::CrossProduct(&vDelta, &outIntersection.vDirection, &outIntersection.vOrigin);
                outIntersection.vOrigin /= fDenom;
                outIntersection.vDirection.Normalise();
                
                return true;
            }
            //----------------------------------------------------------------
            /// Line vs Line
            //----------------------------------------------------------------
            bool Intersects(const Line& inLineLHS, const Line& inLineRHS, Vector3Old& outvIntersection)
            {
                //Due to floating point errors we will compute the smallest distance between the two lines
                //and if this is less than epsilon we will consider that an intersection. We will then pick one of the closest
                //points to return as the intersection point.
                
                //The following is taken from the Orange Physics Book - "Realtime Collision Detection" by Christer Ericson. p. 149
                
                Vector3Old vAB = inLineLHS.vEndPos - inLineLHS.vStartPos;
                Vector3Old vCD = inLineRHS.vEndPos - inLineRHS.vStartPos;
                Vector3Old vAC = inLineLHS.vStartPos - inLineRHS.vStartPos;
                
                f32 fA = Vector3Old::DotProduct(&vAB, &vAB);
                f32 fE = Vector3Old::DotProduct(&vCD, &vCD);
                f32 fF = Vector3Old::DotProduct(&vCD, &vAC);
                f32 fS = 0.0f;
                f32 fT = 0.0f;
                
                Vector3Old vIntersection1, vIntersection2;
                
                if(fabsf(fA) <= MathUtils::kApproxZero && fabsf(fE) <= MathUtils::kApproxZero)
                {
                    vIntersection1 = inLineLHS.vStartPos;
                    vIntersection2 = inLineRHS.vStartPos;
                    
                    Vector3Old vDir = vIntersection1 - vIntersection2;
                    return (fabsf(Vector3Old::DotProduct(&vDir, &vDir)) <= MathUtils::kApproxZero);
                }
                
                if(fA <= MathUtils::kApproxZero)
                {
                    fS = 0.0f;
                    fT = MathUtils::Clamp(fF/fE, 0.0f, 1.0f);
                }
                else
                {
                    f32 fC = Vector3Old::DotProduct(&vAB, &vAC);
                    if(fabsf(fE) <= MathUtils::kApproxZero)
                    {
                        fT = 0.0f;
                        fS = MathUtils::Clamp(-fC/fA, 0.0f, 1.0f);
                    }
                    else
                    {
                        f32 fB = Vector3Old::DotProduct(&vAB, &vCD);
                        f32 fDenom = fA*fE - fB*fB;
                        if(fDenom != 0.0f)
                        {
                            fS = MathUtils::Clamp((fB*fF - fC*fE)/fDenom, 0.0f, 1.0f);
                        }
                        else
                        {
                            fS = 0.0f;
                        }
                        
                        fT = (fB*fS + fF)/fE;
                        
                        if(fT < 0.0f)
                        {
                            fT = 0.0f;
                            fS = MathUtils::Clamp(-fC/fA, 0.0f, 1.0f);
                        }
                        else if(fT > 1.0f)
                        {
                            fT = 1.0f;
                            fS = MathUtils::Clamp((fB - fC)/fA, 0.0f, 1.0f);
                        }
                    }
                }
                
                vIntersection1 = inLineLHS.vStartPos + vAB * fS;
                vIntersection2 = inLineRHS.vStartPos + vCD * fT;
                
                //Pick the first one as the point of intersection
                outvIntersection = vIntersection1;
                
                Vector3Old vDir = vIntersection1 - vIntersection2;
                return (fabsf(Vector3Old::DotProduct(&vDir, &vDir)) <= MathUtils::kApproxZero);
            }
            //----------------------------------------------------------------
            /// Ray vs Slab
            //----------------------------------------------------------------
            bool RaySlabIntersect(f32 infStart, f32 infDir, f32 infMin, f32 infMax, f32& outfFirstT, f32& outfLastT)
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
}

