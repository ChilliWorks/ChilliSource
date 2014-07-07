//
//  Curves.h
//  Chilli Source
//  Created by Stuart McGaw on 17/11/2010.
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

#ifndef _CHILLISOURCE_CORE_MATH_CURVES_H_
#define _CHILLISOURCE_CORE_MATH_CURVES_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/MathUtils.h>

#include <vector>

namespace ChilliSource
{
	namespace Core
	{
		template <typename T> class CatmullRomSpline {
		public:
			CatmullRomSpline()
			{
				mbLengthCacheValid = false;
				mfDeltaT = 0;
			}
			CatmullRomSpline(const std::vector<T> & inPoints)
			: mPoints(inPoints)
			{
				mbLengthCacheValid = false;
				mfDeltaT = 1.0f/mPoints.size();

			}
			
			void AddPoint(const T & inPoint) {
				mbLengthCacheValid = false;
				mPoints.push_back(inPoint);
				mfDeltaT = 1.0f/ (mPoints.size() - 1);
			}
			T GetInterpolatedPointAt(f32 infValue) const{
				// Find out in which interval we are on the spline
				int p = (int)(infValue / mfDeltaT);
				// Compute local control point indices
				#define BOUNDS(pp) { if (pp < 0) pp = 0; else if (pp >= (int)mPoints.Size()) pp = mPoints.Size() - 1; }
				int p0 = p - 1;     BOUNDS(p0);
				int p1 = p;         BOUNDS(p1);
				int p2 = p + 1;     BOUNDS(p2);
				int p3 = p + 2;     BOUNDS(p3);
				// Relative (local) time 
				float lt = (infValue - mfDeltaT*(f32)p) / mfDeltaT;
				// Interpolate
				return Equation(lt, mPoints[p0], mPoints[p1], mPoints[p2], mPoints[p3]);
			}
			T GetTangentAtPoint(f32 infValue)
			{
				return (GetInterpolatedPointAt(infValue + mfSmallTDelta) - GetInterpolatedPointAt(infValue - mfSmallTDelta)).NormalisedCopy();
			}
			
			//Returns the parametric 			
			f32 GetLength() const {
				if (!mbLengthCacheValid) {
					CalculateLength();
				}
				return mfCachedLength;
			}
			u32 GetNumPoints(){
				return mPoints.size();
			}
			T & GetPointAtIndex(u32 inIndex){
				mbLengthCacheValid = false;
				return mPoints[inIndex];				
			}
			const T & GetPointAtIndex(u32 inIndex) const {
				return mPoints[inIndex];
			}
			static T Equation(f32 t, const T& p1, const T& p2, const T& p3, const T& p4)
			{
				f32 t2 = t * t;
				f32 t3 = t2 * t;
				
				f32 b1 = .5f * (     -t3 + 2.0f*t2 - t   );
				f32 b2 = .5f * ( 3.0f*t3 - 5.0f*t2 + 2.0f);
				f32 b3 = .5f * (-3.0f*t3 + 4.0f*t2 + t   );
				f32 b4 = .5f * (      t3 -      t2       );
				
				return T(p1*b1 + p2*b2 + p3*b3 + p4*b4); 
			}
			
			f32 GetParametricValueForDistance(f32 infS)
			{
				
				GetLength();
				
				u32 nMaxIndex = 0;
				u32 NumCachedValues = mfCachedDists.size();
				
				while (nMaxIndex < NumCachedValues  &&  mfCachedDists[nMaxIndex] < infS) {
					nMaxIndex++;
				}
				
				u32 nMinIndex = 0;
				
				if (nMaxIndex != 0) {
					nMinIndex = nMaxIndex - 1;
				}
				
				f32 fInterpAmount = (infS - mfCachedDists[nMinIndex])/(mfCachedDists[nMaxIndex]-mfCachedDists[nMinIndex]);
				fInterpAmount = MathUtils::Clamp(fInterpAmount, 0.0f,1.0f);
				
				f32 fResult = mfCachedParamDists[nMinIndex] + fInterpAmount * (mfCachedParamDists[nMaxIndex] - mfCachedParamDists[nMinIndex]);
								
				return fResult;
			}
			f32 GetDistanceForParametricValue(f32 infT)
			{
				
				GetLength();
				
				u32 nMaxIndex = 0;
				u32 NumCachedValues = mfCachedParamDists.size();
				
				while (nMaxIndex < NumCachedValues  &&  mfCachedParamDists[nMaxIndex] < infT) {
					nMaxIndex++;
				}
				
				u32 nMinIndex = 0;
				
				if (nMaxIndex != 0) {
					nMinIndex = nMaxIndex - 1;
				}
				
				f32 fInterpAmount = (infT - mfCachedParamDists[nMinIndex])/(mfCachedParamDists[nMaxIndex]-mfCachedParamDists[nMinIndex]);
				fInterpAmount = MathUtils::Clamp(fInterpAmount, 0.0f,1.0f);
				
				f32 fResult = mfCachedDists[nMinIndex] + fInterpAmount * (mfCachedDists[nMaxIndex] - mfCachedDists[nMinIndex]);
								
				return fResult;
			}
		protected:
			void CalculateLength() const
			{
				mbLengthCacheValid = true;
				mfCachedLength = 0;
				
				const u32 nSamples = (mPoints.size() - 1) * wLengthSamplesPerSegment;
				const f32 fTStep = 1.0f / nSamples;
				
				mfCachedParamDists.reserve(nSamples + 1);
				mfCachedDists.reserve(nSamples + 1);

				mfCachedParamDists.clear();
				mfCachedDists.clear();
				
				mfCachedParamDists.push_back(0);
				mfCachedDists.push_back(0);
				
				T LastPoint = GetInterpolatedPointAt(0);
				
				for (f32 fSample = fTStep; fSample < 1.01f; fSample+=fTStep) {	
					T CurrentPoint = GetInterpolatedPointAt(fSample);
					mfCachedLength+=(LastPoint-CurrentPoint).Length();
					LastPoint = CurrentPoint;
					
					mfCachedParamDists.push_back(fSample);
					mfCachedDists.push_back(mfCachedLength);
				}
			}
		protected:
			static const u16 wLengthSamplesPerSegment;
			static const float mfSmallTDelta;

			
			mutable bool mbLengthCacheValid;
			mutable f32 mfCachedLength;
			mutable std::vector<f32> mfCachedParamDists; //Cached values 
			mutable std::vector<f32> mfCachedDists; //
			
			f32 mfDeltaT;
			std::vector<T> mPoints;
		};
        template<typename T> const u16 CatmullRomSpline<T>::wLengthSamplesPerSegment = 259;
		template<typename T> const float CatmullRomSpline<T>::mfSmallTDelta = 0.005f;
		
