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

namespace moFlo
{
	namespace Rendering
	{
        DEFINE_NAMED_INTERFACE(IRenderComponent);
		//----------------------------------------------------
		/// Constructor
		//----------------------------------------------------
		IRenderComponent::IRenderComponent() : mfSortValue(0), mbVisible(true), mbShouldCull(true), mbCastsShadows(false)
		{
		}
		//----------------------------------------------------
		/// Is Visible
		//----------------------------------------------------
		bool IRenderComponent::IsVisible() const
		{
            return mpEntityOwner ? mbVisible && (mpEntityOwner->IsVisible() && mpEntityOwner->Transform().GetWorldOpacity() != 0) : mbVisible;
		}
        //----------------------------------------------------
        /// Should Cull
        //----------------------------------------------------
        bool IRenderComponent::IsCullingEnabled() const
        {
            return mbShouldCull;
        }
        //----------------------------------------------------
        /// Set Cull Enabled
        //----------------------------------------------------
        void IRenderComponent::SetCullingEnabled(bool inbCull)
        {
            mbShouldCull = inbCull;
        }
		//----------------------------------------------------
		/// Set Visible
		//----------------------------------------------------
		void IRenderComponent::SetVisible(bool inbVisible)
		{
			mbVisible = inbVisible;
		}
		//-----------------------------------------------------
		/// Get Transformation Matrix
		//-----------------------------------------------------
		const Core::CMatrix4x4& IRenderComponent::GetTransformationMatrix()
		{
			return mpEntityOwner->Transform().GetWorldTransform();
		}
		//-----------------------------------------------------------
		/// Set Material
		//-----------------------------------------------------------
		void IRenderComponent::SetMaterial(const MaterialPtr &inpMaterial)
		{
			mpMaterial = inpMaterial;
		}
		//-----------------------------------------------------------
		/// Get Material
		//-----------------------------------------------------------
		const MaterialPtr& IRenderComponent::GetMaterial() const
		{
			return mpMaterial;
		}
		//-----------------------------------------------------------
		/// Get Material
		//-----------------------------------------------------------
		MaterialPtr& IRenderComponent::GetMaterial()
		{
			return mpMaterial;
		}
		//-----------------------------------------------------------
		/// Is Transparent
		//-----------------------------------------------------------
		bool IRenderComponent::IsTransparent()
		{
			return mpMaterial->IsTransparent();
		}
        //-----------------------------------------------------
        /// Set Shadow Casting Enabled
        //-----------------------------------------------------
        void IRenderComponent::SetShadowCastingEnabled(bool inbEnabled)
        {
            mbCastsShadows = inbEnabled;
        }
        //-----------------------------------------------------
        /// Is Shadow Casting Enabled
        //-----------------------------------------------------
        bool IRenderComponent::IsShadowCastingEnabled() const
        {
            return mbCastsShadows;
        }
	}
}
