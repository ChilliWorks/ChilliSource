//
//  ImageView.cpp
//  moFlo
//
//  Created by Scott Downie on 22/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/Image/ImageView.h>

#include <ChilliSource/Rendering/Sprite/SpriteSheet.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheetManager.h>
#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>

#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/String/StringParser.h>

namespace ChilliSource
{
    namespace GUI
    {
        DEFINE_META_CLASS(ImageView)
		
        //---Properties
        DEFINE_PROPERTY(Texture);
        DEFINE_PROPERTY(SpriteSheet);
        DEFINE_PROPERTY(SpriteSheetIndex);
        DEFINE_PROPERTY(SpriteSheetIndexID);
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
        ImageView::ImageView() : UVs(Core::CVector2(0, 0), Core::CVector2(1, 1)), UVOffsets(Core::CVector2(0, 0), Core::CVector2(1, 1)), SpriteSheetIndex(0), SizeFromImage(false),
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
        : GUIView(insParams), UVs(Core::CVector2(0, 0), Core::CVector2(1, 1)), UVOffsets(Core::CVector2(0, 0), Core::CVector2(1, 1)), SpriteSheetIndex(0),
        SizeFromImage(false), HeightMaintain(false), WidthMaintain(false), WidthFromImage(false), HeightFromImage(false), ActAsSpacer(false), FlipHorizontal(false), FlipVertical(false), mbFillMaintain(false), mbFitMaintain(false)
        {
            std::string strValue;
            
            //---Texture
            Core::StorageLocation eTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("TextureLocation", strValue))
            {
                eTextureLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("Texture", strValue))
            {
                Texture = LOAD_RESOURCE(Rendering::Texture, eTextureLocation, strValue);
            }
            //---Sprite sheet
            Core::StorageLocation eSpriteSheetLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("SpriteSheetLocation", strValue))
            {
                eSpriteSheetLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("SpriteSheet", strValue))
            {
                SetSpriteSheet(strValue, eSpriteSheetLocation);
            }
            //---Sprite sheet index
            if(insParams.TryGetValue("SpriteSheetIndex", strValue))
            {
                CS_ASSERT(SpriteSheet, "Cannot set sprite sheet index without setting sprite sheet");
                SpriteSheetIndex = Core::ParseU32(strValue);
            }
            //---Sprite Sheet Index ID
            if(insParams.TryGetValue("SpriteSheetIndexID", strValue))
            {
                SetSpriteSheetIndexID(strValue);
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
                Core::CVector2 vSize = Core::ParseVector2(strValue);
                SetHeightMaintainingAspect(vSize.x, vSize.y);
            }
            //---Set Maintain Height
            if(insParams.TryGetValue("SetWidthMaintain", strValue))
            {
                Core::CVector2 vSize = Core::ParseVector2(strValue);
                SetWidthMaintainingAspect(vSize.x, vSize.y);
            }
            //---Set Fill Maintain
            if(insParams.TryGetValue("SetFillMaintain", strValue))
            {
                Core::CVector4 vSize = Core::ParseVector4(strValue);
                SetFillMaintainingAspect(vSize.x, vSize.y, vSize.z, vSize.w);
            }
            //---Set Fit Maintain
            if(insParams.TryGetValue("SetFitMaintain", strValue))
            {
                Core::CVector4 vSize = Core::ParseVector4(strValue);
                SetFitMaintainingAspect(vSize.x, vSize.y, vSize.z, vSize.w);
            }
            //---UV
            if(insParams.TryGetValue("UVs", strValue))
            {
                Core::CVector4 vRaw = Core::ParseVector4(strValue);
                UVs.vOrigin.x = vRaw.x;
                UVs.vOrigin.y = vRaw.y;
                UVs.vSize.x = vRaw.z;
                UVs.vSize.y = vRaw.w;
            }
            //---UV Offsets
            if(insParams.TryGetValue("UVOffsets", strValue))
            {
                Core::CVector4 vRaw = Core::ParseVector4(strValue);
                UVOffsets.vOrigin.x = vRaw.x;
                UVOffsets.vOrigin.y = vRaw.y;
                UVOffsets.vSize.x = vRaw.z;
                UVOffsets.vSize.y = vRaw.w;
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
        /// Calculate UVs
        ///
        /// @param UVs
        /// @param UV offsets
        /// @param Out: UVs
		/// @param FlippedX
		/// @param FlippedY
        //--------------------------------------------------------
        void CalculateUVs(const Core::Rectangle& insUVs, const Core::Rectangle& insOffsets, Core::Rectangle& outsUVs)
        {
            f32 fWidthOfArea = outsUVs.Right() - outsUVs.Left();
            f32 fHeightOfArea = outsUVs.Bottom() - outsUVs.Top();
            Core::CVector2 vTopLeft = Core::CVector2((fWidthOfArea*insOffsets.Left())+outsUVs.Left(), (fHeightOfArea*insOffsets.Top())+outsUVs.Top());
            Core::CVector2 vBottomRight = Core::CVector2((fWidthOfArea*insOffsets.Right()), (fHeightOfArea*insOffsets.Bottom()));
            outsUVs = Core::Rectangle(vTopLeft, vBottomRight);
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
                Rendering::TextureSPtr pTexture;
                Core::Rectangle sNewUVs;
                
                if(Texture)
                {
                    pTexture = Texture;
                    sNewUVs = UVs;
                }
                else if(SpriteSheet)
                {
                    pTexture = SpriteSheet->GetTexture();
                    sNewUVs = SpriteSheet->GetUVsForFrame(SpriteSheetIndex);
                }
                
                if(UVOffsets.TopLeft() != Core::CVector2::ZERO || UVOffsets.BottomRight() != Core::CVector2::ONE)
                {
                    CalculateUVs(UVs, UVOffsets, sNewUVs);
                }
				
				if(IsHorizontalFlipEnabled())
				{
					sNewUVs.vOrigin.x += sNewUVs.vSize.x;
					sNewUVs.vSize.x *= -1;
				}
				if(IsVerticalFlipEnabled())
				{
					sNewUVs.vOrigin.y += sNewUVs.vSize.y;
					sNewUVs.vSize.y *= -1;
				}
                
                inpCanvas->DrawBox(GetTransform(), GetAbsoluteSize(), pTexture, sNewUVs, GetAbsoluteColour());
            }
            
            GUIView::Draw(inpCanvas);
        }
        //--------------------------------------------------------
        /// Set Sprite Sheet
        ///
        /// @param Sprite sheet name
        //--------------------------------------------------------
        void ImageView::SetSpriteSheet(const std::string& instrSpriteSheet, Core::StorageLocation ineLocation)
        {
            SpriteSheet = LOAD_RESOURCE(Rendering::SpriteSheet, ineLocation, instrSpriteSheet);
        }
        //--------------------------------------------------------
        /// Set Sprite Sheet
        ///
        /// @param Sprite sheet
        //--------------------------------------------------------
        void ImageView::SetSpriteSheet(const Rendering::SpriteSheetSPtr& inpSpriteSheet)
        {
            SpriteSheet = inpSpriteSheet;
        }
        //--------------------------------------------------------
        /// Get Sprite Sheet
        ///
        /// @param Sprite sheet
        //--------------------------------------------------------
        const Rendering::SpriteSheetSPtr& ImageView::GetSpriteSheet() const
        {
            return SpriteSheet;
        }
        //--------------------------------------------------------
        /// Set Sprite Sheet Index
        ///
        /// @param The index of the image within the sprite sheet
        //--------------------------------------------------------
        void ImageView::SetSpriteSheetIndex(u32 inudwIndex)
        {
            SpriteSheetIndex = inudwIndex;
        }
        //--------------------------------------------------------
        /// Get Sprite Sheet Index
        ///
        /// @return The index of the image within the sprite sheet
        //--------------------------------------------------------
        u32 ImageView::GetSpriteSheetIndex() const
        {
            return SpriteSheetIndex;
        }
        //--------------------------------------------------------
        /// Set Sprite Sheet Index ID
        ///
        /// @param The ID of the index of the image within the sprite sheet
        //--------------------------------------------------------
        void ImageView::SetSpriteSheetIndexID(const std::string& instrID)
        {
            CS_ASSERT(SpriteSheet, "Cannot set sprite sheet index without setting sprite sheet");
            SpriteSheetIndexID = instrID;
            SetSpriteSheetIndex(SpriteSheet->GetFrameIndexByID(instrID));
            
            LayoutContent();
            LayoutChildrensContent();
        }
        //--------------------------------------------------------
        /// Get Sprite Sheet Index ID
        ///
        /// @return The ID of the index of the image within the sprite sheet
        //--------------------------------------------------------
        const std::string& ImageView::GetSpriteSheetIndexID() const
        {
            return SpriteSheetIndexID;
        }
        //--------------------------------------------------------
        /// Set Texture
        ///
        /// Set the texture that defines the image in the view
        ///
        /// @param Texture shared pointer
        //--------------------------------------------------------
        void ImageView::SetTexture(const Rendering::TextureSPtr& inpTexture)
        {
            Texture = inpTexture;
            
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
        const Rendering::TextureSPtr& ImageView::GetTexture() const
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
        void ImageView::SetUVs(const Core::Rectangle& insUVs)
        {
            UVs = insUVs;
        }
        //--------------------------------------------------------
        /// Get UVs
        ///
        /// @return Rect containing u, v, s, & t
        //--------------------------------------------------------
        const Core::Rectangle& ImageView::GetUVs() const
        {
            return UVs;
        }
        //--------------------------------------------------------
        /// Set UV Offsets
        ///
        /// Set the offsets to the UV so that on a sprite sheet the image
        /// can be treated as if it were a single texture
        ///
        /// @param Rect containing u, v, s, & t
        //--------------------------------------------------------
        void ImageView::SetUVOffsets(const Core::Rectangle& insUVOffsets)
        {
            UVOffsets = insUVOffsets;
        }
        //--------------------------------------------------------
        /// Get UV Offsets
        ///
        /// @return Rect containing u, v, s, & t offsets
        //--------------------------------------------------------
        const Core::Rectangle& ImageView::GetUVOffsets() const
        {
            return UVOffsets;
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
            SetSize(Core::UnifiedVector2(Core::CVector2::ZERO, GetSizeFromImage()));
        }
		
        //--------------------------------------------------------
        /// Get Size From Image
        ///
        /// @return Absolute size of the image
        //--------------------------------------------------------
        Core::CVector2 ImageView::GetSizeFromImage() const
        {
            CS_ASSERT((Texture || SpriteSheet), "Must have a sprite sheet or texture");
            
            if(Texture)
            {
                return Core::CVector2((f32)Texture->GetWidth(), (f32)Texture->GetHeight());
            }
            else if(SpriteSheet)
            {
                return SpriteSheet->GetSizeForFrame(SpriteSheetIndex);
            }
            
            return Core::CVector2::ZERO;
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
            
            Core::CVector2 vCurrentSize = GetSizeFromImage();
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
            
            Core::CVector2 vCurrentSize = GetSizeFromImage();
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
            
            Core::CVector2 vParentSize(GetParentViewPtr()->GetAbsoluteSize());
            Core::CVector2 vImageSize(GetSizeFromImage());
            
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
            
            Core::CVector2 vParentSize(GetParentViewPtr()->GetAbsoluteSize());
            Core::CVector2 vImageSize(GetSizeFromImage());
            
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