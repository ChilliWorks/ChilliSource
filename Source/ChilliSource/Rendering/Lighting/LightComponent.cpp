//
//  LightComponent.cpp
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
#include <ChilliSource/Rendering/Lighting/LightComponent.h>

#include <ChilliSource/Core/Entity/Entity.h>

#include <limits>

namespace ChilliSource
{
	namespace Rendering
	{
        CS_DEFINE_NAMEDTYPE(LightComponent);

		//---------------------------------------------
		/// Constructor
		//---------------------------------------------
		LightComponent::LightComponent()
        : mfIntensity(1.0f)
        , mbCacheValid(false)
		{
			
		}
        //----------------------------------------------------------
        /// Set Colour
        //----------------------------------------------------------
        void LightComponent::SetColour(const Core::Colour &inColour)
        {
            mColour = inColour;
            
            mbCacheValid = false;
        }
        //----------------------------------------------------------
        /// Get Colour
        //----------------------------------------------------------
        Core::Colour LightComponent::GetColour() const
        {
            return mColour * mfIntensity;
        }
        //----------------------------------------------------------
        /// Get World Position
        //----------------------------------------------------------
        const Core::Vector3& LightComponent::GetWorldPosition() const
        {
            if(GetEntity() != nullptr)
            {
                return GetEntity()->GetTransform().GetWorldPosition();
            }
            
            return Core::Vector3::k_zero;
        }
        //----------------------------------------------------------
        /// Set Intensity
        //----------------------------------------------------------
        void LightComponent::SetIntensity(f32 infIntensity)
        {
            mfIntensity = infIntensity;
            
            mbCacheValid = false;
        }
        //----------------------------------------------------------
        /// Get Intensity
        //----------------------------------------------------------
        f32 LightComponent::GetIntensity() const
        {
            return mfIntensity;
        }
        //----------------------------------------------------------
        /// Is Cache Valid
        //----------------------------------------------------------
        bool LightComponent::IsCacheValid() const
        {
            return mbCacheValid;
        }
        //----------------------------------------------------------
        /// Set Cache Valid
        //----------------------------------------------------------
        void LightComponent::SetCacheValid()
        {
            mbCacheValid = true;
        }
	}
}

