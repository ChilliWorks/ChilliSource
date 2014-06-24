//
//  SpriteComponent.cpp
//  Chilli Source
//  Created by Scott Downie on 29/09/2010.
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

#include <ChilliSource/Rendering/Sprite/SpriteComponent.h>

#include <ChilliSource/Core/Base/ByteColour.h>
#include <ChilliSource/Core/Base/ColourUtils.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Rendering/Base/AspectRatioUtils.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Sprite/DynamicSpriteBatcher.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

#include <algorithm>

namespace ChilliSource
{
	namespace Rendering
	{
        namespace
        {
            //----------------------------------------------------------
            /// Aspect ratio maintaing function that returns the original
            /// size. This is used despite the fact it doesn't do much to
            /// prevent multiple code paths when calculating size.
            ///
            /// @author S Downie
            ///
            /// @param Original size
            /// @param Preferred size
            ///
            /// @return Original size
            //----------------------------------------------------------
            Core::Vector2 UseOriginalSize(const Core::Vector2& in_originalSize, const Core::Vector2& in_preferredSize)
            {
                return in_originalSize;
            }
            //----------------------------------------------------------
            /// Aspect ratio maintaing function that returns the preferred
            /// size. This is used despite the fact it doesn't do much to
            /// prevent multiple code paths when calculating size.
            ///
            /// @author S Downie
            ///
            /// @param Original size
            /// @param Preferred size
            ///
            /// @return Preferred size
            //----------------------------------------------------------
            Core::Vector2 UsePreferredSize(const Core::Vector2& in_originalSize, const Core::Vector2& in_preferredSize)
            {
                return in_preferredSize;
            }
            
            const SpriteComponent::SizePolicyDelegate k_sizeDelegates[(u32)SpriteComponent::SizePolicy::k_totalNum] =
            {
                UseOriginalSize,
                UsePreferredSize,
                AspectRatioUtils::KeepOriginalWidthAdaptHeight,
                AspectRatioUtils::KeepOriginalHeightAdaptWidth,
                AspectRatioUtils::FitOriginal,
                AspectRatioUtils::FillOriginal
            };
        }
        
