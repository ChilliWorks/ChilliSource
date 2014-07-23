//
//  ShapeIntersection.cpp
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
                
                Vector3 vRayOrigin = inRay.vOrigin;
                Vector3 vRayDir = inRay.vDirection * inRay.fLength;
                
                Vector3 vAABBMin = inAABB.GetMin();
                Vector3 vAABBMax = inAABB.GetMax();
                
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
            bool Intersects(const AABB& inAABB, const Vector3& invPoint)
            {
                Vector3 vOrigin = inAABB.GetOrigin();
                Vector3 vHalfSize = inAABB.GetHalfSize();
                
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
                Vector3 vDelta = inRay.vOrigin - inSphere.vOrigin;
				f32 A = Vector3::DotProduct(inRay.vDirection, inRay.vDirection);
				f32 B = 2.0f * Vector3::DotProduct(vDelta, inRay.vDirection);
				f32 C = Vector3::DotProduct(vDelta, vDelta)-inSphere.fRadius * inSphere.fRadius;
                f32 D = B * B - 4.0f * A * C;
                return (D >= 0.0f); //returning true if equation has real roots.
            }
            //----------------------------------------------------------------
            /// Sphere vs Point
            //----------------------------------------------------------------
            bool Intersects(const Sphere& inSphere, const Vector3& invPoint)
            {
                return (inSphere.vOrigin - invPoint).LengthSquared() <= (inSphere.fRadius * inSphere.fRadius);
            }
            //----------------------------------------------------------------
            /// Sphere vs Plane
            //----------------------------------------------------------------
            ShapeIntersection::Result Intersects(const Sphere& inSphere, const Plane& inPlane)
            {
                f32 fDist = Vector3::DotProduct(inSphere.vOrigin, inPlane.mvNormal) + inPlane.mfD;
                
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
                Vector2 vMinLS(inRectLHS.BottomLeft());
                Vector2 vMaxLS(inRectLHS.TopRight());
                
                Vector2 vMinRS(inRectRHS.BottomLeft());
                Vector2 vMaxRS(inRectRHS.TopRight());
                
                return	(vMaxLS.x > vMinRS.x && vMinLS.x < vMaxRS.x) &&
                        (vMaxLS.y > vMinRS.y && vMinLS.y < vMaxRS.y);
            }
            //----------------------------------------------------------------
            /// Rect vs Point
            //----------------------------------------------------------------
            bool Intersects(const Rectangle& inRect, const Vector2& invPoint)
            {
                Vector2 bottLeft = inRect.BottomLeft();
                Vector2 topRight = inRect.TopRight();
                
                return invPoint.x >= bottLeft.x && invPoint.y >= bottLeft.y && invPoint.x <= topRight.x && invPoint.y <= topRight.y;
            }
            //----------------------------------------------------------------
            /// Plane vs Plane
            //----------------------------------------------------------------
            bool Intersects(const Plane& inPlaneLHS, const Plane& inPlaneRHS, Ray& outIntersection)
            {
				outIntersection.vDirection= Vector3::CrossProduct(inPlaneLHS.mvNormal, inPlaneRHS.mvNormal);
                
				f32 fDenom = Vector3::DotProduct(outIntersection.vDirection, outIntersection.vDirection);
                
                if(fDenom < std::numeric_limits<f32>::epsilon())
                {
                    return false;
                }
                
                Vector3 v1(inPlaneLHS.mfD * inPlaneRHS.mvNormal);
                Vector3 v2(inPlaneRHS.mfD * inPlaneLHS.mvNormal);
                Vector3 vDelta = v1 - v2;
				outIntersection.vOrigin = Vector3::CrossProduct(vDelta, outIntersection.vDirection);
                outIntersection.vOrigin /= fDenom;
                outIntersection.vDirection.Normalise();
                
                return true;
            }
            //----------------------------------------------------------------
            /// Line vs Line
            //----------------------------------------------------------------
            bool Intersects(const Line& inLineLHS, const Line& inLineRHS, Vector3& outvIntersection)
            {
                //Due to floating point errors we will compute the smallest distance between the two lines
                //and if this is less than epsilon we will consider that an intersection. We will then pick one of the closest
                //points to return as the intersection point.
                
                //The following is taken from the Orange Physics Book - "Realtime Collision Detection" by Christer Ericson. p. 149
                
                Vector3 vAB = inLineLHS.vEndPos - inLineLHS.vStartPos;
                Vector3 vCD = inLineRHS.vEndPos - inLineRHS.vStartPos;
                Vector3 vAC = inLineLHS.vStartPos - inLineRHS.vStartPos;
                
                f32 fA = Vector3::DotProduct(vAB, vAB);
                f32 fE = Vector3::DotProduct(vCD, vCD);
                f32 fF = Vector3::DotProduct(vCD, vAC);
                f32 fS = 0.0f;
                f32 fT = 0.0f;
                
                Vector3 vIntersection1, vIntersection2;
                
                if(fabsf(fA) <= std::numeric_limits<f32>::epsilon() && fabsf(fE) <= std::numeric_limits<f32>::epsilon())
                {
                    vIntersection1 = inLineLHS.vStartPos;
                    vIntersection2 = inLineRHS.vStartPos;
                    
                    Vector3 vDir = vIntersection1 - vIntersection2;
                    return (fabsf(Vector3::DotProduct(vDir, vDir)) <= std::numeric_limits<f32>::epsilon());
                }
                
                if(fA <= std::numeric_limits<f32>::epsilon())
                {
                    fS = 0.0f;
                    fT = MathUtils::Clamp(fF/fE, 0.0f, 1.0f);
                }
                else
                {
                    f32 fC = Vector3::DotProduct(vAB, vAC);
                    if(fabsf(fE) <= std::numeric_limits<f32>::epsilon())
                    {
                        fT = 0.0f;
                        fS = MathUtils::Clamp(-fC/fA, 0.0f, 1.0f);
                    }
                    else
                    {
                        f32 fB = Vector3::DotProduct(vAB, vCD);
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
                
                Vector3 vDir = vIntersection1 - vIntersection2;
                return (fabsf(Vector3::DotProduct(vDir, vDir)) <= std::numeric_limits<f32>::epsilon());
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

