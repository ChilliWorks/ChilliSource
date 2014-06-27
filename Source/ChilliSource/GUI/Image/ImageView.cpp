//
//  ImageView.cpp
//  Chilli Source
//  Created by Scott Downie on 22/04/2011
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

#include <ChilliSource/GUI/Image/ImageView.h>

#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/String/StringParser.h>

namespace ChilliSource
{
    namespace GUI
    {
        DEFINE_META_CLASS(ImageView)
		
        //---Properties
        DEFINE_PROPERTY(Texture);
        DEFINE_PROPERTY(TextureAtlas);
        DEFINE_PROPERTY(TextureAtlasID);
        DEFINE_PROPERTY(SizeFromImage);
        DEFINE_PROPERTY(WidthFromImage);
        DEFINE_PROPERTY(HeightFromImage);
        DEFINE_PROPERTY(HeightMaintain);
        DEFINE_PROPERTY(WidthMaintain);
        DEFINE_PROPERTY(UVs);
        DEFINE_PROPERTY(UVOffsets);
        DEFINE_PROPERTY(FlipVertical);
        DEFINE_PROPERTY(FlipHorizontal);
		
        //--------------------------------------------------------
        /// Construtor
        ///
        /// Default
        //--------------------------------------------------------
        ImageView::ImageView() : UVs(0.0f, 0.0f, 1.0f, 1.0f), SizeFromImage(false),
        HeightMaintain(false), WidthMaintain(false), WidthFromImage(false), HeightFromImage(false), ActAsSpacer(false), FlipHorizontal(false), FlipVertical(false), mbFillMaintain(false), mbFitMaintain(false)
        {
            
        }
        //------------------------------------------------------
        /// Constructor
        ///
        /// Construct view from given key values
        ///
        /// @param Param dictionary
        //------------------------------------------------------
        ImageView::ImageView(const Core::ParamDictionary& insParams)
        : GUIView(insParams), UVs(0.0f, 0.0f, 1.0f, 1.0f),
        SizeFromImage(false), HeightMaintain(false), WidthMaintain(false), WidthFromImage(false), HeightFromImage(false), ActAsSpacer(false), FlipHorizontal(false), FlipVertical(false), mbFillMaintain(false), mbFitMaintain(false)
        {
            std::string strValue;
            
            //---Texture
            Core::StorageLocation eTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("TextureLocation", strValue))
            {
                eTextureLocation = Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("Texture", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                Texture = resourcePool->LoadResource<Rendering::Texture>(eTextureLocation, strValue);
            }
            //---Sprite sheet
            Core::StorageLocation eTextureAtlasLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("TextureAtlasLocation", strValue))
            {
                eTextureAtlasLocation = Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("TextureAtlas", strValue))
            {
                SetTextureAtlas(eTextureAtlasLocation, strValue);
            }
            //---Sprite Sheet Index ID
            if(insParams.TryGetValue("TextureAtlasID", strValue))
            {
                SetTextureAtlasID(strValue);
            }
            //---Size from image
            if(insParams.TryGetValue("SizeFromImage", strValue))
            {
                EnableSizeFromImage(Core::ParseBool(strValue));
            }
            //---Width from image
            if(insParams.TryGetValue("WidthFromImage", strValue))
            {
                EnableWidthFromImage(Core::ParseBool(strValue));
            }
            //---Height from image
            if(insParams.TryGetValue("HeightFromImage", strValue))
            {
                EnableHeightFromImage(Core::ParseBool(strValue));
            }
            //---Maintain Width
            if(insParams.TryGetValue("HeightMaintain", strValue))
            {
                HeightMaintain = Core::ParseBool(strValue);
            }
            //---Maintain Height
            if(insParams.TryGetValue("WidthMaintain", strValue))
            {
                WidthMaintain = Core::ParseBool(strValue);
            }
            //---Set Maintain Width
            if(insParams.TryGetValue("SetHeightMaintain", strValue))
            {
                Core::Vector2 vSize = Core::ParseVector2(strValue);
                SetHeightMaintainingAspect(vSize.x, vSize.y);
            }
            //---Set Maintain Height
            if(insParams.TryGetValue("SetWidthMaintain", strValue))
            {
                Core::Vector2 vSize = Core::ParseVector2(strValue);
                SetWidthMaintainingAspect(vSize.x, vSize.y);
            }
            //---Set Fill Maintain
            if(insParams.TryGetValue("SetFillMaintain", strValue))
            {
                Core::Vector4 vSize = Core::ParseVector4(strValue);
                SetFillMaintainingAspect(vSize.x, vSize.y, vSize.z, vSize.w);
            }
            //---Set Fit Maintain
            if(insParams.TryGetValue("SetFitMaintain", strValue))
            {
                Core::Vector4 vSize = Core::ParseVector4(strValue);
                SetFitMaintainingAspect(vSize.x, vSize.y, vSize.z, vSize.w);
            }
            //---UV
            if(insParams.TryGetValue("UVs", strValue))
            {
                Core::Vector4 vRaw = Core::ParseVector4(strValue);
                UVs.m_u = vRaw.x;
                UVs.m_v = vRaw.y;
                UVs.m_s = vRaw.z;
                UVs.m_t = vRaw.w;
            }
            //---Act as spacer
            if(insParams.TryGetValue("ActAsSpacer", strValue))
            {
                ActAsSpacer = true;
            }
            //---UV Flipped X
            if(insParams.TryGetValue("FlipHorizontal", strValue))
            {
                FlipHorizontal = true;
            }
            //---UV Flipped Y
            if(insParams.TryGetValue("FlipVertical", strValue))
            {
                FlipVertical = true;
            }
        }
        //--------------------------------------------------------
        /// Draw
        ///
        /// Override the base draw method to render ourselves
        ///
        /// @param Canvas renderer pointer
        //--------------------------------------------------------
        void ImageView::Draw(Rendering::CanvasRenderer* inpCanvas)
        {
            if(!IsOnscreen() || !Visible)
            {
                return;
            }
			
            UpdateSizeFromImage();
            
            if(ActAsSpacer == false)
            {
                Rendering::UVs sNewUVs = UVs;

                if(IsHorizontalFlipEnabled() && IsVerticalFlipEnabled())
				{
                    sNewUVs = Rendering::UVs::FlipDiagonally(UVs);
                }
				else if(IsHorizontalFlipEnabled())
				{
                    sNewUVs = Rendering::UVs::FlipHorizontally(UVs);
				}
				else if(IsVerticalFlipEnabled())
				{
                    sNewUVs = Rendering::UVs::FlipVertically(UVs);
				}
                
                Core::Vector2 offsetTL;
                Core::Vector2 size(GetAbsoluteSize());
                if(TextureAtlas != nullptr && m_hashedTextureAtlasId > 0)
                {
                    const auto& frame = TextureAtlas->GetFrame(m_hashedTextureAtlasId);

                    offsetTL.x = (-frame.m_originalSize.x * 0.5f) + (frame.m_croppedSize.x * 0.5f) + frame.m_offset.x;
                    offsetTL.y = (frame.m_originalSize.y * 0.5f) - (frame.m_croppedSize.y * 0.5f) - frame.m_offset.y;
                    
                    //Convert offset from texel space to local sprite space
                    offsetTL = size/frame.m_originalSize * offsetTL;
                    size = size/frame.m_originalSize * frame.m_croppedSize;
                }
                
                inpCanvas->DrawBox(GetTransform(), size, offsetTL, Texture, sNewUVs, GetAbsoluteColour(), Rendering::AlignmentAnchor::k_middleCentre);
            }
            
            GUIView::Draw(inpCanvas);
        }
        //--------------------------------------------------------
        /// Set Sprite Sheet
        ///
        /// @param Sprite sheet name
        //--------------------------------------------------------
        void ImageView::SetTextureAtlas(Core::StorageLocation ineLocation, const std::string& instrTextureAtlas)
        {
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            TextureAtlas = resourcePool->LoadResource<Rendering::TextureAtlas>(ineLocation, instrTextureAtlas);
        }
        //--------------------------------------------------------
        /// Set Sprite Sheet
        ///
        /// @param Sprite sheet
        //--------------------------------------------------------
        void ImageView::SetTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
        {
            TextureAtlas = inpTextureAtlas;
        }
        //--------------------------------------------------------
        /// Get Sprite Sheet
        ///
        /// @param Sprite sheet
        //--------------------------------------------------------
        const Rendering::TextureAtlasCSPtr& ImageView::GetTextureAtlas() const
        {
            return TextureAtlas;
        }
        //--------------------------------------------------------
        /// Set Sprite Sheet Index ID
        ///
        /// @param The ID of the index of the image within the sprite sheet
        //--------------------------------------------------------
        void ImageView::SetTextureAtlasID(const std::string& instrID)
        {
            CS_ASSERT(TextureAtlas, "Cannot set sprite sheet index without setting sprite sheet");
            TextureAtlasID = instrID;
            
            m_hashedTextureAtlasId = Core::HashCRC32::GenerateHashCode(TextureAtlasID);
            
            UVs = TextureAtlas->GetFrameUVs(m_hashedTextureAtlasId);
            m_imageSize = TextureAtlas->GetOriginalFrameSize(m_hashedTextureAtlasId);
            
            LayoutContent();
            LayoutChildrensContent();
        }
        //--------------------------------------------------------
        /// Get Sprite Sheet Index ID
        ///
        /// @return The ID of the index of the image within the sprite sheet
        //--------------------------------------------------------
        const std::string& ImageView::GetTextureAtlasID() const
        {
            return TextureAtlasID;
        }
        //--------------------------------------------------------
        /// Set Texture
        ///
        /// Set the texture that defines the image in the view
        ///
        /// @param Texture shared pointer
        //--------------------------------------------------------
        void ImageView::SetTexture(const Rendering::TextureCSPtr& inpTexture)
        {
            Texture = inpTexture;
            
            if(TextureAtlas == nullptr)
            {
                m_imageSize = Core::Vector2((f32)Texture->GetWidth(), (f32)Texture->GetHeight());
            }
            
            LayoutContent();
            LayoutChildrensContent();
        }
        //--------------------------------------------------------
        /// Get Texture
        ///
        /// Get the texture that defines the image in the view
        ///
        /// @param Texture shared pointer
        //--------------------------------------------------------
        const Rendering::TextureCSPtr& ImageView::GetTexture() const
        {
            return Texture;
        }
        //--------------------------------------------------------
        /// Set UVs
        ///
        /// Set the texture co-ordinates of the image. By default
        /// they will be mapped from 0 to 1
        ///
        /// @param Rect containing u, v, s, & t
        //--------------------------------------------------------
        void ImageView::SetUVs(const Rendering::UVs& insUVs)
        {
            UVs = insUVs;
        }
        //--------------------------------------------------------
        /// Get UVs
        ///
        /// @return Rect containing u, v, s, & t
        //--------------------------------------------------------
        const Rendering::UVs& ImageView::GetUVs() const
        {
            return UVs;
        }
        //--------------------------------------------------------
        /// Enable Size From Image
        ///
        /// When this is enabled the image view's size will be
        /// based on the size of the image
        ///
        /// @param Enable/disable
        //--------------------------------------------------------
        void ImageView::EnableSizeFromImage(bool inbEnable)
        {
            SizeFromImage = inbEnable;
            
            LayoutContent();
            LayoutChildrensContent();
        }
        //--------------------------------------------------------
        /// Is Size From Image Enabled
        ///
        /// When this is enabled the image view's size will be
        /// based on the size of the image
        ///
        /// @return Whether the image view's size will be
        /// based on the size of the image
        //--------------------------------------------------------
        bool ImageView::IsSizeFromImageEnabled() const
        {
            return SizeFromImage;
        }
        //--------------------------------------------------------
        /// Set Size From Image
        ///
        /// Sets the of the image view to the size of the image
        /// without requiring a draw to call. Useful for working
        /// with relative sizes and aspect ratios.
        //--------------------------------------------------------
        void ImageView::SetSizeFromImage()
        {
            SetSize(Core::UnifiedVector2(Core::Vector2::k_zero, GetSizeFromImage()));
        }
		
