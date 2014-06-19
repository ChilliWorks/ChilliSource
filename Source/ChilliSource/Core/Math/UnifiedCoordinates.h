//
//  UnifiedCoordinates.h
//  Chilli Source
//  Created by Scott Downie on 22/04/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifndef _CHILLISOURCE_GUI_UNIFIED_COORDINATES_H_
#define _CHILLISOURCE_GUI_UNIFIED_COORDINATES_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2.h>

//---------------------------------------------------------------
/// Unified Co-ordinates
///
/// Specify position, scaling etc in relative and absolute terms.
///
/// Relative co-ordinates are described in normalized units of 
/// the parent. i.e. the parent is bound by 0.0 - 1.0. Setting
/// a relative size of the view as (0.5f, 1.5f) would set the 
/// views dimensions to half the width of the parent and 1.5
/// times the height of the parent.
///
/// Absolute co-ordinates are described in dips and are applied
/// to the relative term. Using the above example setting the
/// absolute term to (10.0f, 10.0f) would add 10 units to the
/// width and height of the view once the relative term has 
/// been applied.
//--------------------------------------------------------------
namespace ChilliSource
{
    namespace Core
    {
        struct UnifiedScalar
        {
            UnifiedScalar() : fRelative(0.0f), fAbsolute(0.0f){}
            UnifiedScalar(f32 infRel, f32 infAbs) : fRelative(infRel), fAbsolute(infAbs){}

            f32 fRelative;
            f32 fAbsolute;
        };

        struct UnifiedVector2
        {
            UnifiedVector2(){}
            UnifiedVector2(const Core::Vector2& invRelative, const Core::Vector2& invAbsolute) : vRelative(invRelative), vAbsolute(invAbsolute){}
            UnifiedVector2(f32 infRelX, f32 infRelY, f32 infAbsX, f32 infAbsY) : vRelative(infRelX, infRelY), vAbsolute(infAbsX, infAbsY){}

            //------------------------------------------------------
            /// Get Absolute
            ///
            /// @return Absolute vector
            //------------------------------------------------------
            const Core::Vector2& GetAbsolute() const;
            //------------------------------------------------------
            /// Get Relative
            ///
            /// @return Relative vector
            //------------------------------------------------------
            const Core::Vector2& GetRelative() const;

            Core::Vector2 vRelative;
            Core::Vector2 vAbsolute;

            //---------------------------------
            // Operators
            //---------------------------------
            UnifiedVector2& operator=(const UnifiedVector2 &Vec)
            {this->vRelative = Vec.vRelative; this->vAbsolute = Vec.vAbsolute; return *this;}

            UnifiedVector2 operator+(const UnifiedVector2 &Vec) const
            {UnifiedVector2 Result; Result.vRelative = this->vRelative + Vec.vRelative; Result.vAbsolute = this->vAbsolute + Vec.vAbsolute; return Result;}
            
            UnifiedVector2 operator-(const UnifiedVector2 &Vec) const
            {UnifiedVector2 Result; Result.vRelative = this->vRelative - Vec.vRelative; Result.vAbsolute = this->vAbsolute - Vec.vAbsolute; return Result;}

            UnifiedVector2 operator*(const UnifiedVector2 &Vec) const
            {UnifiedVector2 Result; Result.vRelative = this->vRelative * Vec.vRelative; Result.vAbsolute = this->vAbsolute * Vec.vAbsolute; return Result;}
            
            UnifiedVector2 operator*(f32 infValue) const
            {UnifiedVector2 Result; Result.vRelative = this->vRelative * infValue; Result.vAbsolute = this->vAbsolute * infValue; return Result;}

            UnifiedVector2 operator/(const UnifiedVector2 &Vec) const
            {UnifiedVector2 Result; Result.vRelative = this->vRelative / Vec.vRelative; Result.vAbsolute = this->vAbsolute / Vec.vAbsolute; return Result;}
            
            UnifiedVector2 operator/(f32 infValue) const
            {UnifiedVector2 Result; Result.vRelative = this->vRelative / infValue; Result.vAbsolute = this->vAbsolute / infValue; return Result;}

            const UnifiedVector2& operator+=(const UnifiedVector2 &Vec)
            {this->vRelative += Vec.vRelative; this->vAbsolute += Vec.vAbsolute; return *this;}

            const UnifiedVector2& operator-=(const UnifiedVector2 &Vec)
            {this->vRelative -= Vec.vRelative; this->vAbsolute -= Vec.vAbsolute; return *this;}

            const UnifiedVector2& operator*=(const UnifiedVector2 &Vec)
            {this->vRelative *= Vec.vRelative; this->vAbsolute *= Vec.vAbsolute; return *this;}

            const UnifiedVector2& operator/=(const UnifiedVector2 &Vec)
            {this->vRelative /= Vec.vRelative; this->vAbsolute /= Vec.vAbsolute; return *this;}

            bool operator==(const UnifiedVector2 &Vec) const
            {return (Vec.vRelative == vRelative && Vec.vAbsolute == vAbsolute);}

            bool operator!=(const UnifiedVector2 &Vec) const
            {return (Vec.vRelative != vRelative || Vec.vAbsolute != vAbsolute);}
            
            static const UnifiedVector2 ZERO;
        };

        struct UnifiedRectangle
        {
            UnifiedRectangle(){}
            UnifiedRectangle(const UnifiedVector2& invTL, const UnifiedVector2& invTR, const UnifiedVector2& invBL, const UnifiedVector2& invBR)
            : vTopLeft(invTL), vTopRight(invTR), vBottomLeft(invBL), vBottomRight(invBR){}

            UnifiedVector2 vTopLeft;
            UnifiedVector2 vTopRight;
            UnifiedVector2 vBottomLeft;
            UnifiedVector2 vBottomRight;
        };
    }
}

#endif