        CS_DEFINE_NAMEDTYPE(SpriteComponent);
		//----------------------------------------------------------
		//----------------------------------------------------------
		SpriteComponent::SpriteComponent()
        : m_originalUVs(0.0f, 0.0f, 1.0f, 1.0f), m_transformedUVs(m_originalUVs)
		{
            m_sizePolicyDelegate = k_sizeDelegates[(u32)SizePolicy::k_none];
		}
		//----------------------------------------------------------
		//----------------------------------------------------------
		bool SpriteComponent::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return  (in_interfaceId == SpriteComponent::InterfaceID) ||
                    (in_interfaceId == RenderComponent::InterfaceID) ||
                    (in_interfaceId == VolumeComponent::InterfaceID);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		const Core::AABB& SpriteComponent::GetAABB()
		{
			if(GetEntity() && !m_isAABBValid)
			{
                m_isAABBValid = true;
                
                // Realign the origin
                Core::Vector2 vHalfSize(m_transformedSize.x * 0.5f, m_transformedSize.y * 0.5f);
                Core::Vector2 vAlignedPos;
                Align(m_originAlignment, vHalfSize, vAlignedPos);
                
				// Rebuild the box
				mBoundingBox.SetSize(Core::Vector3(m_transformedSize, 0.0f));
				mBoundingBox.SetOrigin(GetEntity()->GetTransform().GetWorldPosition() + Core::Vector3(vAlignedPos, 0.0f));
			}
			return mBoundingBox;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		const Core::OOBB& SpriteComponent::GetOOBB()
		{
			if(GetEntity() && !m_isOOBBValid)
			{
                m_isOOBBValid = true;
                
                // Realign the origin
                Core::Vector2 vHalfSize(m_transformedSize.x * 0.5f, m_transformedSize.y * 0.5f);
                Core::Vector2 vAlignedPos;
                Align(m_originAlignment, vHalfSize, vAlignedPos);
                
				// Rebuild the box
                mOBBoundingBox.SetOrigin(Core::Vector3(vAlignedPos, 0.0f));
				mOBBoundingBox.SetSize(Core::Vector3(m_transformedSize, 0.0f));
				mOBBoundingBox.SetTransform(GetEntity()->GetTransform().GetWorldTransform());
			}
			return mOBBoundingBox;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		const Core::Sphere& SpriteComponent::GetBoundingSphere()
		{
			if(GetEntity() && !m_isBSValid)
			{
                m_isBSValid = true;
                
                // Realign the origin
                Core::Vector2 vHalfSize(m_transformedSize.x * 0.5f, m_transformedSize.y * 0.5f);
                Core::Vector2 vAlignedPos;
                Align(m_originAlignment, vHalfSize, vAlignedPos);
                
				mBoundingSphere.vOrigin = GetEntity()->GetTransform().GetWorldPosition() + Core::Vector3(vAlignedPos, 0.0f);
				mBoundingSphere.fRadius = std::sqrt((m_transformedSize.x * m_transformedSize.x) + (m_transformedSize.y * m_transformedSize.y)) * 0.5f;
			}
			return mBoundingSphere;
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void SpriteComponent::SetSize(const Core::Vector2& in_size)
		{
            m_vertexPositionsValid = false;
			
			m_originalSize = in_size;
            
            //TODO: What happens if the texture isn't set or changes
            auto texture = mpMaterial->GetTexture();
            m_transformedSize = m_sizePolicyDelegate(m_originalSize, Core::Vector2(texture->GetWidth(), texture->GetHeight()));
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void SpriteComponent::SetSize(f32 in_width, f32 in_height)
		{
			SetSize(Core::Vector2(in_width, in_height));
		}
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void SpriteComponent::SetSizePolicy(SizePolicy in_sizePolicy)
        {
            m_vertexPositionsValid = false;
            m_sizePolicyDelegate = k_sizeDelegates[(u32)in_sizePolicy];
            
            //TODO: What happens if the texture isn't set or changes
            auto texture = mpMaterial->GetTexture();
            m_transformedSize = m_sizePolicyDelegate(m_originalSize, Core::Vector2(texture->GetWidth(), texture->GetHeight()));
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        const Core::Vector2& SpriteComponent::GetSize() const
        {
            return m_transformedSize;
        }
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void SpriteComponent::SetUVs(const Rendering::UVs& in_uvs)
		{
			m_originalUVs = in_uvs;
            m_transformedUVs = in_uvs;
            
            UpdateVertexUVs();
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void SpriteComponent::SetUVs(f32 in_u, f32 in_v, f32 in_s, f32 in_t)
		{
			m_originalUVs.m_u = in_u;
			m_originalUVs.m_s = in_s;
			m_originalUVs.m_v = in_v;
			m_originalUVs.m_t = in_t;
            
            m_transformedUVs = m_originalUVs;
            
            UpdateVertexUVs();
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void SpriteComponent::SetColour(const Core::Colour& in_colour)
		{
			m_colour = in_colour;
            
            Core::ByteColour byteCol = Core::ColourUtils::ColourToByteColour(m_colour);
            m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_topLeft].Col = byteCol;
            m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomLeft].Col = byteCol;
            m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_topRight].Col = byteCol;
            m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomRight].Col = byteCol;
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void SpriteComponent::SetColour(f32 in_r, f32 in_g, f32 in_b, f32 in_a)
		{
			SetColour(Core::Colour(in_r, in_g, in_b, in_a));
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		const Core::Colour& SpriteComponent::GetColour() const
		{
			return m_colour;
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void SpriteComponent::SetFlippedHorizontally(bool in_flip)
		{
            m_flippedHorizontally = in_flip;
            
            UpdateVertexUVs();
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		bool SpriteComponent::IsFlippedHorizontally() const
		{
			return m_flippedHorizontally;
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void SpriteComponent::SetFlippedVertically(bool in_flip)
		{
            m_flippedVertically = in_flip;
            
            UpdateVertexUVs();
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		bool SpriteComponent::IsFlippedVertically() const
		{
			return m_flippedVertically;
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void SpriteComponent::SetOriginAlignment(AlignmentAnchor in_alignment)
        {
			m_originAlignment = in_alignment;
			m_vertexPositionsValid = false;
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		AlignmentAnchor SpriteComponent::GetOriginAlignment() const
        {
			return m_originAlignment;
		}
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void SpriteComponent::Render(RenderSystem* inpRenderSystem, CameraComponent* inpCam, ShaderPass ineShaderPass)
        {
            if (ineShaderPass == ShaderPass::k_ambient)
            {
                if(m_vertexPositionsValid == false)
                {
                    //We have been transformed so we need to recalculate our vertices
                    UpdateVertexPositions();
                    m_vertexPositionsValid = true;
                }
                
                m_spriteData.pMaterial = mpMaterial;
                
                //Add us to the render systems dynamic batch
                //If we force a batch flush here then the previous sprites
                //will be rendered.
                inpRenderSystem->GetDynamicSpriteBatchPtr()->Render(m_spriteData);
            }
        }
        //------------------------------------------------------------
        //------------------------------------------------------------
        void SpriteComponent::OnTransformChanged()
        {
            m_vertexPositionsValid = false;
            m_isBSValid = false;
            m_isAABBValid = false;
            m_isOOBBValid = false;
        }
		//----------------------------------------------------
		//----------------------------------------------------
		void SpriteComponent::OnAddedToScene()
		{
			m_transformChangedConnection = GetEntity()->GetTransform().GetTransformChangedEvent().OpenConnection(Core::MakeDelegate(this, &SpriteComponent::OnTransformChanged));
            
            OnTransformChanged();
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void SpriteComponent::OnRemovedFromScene()
		{
            m_transformChangedConnection = nullptr;
		}
        //-----------------------------------------------------------
        //-----------------------------------------------------------
		void SpriteComponent::UpdateVertexPositions()
        {
            const Core::Matrix4& worldTransform = GetEntity()->GetTransform().GetWorldTransform();
            
			Core::Vector2 vHalfSize(m_transformedSize.x * 0.5f, m_transformedSize.y * 0.5f);
			Core::Vector2 vAlignedPos;
            Align(m_originAlignment, vHalfSize, vAlignedPos);
            
            Core::Vector4 vCentrePos(vAlignedPos.x, vAlignedPos.y, 0, 0);
            Core::Vector4 vTemp(-vHalfSize.x, vHalfSize.y, 0, 1.0f);
			vTemp += vCentrePos;
			m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_topLeft].vPos = vTemp * worldTransform;
            
            vTemp.x = vHalfSize.x;
            vTemp.y = vHalfSize.y;

			vTemp += vCentrePos;
			m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_topRight].vPos = vTemp * worldTransform;
            
            vTemp.x = -vHalfSize.x;
            vTemp.y = -vHalfSize.y;

			vTemp += vCentrePos;
			m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomLeft].vPos = vTemp * worldTransform;
            
            vTemp.x = vHalfSize.x;
            vTemp.y = -vHalfSize.y;

			vTemp += vCentrePos;
			m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomRight].vPos = vTemp * worldTransform;
		}
        //-----------------------------------------------------------
        //-----------------------------------------------------------
		void SpriteComponent::UpdateVertexUVs()
        {
            if(m_flippedHorizontally == true)
            {
                m_transformedUVs = UVs::FlipHorizontally(m_transformedUVs);
            }
            else if(m_flippedHorizontally == false)
            {
                m_transformedUVs.m_u = m_originalUVs.m_u;
                m_transformedUVs.m_s = m_originalUVs.m_s;
            }
            
            if(m_flippedVertically == true)
            {
                m_transformedUVs = UVs::FlipVertically(m_transformedUVs);
            }
            else if(m_flippedVertically == false)
            {
                m_transformedUVs.m_v = m_originalUVs.m_v;
                m_transformedUVs.m_t = m_originalUVs.m_t;
            }
            
            m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_topLeft].vTex.x = m_transformedUVs.m_u;
            m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_topLeft].vTex.y = m_transformedUVs.m_v;
            m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomLeft].vTex.x = m_transformedUVs.m_u;
            m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomLeft].vTex.y = m_transformedUVs.m_v + m_transformedUVs.m_t;
            m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_topRight].vTex.x = m_transformedUVs.m_u + m_transformedUVs.m_s;
            m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_topRight].vTex.y = m_transformedUVs.m_v;
            m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomRight].vTex.x = m_transformedUVs.m_u + m_transformedUVs.m_s;
            m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomRight].vTex.y = m_transformedUVs.m_v + m_transformedUVs.m_t;
        }
	}
}