		template <typename T> class MultiLineSegment {
		public:
			MultiLineSegment()
			{
				mbLengthCacheValid = false;
			}
			MultiLineSegment(const std::vector<T> & inPoints)
			: mPoints(inPoints)
			{
				mbLengthCacheValid = false;
				
			}
			
			void AddPoint(const T & inPoint) {
				mbLengthCacheValid = false;
				mPoints.push_back(inPoint);
			}
			T GetInterpolatedPointAt(f32 infValue) const{
				
			}
			T GetTangentAtPoint(f32 infValue)
			{
				
			}
			
			//Returns the parametric 			
			f32 GetLength() const {
				if (!mbLengthCacheValid) {
					CalculateLength();
				}
				return mfCachedLength;
			}
			u32 GetNumPoints(){
				return mPoints.size();
			}
			T & GetPointAtIndex(u32 inIndex){
				mbLengthCacheValid = false;
				return mPoints[inIndex];				
			}
			const T & GetPointAtIndex(u32 inIndex) const {
				return mPoints[inIndex];
			}
			f32 GetParametricValueForDistance(f32 infS)
			{
				
				return GetLength();
				
			}
			f32 GetDistanceForParametricValue(f32 infT)
			{
				
				return GetLength();
				
			}
		protected:
			void CalculateLength() const
			{
				mbLengthCacheValid = true;
				mfCachedLength = 0;
				mfCachedParamDists.clear();
				mfCachedDists.clear();
				
				if (mPoints.size() < 2) {
					return;
				}
				
				for (u32 nLineSegment = 0; nLineSegment < (mPoints.size() - 1); nLineSegment++) {
					f32 fLineLength = (mPoints[nLineSegment] - mPoints[nLineSegment]).Length();
					mfCachedLength += fLineLength;
					mfCachedDists.push_back(fLineLength);
				}
				

				
			}
		protected:			
			
			mutable bool mbLengthCacheValid;
			mutable f32 mfCachedLength;
			mutable std::vector<f32> mfCachedParamDists; //Cached values 
			mutable std::vector<f32> mfCachedDists; //
			
			std::vector<T> mPoints;
		};
		
	}
}

#endif