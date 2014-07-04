//
//  RenderComponent.cpp
//  Chilli Source
//  Created by Scott Downie on 23/11/2010.
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

#include <ChilliSource/Rendering/Base/RenderComponent.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Rendering/Material/Material.h>

namespace ChilliSource
{
	namespace Rendering
	{
        CS_DEFINE_NAMEDTYPE(RenderComponent);
		//----------------------------------------------------
		/// Constructor
		//----------------------------------------------------
		RenderComponent::RenderComponent() : mfSortValue(0), mbVisible(true), mbShouldCull(true), mbCastsShadows(true)
		{
		}
		//----------------------------------------------------
		/// Is Visible
		//----------------------------------------------------
		bool RenderComponent::IsVisible() const
		{
            return mbVisible;
		}
        //----------------------------------------------------
        /// Should Cull
        //----------------------------------------------------
        bool RenderComponent::IsCullingEnabled() const
        {
            return mbShouldCull;
        }
        //----------------------------------------------------
        /// Set Cull Enabled
        //----------------------------------------------------
        void RenderComponent::SetCullingEnabled(bool inbCull)
        {
            mbShouldCull = inbCull;
        }
		//----------------------------------------------------
		/// Set Visible
		//----------------------------------------------------
		void RenderComponent::SetVisible(bool inbVisible)
		{
			mbVisible = inbVisible;
		}
		//-----------------------------------------------------
		/// Get Transformation Matrix
		//-----------------------------------------------------
		const Core::Matrix4& RenderComponent::GetTransformationMatrix()
		{
			return GetEntity()->GetTransform().GetWorldTransform();
		}
		//-----------------------------------------------------------
		/// Set Material
		//-----------------------------------------------------------
		void RenderComponent::SetMaterial(const MaterialCSPtr &inpMaterial)
		{
			mpMaterial = inpMaterial;
		}
		//-----------------------------------------------------------
		/// Get Material
		//-----------------------------------------------------------
		const MaterialCSPtr& RenderComponent::GetMaterial() const
		{
			return mpMaterial;
		}
		//-----------------------------------------------------------
		/// Is Transparent
		//-----------------------------------------------------------
		bool RenderComponent::IsTransparent()
		{
			return mpMaterial->IsTransparencyEnabled();
		}
        //-----------------------------------------------------
        /// Set Shadow Casting Enabled
        //-----------------------------------------------------
        void RenderComponent::SetShadowCastingEnabled(bool inbEnabled)
        {
            mbCastsShadows = inbEnabled;
        }
        //-----------------------------------------------------
        /// Is Shadow Casting Enabled
        //-----------------------------------------------------
        bool RenderComponent::IsShadowCastingEnabled() const
        {
            return mbCastsShadows;
        }
	}
}