        //--------------------------------------------------------
        /// Get Size From Image
        ///
        /// @return Absolute size of the image
        //--------------------------------------------------------
        Core::Vector2 ImageView::GetSizeFromImage() const
        {
            return m_imageSize;
        }
        //--------------------------------------------------------
        /// Enable Width From Image
        ///
        /// When this is enabled the image view's width will be
        /// based on the width of the image
        ///
        /// @param Enable/disable
        //--------------------------------------------------------
        void ImageView::EnableWidthFromImage(bool inbEnable)
        {
            WidthFromImage = inbEnable;
            
            LayoutContent();
            LayoutChildrensContent();
        }
        //--------------------------------------------------------
        /// Is Width From Image Enabled
        ///
        /// When this is enabled the image view's width will be
        /// based on the width of the image
        ///
        /// @return Whether the image view's width will be
        /// based on the width of the image
        //--------------------------------------------------------
        bool ImageView::IsWidthFromImageEnabled() const
        {
            return WidthFromImage;
        }
        //--------------------------------------------------------
        /// Set Width From Image
        ///
        /// Sets the of the image view to the width of the image
        /// without requiring a draw to call. Useful for working
        /// with relative sizes and aspect ratios.
        //--------------------------------------------------------
        void ImageView::SetWidthFromImage()
        {
            SetSize(0.0f, GetSize().GetRelative().y, GetSizeFromImage().x, GetSize().GetAbsolute().y);
        }
        //--------------------------------------------------------
        /// Enable Height From Image
        ///
        /// When this is enabled the image view's height will be
        /// based on the height of the image
        ///
        /// @param Enable/disable
        //--------------------------------------------------------
        void ImageView::EnableHeightFromImage(bool inbEnable)
        {
            HeightFromImage = inbEnable;
            
            LayoutContent();
            LayoutChildrensContent();
        }
        //--------------------------------------------------------
        /// Is Height From Image Enabled
        ///
        /// When this is enabled the image view's height will be
        /// based on the height of the image
        ///
        /// @return Whether the image view's height will be
        /// based on the height of the image
        //--------------------------------------------------------
        bool ImageView::IsHeightFromImageEnabled() const
        {
            return HeightFromImage;
        }
        //--------------------------------------------------------
        /// Set Height From Image
        ///
        /// Sets the of the image view to the height of the image
        /// without requiring a draw to call. Useful for working
        /// with relative sizes and aspect ratios.
        //--------------------------------------------------------
        void ImageView::SetHeightFromImage()
        {
            SetSize(GetSize().GetRelative().x, 0.0f, GetSize().GetAbsolute().x, GetSizeFromImage().y);
        }
        //--------------------------------------------------------
        /// Set Width Maintaining Aspect
        ///
        /// Change the width of the image and resize the height
        /// to maintain the aspect ratio
        ///
        /// @param Unified width
        //--------------------------------------------------------
        void ImageView::SetWidthMaintainingAspect(f32 infRelWidth, f32 infAbsWidth)
        {
            WidthMaintain = true;
            
            SetSize(infRelWidth, 0.0f, infAbsWidth, 0.0f);
			
			f32 fScaleY = GetAbsoluteScale().y;
			if(fScaleY == 0.0f)
				return;
            
            Core::Vector2 vCurrentSize = GetSizeFromImage();
            f32 fAspectRatio = vCurrentSize.y / vCurrentSize.x;
            
            vCurrentSize = GetAbsoluteSize();
            f32 fAbsHeight = (fAspectRatio * vCurrentSize.x) / fScaleY;
            SetSize(infRelWidth, 0.0f, infAbsWidth, fAbsHeight);
        }
        //--------------------------------------------------------
        /// Set Height Maintaining Aspect
        ///
        /// Change the height of the image and resize the width
        /// to maintain the aspect ratio
        ///
        /// @param Unified height
        //--------------------------------------------------------
        void ImageView::SetHeightMaintainingAspect(f32 infRelHeight, f32 infAbsHeight)
        {
            HeightMaintain = true;
            
            SetSize(0.0f, infRelHeight, 0.0f, infAbsHeight);
			
			f32 fScaleX = GetAbsoluteScale().x;
			if(fScaleX == 0.0f)
				return;
            
            Core::Vector2 vCurrentSize = GetSizeFromImage();
            f32 fAspectRatio = vCurrentSize.x / vCurrentSize.y;
            
            vCurrentSize = GetAbsoluteSize();
            f32 fAbsWidth = (fAspectRatio * vCurrentSize.y) / fScaleX;
            SetSize(0.0f, infRelHeight, fAbsWidth, infAbsHeight);
        }
        //--------------------------------------------------------
        /// Set Fill Maintaining Aspect
        ///
        /// Based on the size of the image and the target size
        /// resize the image view to be the minimum of the target
        /// size while maintaining the aspect ratio
        ///
        /// @param Unified size
        //--------------------------------------------------------
        void ImageView::SetFillMaintainingAspect(f32 infRelWidth, f32 infRelHeight, f32 infAbsWidth, f32 infAbsHeight)
        {
            mbFillMaintain = true;
            mvFillMaintainTarget.vRelative.x = infRelWidth;
            mvFillMaintainTarget.vRelative.y = infRelHeight;
            mvFillMaintainTarget.vAbsolute.x = infAbsWidth;
            mvFillMaintainTarget.vAbsolute.y = infAbsHeight;
            
            if(GetParentViewPtr() == nullptr)
                return;
            
            Core::Vector2 vParentSize(GetParentViewPtr()->GetAbsoluteSize());
            Core::Vector2 vImageSize(GetSizeFromImage());
            
            f32 fParentRatio = (vParentSize.x * infRelWidth) / (vParentSize.y * infRelHeight);
            f32 fImageRatio = vImageSize.x / vImageSize.y;
            
            if(fImageRatio <= fParentRatio)
            {
                SetWidthMaintainingAspect(infRelWidth, infAbsWidth);
            }
            else
            {
                SetHeightMaintainingAspect(infRelHeight, infAbsHeight);
            }
        }
        //--------------------------------------------------------
        /// Set Fit Maintaining Aspect
        ///
        /// Based on the size of the image and the target size
        /// resize the image view to be the maximum of the target
        /// size while maintaining the aspect ratio
        ///
        /// @param Unified size
        //--------------------------------------------------------
        void ImageView::SetFitMaintainingAspect(f32 infRelWidth, f32 infRelHeight, f32 infAbsWidth, f32 infAbsHeight)
        {
            mbFitMaintain = true;
            mvFitMaintainTarget.vRelative.x = infRelWidth;
            mvFitMaintainTarget.vRelative.y = infRelHeight;
            mvFitMaintainTarget.vAbsolute.x = infAbsWidth;
            mvFitMaintainTarget.vAbsolute.y = infAbsHeight;
            
            if(GetParentViewPtr() == nullptr)
                return;
            
            Core::Vector2 vParentSize(GetParentViewPtr()->GetAbsoluteSize());
            Core::Vector2 vImageSize(GetSizeFromImage());
            
            f32 fParentRatio = (vParentSize.x * infRelWidth) / (vParentSize.y * infRelHeight);
            f32 fImageRatio = vImageSize.x / vImageSize.y;
            
            if(fImageRatio > fParentRatio)
            {
                SetWidthMaintainingAspect(infRelWidth, infAbsWidth);
            }
            else
            {
                SetHeightMaintainingAspect(infRelHeight, infAbsHeight);
            }
        }
        //--------------------------------------------------------
        /// Is Height Maintaining Aspect Enabled
        ///
        /// @return Whether auto scaling of the height to maintain the aspect ratio
        //--------------------------------------------------------
        bool ImageView::IsHeightMaintainingAspectEnabled() const
        {
            return HeightMaintain;
        }
        //--------------------------------------------------------
        /// Enable Height Maintaining Aspect
        ///
        /// Enables auto scaling of the height to maintain the aspect ratio
        ///
        /// @param boolean to disable or enable
        //--------------------------------------------------------
        void ImageView::EnableHeightMaintainingAspect(bool inbEnabled)
        {
            HeightMaintain = inbEnabled;
        }
        //--------------------------------------------------------
        /// Is Width Maintaining Aspect Enabled
        ///
        /// @return Whether auto scaling of the height to maintain the aspect ratio
        //--------------------------------------------------------
        bool ImageView::IsWidthMaintainingAspectEnabled() const
        {
            return WidthMaintain;
        }
        //--------------------------------------------------------
        /// Enable Width Maintaining Aspect
        ///
        /// Enables auto scaling of the height to maintain the aspect ratio
        ///
        /// @param boolean to disable or enable
        //--------------------------------------------------------
        void ImageView::EnableWidthMaintainingAspect(bool inbEnabled)
        {
            WidthMaintain = inbEnabled;	
        }
        //--------------------------------------------------------
        /// Update Size From Image
        ///
        /// Image view can be sized from assets etc. This function
        /// calculates the size based on these factors
        //--------------------------------------------------------
        void ImageView::UpdateSizeFromImage()
        {
            if(SizeFromImage)
            {
                SetSizeFromImage();
            }
            else if(WidthFromImage)
            {
                SetWidthFromImage();
            }
            else if(HeightFromImage)
            {
                SetHeightFromImage();
            }
            else 
            {
                UpdateSizeMaintainingAspect();
            }
        }
        //--------------------------------------------------------
        /// Update Size Maintaining Aspect
        ///
        /// Image view can be sized from assets etc. This function
        /// calculates the size based on these factors
        //--------------------------------------------------------
        void ImageView::UpdateSizeMaintainingAspect()
        {
            if(mbFillMaintain)
            {
                SetFillMaintainingAspect(mvFillMaintainTarget.GetRelative().x, mvFillMaintainTarget.GetRelative().y, mvFillMaintainTarget.GetAbsolute().x, mvFillMaintainTarget.GetAbsolute().y);
            }
            else if(mbFitMaintain)
            {
                SetFitMaintainingAspect(mvFitMaintainTarget.GetRelative().x, mvFitMaintainTarget.GetRelative().y, mvFitMaintainTarget.GetAbsolute().x, mvFitMaintainTarget.GetAbsolute().y);
            }
            else if(HeightMaintain)
            {
                Core::UnifiedVector2 uvSize = GetSize();
                SetHeightMaintainingAspect(uvSize.GetRelative().y, uvSize.GetAbsolute().y);
            }
            else if(WidthMaintain)
            {
                Core::UnifiedVector2 uvSize = GetSize();
                SetWidthMaintainingAspect(uvSize.GetRelative().x, uvSize.GetAbsolute().x);
            }
        }
        //--------------------------------------------------------
        /// Layout Content
        ///
        /// Called when the view is able to retrieve an absolute
        /// value.
        //--------------------------------------------------------
        void ImageView::LayoutContent()
        {
            UpdateSizeFromImage();
            GUIView::LayoutContent();
        }
        //-----------------------------------------------------------
		/// Set Flipped Horizontal
		///
		/// Flip the sprite about it's local x-axis
		///
		/// @param Whether to flip or not
		//-----------------------------------------------------------
		void ImageView::EnableHorizontalFlip(bool inbValue)
		{
			FlipHorizontal = inbValue;
		}
		//-----------------------------------------------------------
		/// Get Flipped Horizontal
		///
		/// @param Whether sprite is flipped about it's local x-axis
		//-----------------------------------------------------------
		bool ImageView::IsHorizontalFlipEnabled() const
		{
			return FlipHorizontal;
		}
		//-----------------------------------------------------------
		/// Set Flipped Vertical
		///
		/// Flip the sprite about it's local y-axis
		///
		/// @param Whether to flip or not
		//-----------------------------------------------------------
		void ImageView::EnableVerticalFlip(bool inbValue)
		{
			FlipVertical = inbValue;
		}
		//-----------------------------------------------------------
		/// Get Flipped Vertical
		///
		/// @param Whether sprite is flipped about it's local y-axis
		//-----------------------------------------------------------
		bool ImageView::IsVerticalFlipEnabled() const
		{
			return FlipVertical;
		}
        //-----------------------------------------------------------
        /// Enable Act As Spacer
        ///
        /// The image will not draw but simply act as a container
        ///
        /// @param Enable/disable
        //-----------------------------------------------------------
        void ImageView::EnableActAsSpacer(bool inbValue)
        {
            ActAsSpacer = inbValue;
        }
        //-----------------------------------------------------------
        /// Is Act As Spacer Enabled
        ///
        /// @param Whether the image view is acting as a container
        //-----------------------------------------------------------
        bool ImageView::IsActAsSpacerEnabled() const
        {
            return ActAsSpacer;
        }
    }
}