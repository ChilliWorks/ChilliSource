//
//  LightComponent.h
//  Chilli Source
//  Created by Scott Downie on 08/12/2010.
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

#ifndef _CHILLISOURCE_RENDERING_COMPONENTS_LIGHTCOMPONENT_H_
#define _CHILLISOURCE_RENDERING_COMPONENTS_LIGHTCOMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Entity/Component.h>
#include <ChilliSource/Core/Math/Matrix4.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class LightComponent : public Core::Component
		{
		public:
			CS_DECLARE_NAMEDTYPE(LightComponent);
            
            //----------------------------------------------------------
            /// Constructor
            //----------------------------------------------------------
			LightComponent();
            //----------------------------------------------------------
            /// Destructor
            //----------------------------------------------------------
			virtual ~LightComponent(){}
            //----------------------------------------------------------
			/// Set Colour
			///
			/// @param Colour of the light
			//----------------------------------------------------------
			void SetColour(const Core::Colour &inColour);
            //----------------------------------------------------------
			/// Get Colour
			///
			/// @return Colour of the light with intensity applied
			//----------------------------------------------------------
            Core::Colour GetColour() const;
            //----------------------------------------------------------
			/// Get World Position
			///
			/// @return Position in world space
			//----------------------------------------------------------
			const Core::Vector3& GetWorldPosition() const;
            //----------------------------------------------------------
			/// Set Intensity
			///
			/// @param Intensity scaler
			//----------------------------------------------------------
			void SetIntensity(f32 infIntensity);
            //----------------------------------------------------------
			/// Get Intensity
			///
			/// @return Intensity scaler
			//----------------------------------------------------------
			f32 GetIntensity() const;
            //----------------------------------------------------------
            /// Get Light Matrix
            ///
            /// @return Matrix to transform into light space
            //----------------------------------------------------------
            virtual const Core::Matrix4& GetLightMatrix() const = 0;
            //----------------------------------------------------------
            /// Is Cache Valid
            ///
            /// @return Have any lighting values changed since the last
            /// render
            //----------------------------------------------------------
            bool IsCacheValid() const;
            //----------------------------------------------------------
            /// Calculate Lighting Values
            ///
            /// This calculates any lighting values that are based upon
            /// other lighting values. This is called whenever the light
            /// cache is invalidated.
            //----------------------------------------------------------
            virtual void CalculateLightingValues() {};
            //----------------------------------------------------------
            /// Set Cache Valid
            ///
            /// After rendering the cache is validated
            //----------------------------------------------------------
            void SetCacheValid();
            
        protected:
            
            mutable Core::Matrix4 mmatLight;
            
            Core::Colour mColour;
            
            f32 mfIntensity;
            
            bool mbCacheValid;
		};
    }
}

#endif