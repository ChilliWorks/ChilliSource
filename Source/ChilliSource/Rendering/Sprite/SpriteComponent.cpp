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
#include <ChilliSource/Rendering/Base/SizePolicy.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Sprite/DynamicSpriteBatcher.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>

#include <algorithm>

namespace ChilliSource
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
        Vector2 UseOriginalSize(const Vector2& in_originalSize, const Vector2& in_preferredSize)
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
        Vector2 UsePreferredSize(const Vector2& in_originalSize, const Vector2& in_preferredSize)
        {
            return in_preferredSize;
        }
        //----------------------------------------------------------------------------------------
        /// Aspect ratio maintaining function that keeps the original width but adapts
        /// the height to maintain the aspect ratio
        ///
        /// @author S Downie
        ///
        /// @param Original size
        /// @param Preferred size
        ///
        /// @return Size with aspect maintained
        //----------------------------------------------------------------------------------------
        Vector2 KeepOriginalWidthAdaptHeight(const Vector2& in_originalSize, const Vector2& in_preferredSize)
        {
            return AspectRatioUtils::KeepOriginalWidthAdaptHeight(in_originalSize, in_preferredSize.x/in_preferredSize.y);
        }
        //----------------------------------------------------------------------------------------
        /// Aspect ratio maintaining function that keeps the original height but adapts
        /// the width to maintain the aspect ratio
        ///
        /// @author S Downie
        ///
        /// @param Original size
        /// @param Preferred size
        ///
        /// @return Size with aspect maintained
        //----------------------------------------------------------------------------------------
        Vector2 KeepOriginalHeightAdaptWidth(const Vector2& in_originalSize, const Vector2& in_preferredSize)
        {
            return AspectRatioUtils::KeepOriginalHeightAdaptWidth(in_originalSize, in_preferredSize.x/in_preferredSize.y);
        }
        //----------------------------------------------------------------------------------------
        /// Aspect ratio maintaining function that maintains the given target aspect ratio
        /// while ensuring the size does not DROP BELOW the original size
        ///
        /// @author S Downie
        ///
        /// @param Original size
        /// @param Preferred size
        ///
        /// @return Size with aspect maintained
        //----------------------------------------------------------------------------------------
        Vector2 FillOriginal(const Vector2& in_originalSize, const Vector2& in_preferredSize)
        {
            return AspectRatioUtils::FillOriginal(in_originalSize, in_preferredSize.x/in_preferredSize.y);
        }
        //----------------------------------------------------------------------------------------
        /// Aspect ratio maintaining function that maintains the given target aspect ratio
        /// while ensuring the size does not EXCEED the original size
        ///
        /// @author S Downie
        ///
        /// @param Original size
        /// @param Preferred size
        ///
        /// @return Size with aspect maintained
        //----------------------------------------------------------------------------------------
        Vector2 FitOriginal(const Vector2& in_originalSize, const Vector2& in_preferredSize)
        {
            return AspectRatioUtils::FitOriginal(in_originalSize, in_preferredSize.x/in_preferredSize.y);
        }
        
        const SpriteComponent::SizePolicyDelegate k_sizeDelegates[(u32)SizePolicy::k_totalNum] =
        {
            UseOriginalSize,
            UsePreferredSize,
            KeepOriginalWidthAdaptHeight,
            KeepOriginalHeightAdaptWidth,
            FitOriginal,
            FillOriginal
        };
    }
    
    CS_DEFINE_NAMEDTYPE(SpriteComponent);
    //----------------------------------------------------------
    //----------------------------------------------------------
    SpriteComponent::SpriteComponent()
    : m_uvs(0.0f, 0.0f, 1.0f, 1.0f)
    {
        m_sizePolicyDelegate = k_sizeDelegates[(u32)SizePolicy::k_none];
        UpdateVertexUVs(m_uvs);
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    bool SpriteComponent::IsA(InterfaceIDType in_interfaceId) const
    {
        return  (in_interfaceId == SpriteComponent::InterfaceID) ||
                (in_interfaceId == RenderComponent::InterfaceID) ||
                (in_interfaceId == VolumeComponent::InterfaceID);
    }
    //----------------------------------------------------
    //----------------------------------------------------
    const AABB& SpriteComponent::GetAABB()
    {
        if(IsTextureSizeCacheValid() == false)
        {
            OnTransformChanged();
            SetTextureSizeCacheValid();
        }
        
        if(GetEntity() && m_isAABBValid == false)
        {
            m_isAABBValid = true;
            
            Vector2 transformedSize = GetSize();
            
            // Realign the origin
            Vector2 anchorPoint = GetAnchorPoint(m_originAlignment, transformedSize * 0.5f);
            
            // Rebuild the box
            mBoundingBox.SetSize(Vector3(transformedSize, 0.0f));
            mBoundingBox.SetOrigin(GetEntity()->GetTransform().GetWorldPosition() - Vector3(anchorPoint, 0.0f));
        }
        return mBoundingBox;
    }
    //----------------------------------------------------
    //----------------------------------------------------
    const OOBB& SpriteComponent::GetOOBB()
    {
        if(IsTextureSizeCacheValid() == false)
        {
            OnTransformChanged();
            SetTextureSizeCacheValid();
        }
        
        if(GetEntity() && m_isOOBBValid == false)
        {
            m_isOOBBValid = true;
            
            Vector2 transformedSize = GetSize();
            
            // Realign the origin
            Vector2 anchorPoint = GetAnchorPoint(m_originAlignment, transformedSize * 0.5f);
            
            // Rebuild the box
            mOBBoundingBox.SetOrigin(Vector3(-anchorPoint, 0.0f));
            mOBBoundingBox.SetSize(Vector3(transformedSize, 0.0f));
            mOBBoundingBox.SetTransform(GetEntity()->GetTransform().GetWorldTransform());
        }
        return mOBBoundingBox;
    }
    //----------------------------------------------------
    //----------------------------------------------------
    const Sphere& SpriteComponent::GetBoundingSphere()
    {
        if(IsTextureSizeCacheValid() == false)
        {
            OnTransformChanged();
            SetTextureSizeCacheValid();
        }
        
        if(GetEntity() && m_isBSValid == false)
        {
            m_isBSValid = true;
            
            Vector2 transformedSize = GetSize();
            
            // Realign the origin
            Vector2 anchorPoint = GetAnchorPoint(m_originAlignment, transformedSize * 0.5f);
            
            mBoundingSphere.vOrigin = GetEntity()->GetTransform().GetWorldPosition() - Vector3(anchorPoint, 0.0f);
            mBoundingSphere.fRadius = std::sqrt((transformedSize.x * transformedSize.x) + (transformedSize.y * transformedSize.y)) * 0.5f;
        }
        return mBoundingSphere;
    }
    //-----------------------------------------------------------
    /// The texture atlas has the priority, then the texture
    //-----------------------------------------------------------
    Vector2 SpriteComponent::GetPreferredSize() const
    {
        if(m_textureAtlas != nullptr && m_hashedTextureAtlasId > 0)
        {
            return m_textureAtlas->GetOriginalFrameSize(m_hashedTextureAtlasId);
        }
        else if(mpMaterial != nullptr && mpMaterial->GetTexture() != nullptr)
        {
            auto texture = mpMaterial->GetTexture().get();
            return Vector2((f32)texture->GetWidth(), (f32)texture->GetHeight());
        }
        
        return Vector2::k_one;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void SpriteComponent::SetSize(const Vector2& in_size)
    {
        OnTransformChanged();
        
        m_originalSize = in_size;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void SpriteComponent::SetSize(f32 in_width, f32 in_height)
    {
        SetSize(Vector2(in_width, in_height));
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void SpriteComponent::SetSizePolicy(SizePolicy in_sizePolicy)
    {
        OnTransformChanged();
        
        m_sizePolicyDelegate = k_sizeDelegates[(u32)in_sizePolicy];
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    Vector2 SpriteComponent::GetSize() const
    {
        Vector2 preferredSize = GetPreferredSize();
        return m_sizePolicyDelegate(m_originalSize, preferredSize);
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void SpriteComponent::SetTextureAtlas(const TextureAtlasCSPtr& in_atlas)
    {
        OnTransformChanged();
        
        m_textureAtlas = in_atlas;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void SpriteComponent::SetTextureAtlasId(const std::string& in_atlasId)
    {
        CS_ASSERT(m_textureAtlas != nullptr, "SpriteComponent: Cannot set texture atlas id until atlas has been set");
        
        OnTransformChanged();
        
        m_hashedTextureAtlasId = HashCRC32::GenerateHashCode(in_atlasId);
        m_uvs = m_textureAtlas->GetFrameUVs(m_hashedTextureAtlasId);
        
        UpdateVertexUVs(m_uvs);
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void SpriteComponent::SetUVs(const UVs& in_uvs)
    {
        m_uvs = in_uvs;
        UpdateVertexUVs(in_uvs);
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void SpriteComponent::SetUVs(f32 in_u, f32 in_v, f32 in_s, f32 in_t)
    {
        m_uvs = UVs(in_u, in_v, in_s, in_t);
        UpdateVertexUVs(m_uvs);
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void SpriteComponent::SetColour(const Colour& in_colour)
    {
        m_colour = in_colour;
        
        ByteColour byteCol = ColourUtils::ColourToByteColour(m_colour);
        m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_topLeft].Col = byteCol;
        m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomLeft].Col = byteCol;
        m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_topRight].Col = byteCol;
        m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomRight].Col = byteCol;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void SpriteComponent::SetColour(f32 in_r, f32 in_g, f32 in_b, f32 in_a)
    {
        SetColour(Colour(in_r, in_g, in_b, in_a));
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    const Colour& SpriteComponent::GetColour() const
    {
        return m_colour;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void SpriteComponent::SetFlippedHorizontally(bool in_flip)
    {
        m_flippedHorizontally = in_flip;
        m_vertexPositionsValid = false;
        
        UpdateVertexUVs(m_uvs);
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
        m_vertexPositionsValid = false;
        
        UpdateVertexUVs(m_uvs);
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
            if(IsTextureSizeCacheValid() == false)
            {
                OnTransformChanged();
                SetTextureSizeCacheValid();
            }
            
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
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    bool SpriteComponent::IsTextureSizeCacheValid() const
    {
        if(m_textureAtlas == nullptr || m_hashedTextureAtlasId == 0)
        {
            if(mpMaterial != nullptr && mpMaterial->GetTexture() != nullptr)
            {
                auto texture = mpMaterial->GetTexture().get();
                Vector2 texSize((f32)texture->GetWidth(), (f32)texture->GetHeight());
                
                return texSize == m_cachedTextureSize;
            }
        }
        
        return true;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void SpriteComponent::SetTextureSizeCacheValid()
    {
        if(mpMaterial != nullptr && mpMaterial->GetTexture() != nullptr)
        {
            auto texture = mpMaterial->GetTexture().get();
            Vector2 texSize((f32)texture->GetWidth(), (f32)texture->GetHeight());
            m_cachedTextureSize = texSize;
        }
    }
    //----------------------------------------------------
    //----------------------------------------------------
    void SpriteComponent::OnAddedToScene()
    {
        m_transformChangedConnection = GetEntity()->GetTransform().GetTransformChangedEvent().OpenConnection(MakeDelegate(this, &SpriteComponent::OnTransformChanged));
        
        OnTransformChanged();
    }
    //----------------------------------------------------
    //----------------------------------------------------
    void SpriteComponent::OnRemovedFromScene()
    {
        m_transformChangedConnection = nullptr;
    }
    //-----------------------------------------------------------
    /// The image from the texture atlas will have potentially
    /// been cropped by the tool. This will affect the sprites
    /// position within the uncropped image and we need to
    /// account for that when positioning the corners
    //-----------------------------------------------------------
    void SpriteComponent::UpdateVertexPositions()
    {
        Vector2 frameCenter;
        Vector2 frameSize;
        if(m_textureAtlas != nullptr && m_hashedTextureAtlasId > 0)
        {
            CalcFrameCentreAndSize(frameCenter, frameSize);
        }
        else if(mpMaterial != nullptr && mpMaterial->GetTexture() != nullptr)
        {
            auto texture = mpMaterial->GetTexture().get();
            frameSize = m_sizePolicyDelegate(m_originalSize, Vector2((f32)texture->GetWidth(), (f32)texture->GetHeight()));
        }
        
        const Matrix4& worldTransform = GetEntity()->GetTransform().GetWorldTransform();
        Vector2 halfFrameSize(frameSize.x * 0.5f, frameSize.y * 0.5f);
        Vector2 alignedPosition = -GetAnchorPoint(m_originAlignment, halfFrameSize);
        Vector4 vertexCentre(alignedPosition.x + frameCenter.x, alignedPosition.y + frameCenter.y, 0.0f, 1.0f);
        
        //TL
        Vector4 vertexOffset(-halfFrameSize.x, halfFrameSize.y, 0.0f, 0.0f);
        m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_topLeft].vPos = (vertexCentre + vertexOffset) * worldTransform;
        
        //TR
        vertexOffset.x = halfFrameSize.x;
        vertexOffset.y = halfFrameSize.y;
        m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_topRight].vPos = (vertexCentre + vertexOffset) * worldTransform;
        
        //BL
        vertexOffset.x = -halfFrameSize.x;
        vertexOffset.y = -halfFrameSize.y;
        m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomLeft].vPos = (vertexCentre + vertexOffset) * worldTransform;
        
        //BR
        vertexOffset.x = halfFrameSize.x;
        vertexOffset.y = -halfFrameSize.y;
        m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomRight].vPos = (vertexCentre + vertexOffset) * worldTransform;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    void SpriteComponent::UpdateVertexUVs(const UVs& in_uvs)
    {
        UVs transformedUVs = in_uvs;
        
        if(m_flippedHorizontally == true && m_flippedVertically == true)
        {
            transformedUVs = UVs::FlipDiagonally(in_uvs);
        }
        else if(m_flippedHorizontally == true)
        {
            transformedUVs = UVs::FlipHorizontally(in_uvs);
        }
        else if(m_flippedVertically == true)
        {
            transformedUVs = UVs::FlipVertically(in_uvs);
        }
        
        m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_topLeft].vTex.x = transformedUVs.m_u;
        m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_topLeft].vTex.y = transformedUVs.m_v;
        m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomLeft].vTex.x = transformedUVs.m_u;
        m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomLeft].vTex.y = transformedUVs.m_v + transformedUVs.m_t;
        m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_topRight].vTex.x = transformedUVs.m_u + transformedUVs.m_s;
        m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_topRight].vTex.y = transformedUVs.m_v;
        m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomRight].vTex.x = transformedUVs.m_u + transformedUVs.m_s;
        m_spriteData.sVerts[(u32)SpriteBatch::Verts::k_bottomRight].vTex.y = transformedUVs.m_v + transformedUVs.m_t;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void SpriteComponent::CalcFrameCentreAndSize(Vector2& out_centre, Vector2& out_size) const
    {
        CS_ASSERT((m_textureAtlas != nullptr && m_hashedTextureAtlasId > 0), "Texture atlas and atlas Id must be valid.");
        
        const auto& frame = m_textureAtlas->GetFrame(m_hashedTextureAtlasId);
        Vector2 spriteSize = m_sizePolicyDelegate(m_originalSize, frame.m_originalSize);
        
        if (m_flippedHorizontally == true)
        {
            out_centre.x = (frame.m_originalSize.x * 0.5f) - frame.m_offset.x - (frame.m_croppedSize.x * 0.5f);
        }
        else
        {
            out_centre.x = (-frame.m_originalSize.x * 0.5f) +  frame.m_offset.x + (frame.m_croppedSize.x * 0.5f);
        }
        
        if (m_flippedVertically == true)
        {
            out_centre.y = (-frame.m_originalSize.y * 0.5f) + frame.m_offset.y + (frame.m_croppedSize.y * 0.5f);
        }
        else
        {
            out_centre.y = (frame.m_originalSize.y * 0.5f) - frame.m_offset.y - (frame.m_croppedSize.y * 0.5f);
        }
        
        Vector2 frameScale = spriteSize / frame.m_originalSize;
        out_centre = out_centre * frameScale;
        out_size = frame.m_croppedSize * frameScale;
    }
}
