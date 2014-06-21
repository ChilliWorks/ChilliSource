//
//  VolumeComponent.h
//  Chilli Source
//  Created by Ian Copland on 09/01/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_VOLUME_COMPONENT_H_
#define _CHILLISOURCE_CORE_VOLUME_COMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Entity/Component.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>

namespace ChilliSource
{
    namespace Core
    {
        //====================================================
        /// Description:
        ///
        /// Any object that has a AABB, OOBB or Bounding
        /// Sphere.
        //====================================================
        class VolumeComponent : public Core::Component
        {
        public:
            CS_DECLARE_NAMEDTYPE(VolumeComponent);
            VolumeComponent() : mfQueryIntersectionValue(0.0f){};
            virtual ~VolumeComponent(){}

            //----------------------------------------------------
            /// Get Axis Aligned Bounding Box
            ///
            /// All render components have a box used for culling
            /// and coarse intersections. This is cached and
            /// recomputed when required.
            /// @return Axis aligned bounding box
            //----------------------------------------------------
            virtual const Core::AABB& GetAABB() = 0;
            //----------------------------------------------------
            /// Get Object Oriented Bounding Box
            ///
            /// All render objects have an OOBB for
            /// picking. This is cached and
            /// recomputed when required.
            /// @return OOBB
            //----------------------------------------------------
            virtual const Core::OOBB& GetOOBB() = 0;
            //----------------------------------------------------
            /// Get Bounding Sphere
            ///
            /// All render objects have an bounding sphere for
            /// culling. This is cached and
            /// recomputed when required.
            /// @return bounding sphere
            //----------------------------------------------------
            virtual const Core::Sphere& GetBoundingSphere() = 0;
            //----------------------------------------------------
            /// Is Visible
            ///
            /// @return Whether or not to render
            //----------------------------------------------------
            virtual bool IsVisible() const = 0;

        public:
            f32 mfQueryIntersectionValue;
        };
    }
}

#endif
