/*
 * File: RenderComponent.cpp
 * Date: 23/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Rendering/Base/RenderComponent.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Rendering/Material/Material.h>

namespace ChilliSource
{
	namespace Rendering
	{
        DEFINE_NAMED_INTERFACE(RenderComponent);
		//----------------------------------------------------
		/// Constructor
		//----------------------------------------------------
		RenderComponent::RenderComponent() : mfSortValue(0), mbVisible(true), mbShouldCull(true), mbCastsShadows(false)
		{
		}
		//----------------------------------------------------
		/// Is Visible
		//----------------------------------------------------
		bool RenderComponent::IsVisible() const
		{
            return mpEntityOwner ? mbVisible && (mpEntityOwner->IsVisible() && mpEntityOwner->Transform().GetWorldOpacity() != 0) : mbVisible;
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
		const Core::CMatrix4x4& RenderComponent::GetTransformationMatrix()
		{
			return mpEntityOwner->Transform().GetWorldTransform();
		}
		//-----------------------------------------------------------
		/// Set Material
		//-----------------------------------------------------------
		void RenderComponent::SetMaterial(const MaterialSPtr &inpMaterial)
		{
			mpMaterial = inpMaterial;
		}
		//-----------------------------------------------------------
		/// Get Material
		//-----------------------------------------------------------
		const MaterialSPtr& RenderComponent::GetMaterial() const
		{
			return mpMaterial;
		}
		//-----------------------------------------------------------
		/// Get Material
		//-----------------------------------------------------------
		MaterialSPtr& RenderComponent::GetMaterial()
		{
			return mpMaterial;
		}
		//-----------------------------------------------------------
		/// Is Transparent
		//-----------------------------------------------------------
		bool RenderComponent::IsTransparent()
		{
			return mpMaterial->IsTransparent();
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
