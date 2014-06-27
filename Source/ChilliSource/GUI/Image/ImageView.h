//
//  ImageView.h
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

#ifndef _CHILLISOURCE_GUI_IMAGEVIEW_H_
#define _CHILLISOURCE_GUI_IMAGEVIEW_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Base/GUIView.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Rendering/Texture/UVs.h>

namespace ChilliSource
{
    namespace GUI
    {
        class ImageView : public GUIView
        {
        public:

			DECLARE_META_CLASS(ImageView)

            ImageView();
            ImageView(const Core::ParamDictionary& insParams);
            virtual ~ImageView(){}
            //--------------------------------------------------------
            /// Draw
            ///
            /// Override the base draw method to render ourselves
            ///
            /// @param Canvas renderer pointer
            //--------------------------------------------------------
            virtual void Draw(Rendering::CanvasRenderer* inpCanvas);
            //--------------------------------------------------------
            /// Set Texture
            ///
            /// Set the texture that defines the image in the view
            ///
            /// @param Texture shared pointer
            //--------------------------------------------------------
            void SetTexture(const Rendering::TextureCSPtr& inpTexture);
            //--------------------------------------------------------
            /// Get Texture
            ///
            /// Get the texture that defines the image in the view
            ///
            /// @param Texture shared pointer
            //--------------------------------------------------------
            const Rendering::TextureCSPtr& GetTexture() const;
            //--------------------------------------------------------
            /// Set Sprite Sheet
            ///
            /// @param Location
            /// @param Sprite sheet name
            //--------------------------------------------------------
            void SetTextureAtlas(Core::StorageLocation ineLocation, const std::string& instrTextureAtlas);
            //--------------------------------------------------------
            /// Set Sprite Sheet
            ///
            /// @param Sprite sheet
            //--------------------------------------------------------
            void SetTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas);
            //--------------------------------------------------------
            /// Get Sprite Sheet
            ///
            /// @param Sprite sheet
            //--------------------------------------------------------
            const Rendering::TextureAtlasCSPtr& GetTextureAtlas() const;
			//--------------------------------------------------------
			/// Set Sprite Sheet Index ID
			///
			/// @param The ID of the index of the image within the sprite sheet
			//--------------------------------------------------------
			void SetTextureAtlasID(const std::string& instrID);
			//--------------------------------------------------------
			/// Get Sprite Sheet Index ID
			///
			/// @return The ID of the index of the image within the sprite sheet
			//--------------------------------------------------------
			const std::string& GetTextureAtlasID() const;
            //--------------------------------------------------------
            /// Set UVs
            ///
            /// Set the texture co-ordinates of the image. By default
            /// they will be mapped from 0 to 1
            ///
            /// @param Rect containing u, v, s, & t
            //--------------------------------------------------------
            void SetUVs(const Rendering::UVs& insUVs);
			//--------------------------------------------------------
			/// Get UVs
			///
			/// @return Rect containing u, v, s, & t
			//--------------------------------------------------------
			const Rendering::UVs& GetUVs() const;
            //--------------------------------------------------------
            /// Enable Size From Image
            ///
            /// When this is enabled the image view's size will be 
            /// based on the size of the image
            ///
            /// @param Enable/disable
            //--------------------------------------------------------
            void EnableSizeFromImage(bool inbEnable);
			//--------------------------------------------------------
			/// Is Size From Image Enabled
			///
			/// When this is enabled the image view's size will be 
			/// based on the size of the image
			///
			/// @return Whether the image view's size will be 
			/// based on the size of the image
			//--------------------------------------------------------
			bool IsSizeFromImageEnabled() const;
            //--------------------------------------------------------
            /// Enable Width From Image
            ///
            /// When this is enabled the image view's width will be 
            /// based on the width of the image
            ///
            /// @param Enable/disable
            //--------------------------------------------------------
            void EnableWidthFromImage(bool inbEnable);
			//--------------------------------------------------------
			/// Is Width From Image Enabled
			///
			/// When this is enabled the image view's width will be 
			/// based on the width of the image
			///
			/// @return Whether the image view's width will be 
			/// based on the width of the image
			//--------------------------------------------------------
			bool IsWidthFromImageEnabled() const;
            //--------------------------------------------------------
            /// Set Width From Image
            ///
            /// Sets the of the image view to the width of the image
            /// without requiring a draw to call. Useful for working
            /// with relative sizes and aspect ratios.
            //--------------------------------------------------------
            void SetWidthFromImage();
            //--------------------------------------------------------
            /// Enable Height From Image
            ///
            /// When this is enabled the image view's height will be 
            /// based on the height of the image
            ///
            /// @param Enable/disable
            //--------------------------------------------------------
            void EnableHeightFromImage(bool inbEnable);
			//--------------------------------------------------------
			/// Is Height From Image Enabled
			///
			/// When this is enabled the image view's height will be 
			/// based on the height of the image
			///
			/// @return Whether the image view's height will be 
			/// based on the height of the image
			//--------------------------------------------------------
			bool IsHeightFromImageEnabled() const;
            //--------------------------------------------------------
            /// Set Height From Image
            ///
            /// Sets the of the image view to the height of the image
            /// without requiring a draw to call. Useful for working
            /// with relative sizes and aspect ratios.
            //--------------------------------------------------------
            void SetHeightFromImage();
            //--------------------------------------------------------
            /// Set Width Maintaining Aspect
            ///
            /// Change the current width of the image view and resize the height
            /// to maintain the aspect ratio of the image
            ///
            /// @param Unified width
            //--------------------------------------------------------
            void SetWidthMaintainingAspect(f32 infRelWidth, f32 infAbsWidth);
            //--------------------------------------------------------
            /// Set Height Maintaining Aspect
            ///
            /// Change the current height of the image view and resize the width
            /// to maintain the aspect ratio of the image
            ///
            /// @param Unified height
            //--------------------------------------------------------
            void SetHeightMaintainingAspect(f32 infRelHeight, f32 infAbsHeight);
            //--------------------------------------------------------
            /// Set Fill Maintaining Aspect
            ///
            /// Based on the size of the image and the target size
            /// resize the image view to be the minimum of the target
            /// size while maintaining the aspect ratio
            ///
            /// @param Unified size
            //--------------------------------------------------------
            void SetFillMaintainingAspect(f32 infRelWidth, f32 infRelHeight, f32 infAbsWidth, f32 infAbsHeight);
            //--------------------------------------------------------
            /// Set Fit Maintaining Aspect
            ///
            /// Based on the size of the image and the target size
            /// resize the image view to be the maximum of the target
            /// size while maintaining the aspect ratio
            ///
            /// @param Unified size
            //--------------------------------------------------------
            void SetFitMaintainingAspect(f32 infRelWidth, f32 infRelHeight, f32 infAbsWidth, f32 infAbsHeight);
            //--------------------------------------------------------
            /// Enable Width Maintaining Aspect
            ///
            /// Enables auto scaling of the height to maintain the aspect ratio
            ///
            /// @param boolean to disable or enable
            //--------------------------------------------------------
            void EnableWidthMaintainingAspect(bool inbEnabled);
			//--------------------------------------------------------
			/// Is Width Maintaining Aspect Enabled
			///
			/// @return Whether auto scaling of the width to maintain the aspect ratio
			//--------------------------------------------------------
			bool IsWidthMaintainingAspectEnabled() const;
            //--------------------------------------------------------
            /// Enable Height Maintaining Aspect
            ///
            /// Enables auto scaling of the height to maintain the aspect ratio
            ///
            /// @param boolean to disable or enable
            //--------------------------------------------------------
            void EnableHeightMaintainingAspect(bool inbEnabled);
			//--------------------------------------------------------
			/// Is Height Maintaining Aspect Enabled
			///
			/// @return Whether auto scaling of the height to maintain the aspect ratio
			//--------------------------------------------------------
			bool IsHeightMaintainingAspectEnabled() const;
            //-----------------------------------------------------------
			/// Set Flipped Horizontal
			///
			/// Flip the sprite about it's local x-axis
			///
			/// @param Whether to flip or not
			//-----------------------------------------------------------
			void EnableHorizontalFlip(bool inbValue);
			//-----------------------------------------------------------
			/// Get Flipped Horizontal
			///
			/// @param Whether sprite is flipped about it's local x-axis
			//-----------------------------------------------------------
			bool IsHorizontalFlipEnabled() const;
			//-----------------------------------------------------------
			/// Set Flipped Vertical
			///
			/// Flip the sprite about it's local y-axis
			///
			/// @param Whether to flip or not
			//-----------------------------------------------------------
			void EnableVerticalFlip(bool inbValue);
			//-----------------------------------------------------------
			/// Get Flipped Vertical
			///
			/// @param Whether sprite is flipped about it's local y-axis
			//-----------------------------------------------------------
			bool IsVerticalFlipEnabled() const;
            //-----------------------------------------------------------
			/// Enable Act As Spacer
			///
			/// The image will not draw but simply act as a container
			///
			/// @param Enable/disable
			//-----------------------------------------------------------
			void EnableActAsSpacer(bool inbValue);
			//-----------------------------------------------------------
			/// Is Act As Spacer Enabled
			///
			/// @param Whether the image view is acting as a container
			//-----------------------------------------------------------
			bool IsActAsSpacerEnabled() const;
			//--------------------------------------------------------
			/// Get Size From Image
			///
			/// @return Absolute size of the image
			//--------------------------------------------------------
			Core::Vector2 GetSizeFromImage() const;
			//--------------------------------------------------------
			/// Layout Content
			///
			/// Called when the view is able to retrieve an absolute
			/// value.
			//--------------------------------------------------------
			void LayoutContent();
            
        private:
            //--------------------------------------------------------
            /// Set Size From Image
            ///
            /// Sets the of the image view to the size of the image
            /// without requiring a draw to call. Useful for working
            /// with relative sizes and aspect ratios.
            //--------------------------------------------------------
            void SetSizeFromImage();
			//--------------------------------------------------------
			/// Update Size From Image
			///
			/// Image view can be sized from assets etc. This function
			/// calculates the size based on these factors
			//--------------------------------------------------------
			void UpdateSizeFromImage();
			//--------------------------------------------------------
			/// Update Size Maintaining Aspect
			///
			/// Image view can be sized from assets etc. This function
			/// calculates the size based on these factors
			//--------------------------------------------------------
			void UpdateSizeMaintainingAspect(); 

        private:
            
            Core::UnifiedVector2 mvFillMaintainTarget;
            Core::UnifiedVector2 mvFitMaintainTarget;
            
            bool mbFillMaintain;
            bool mbFitMaintain;
            
            Core::Vector2 m_imageSize;
            
			//---Properties
            DECLARE_PROPERTY_A(Rendering::TextureCSPtr, Texture, SetTexture, GetTexture);
            DECLARE_PROPERTY_A(Rendering::TextureAtlasCSPtr, TextureAtlas, SetTextureAtlas, GetTextureAtlas);

            DECLARE_PROPERTY_A(Rendering::UVs, UVs, SetUVs, GetUVs);
            
			DECLARE_PROPERTY_A(std::string, TextureAtlasID, SetTextureAtlasID, GetTextureAtlasID);
            u32 m_hashedTextureAtlasId = 0;
            
            DECLARE_PROPERTY_A(bool, ActAsSpacer, EnableActAsSpacer, IsActAsSpacerEnabled);
            DECLARE_PROPERTY_A(bool, SizeFromImage, EnableSizeFromImage, IsSizeFromImageEnabled);
            DECLARE_PROPERTY_A(bool, WidthFromImage, EnableSizeFromImage, IsSizeFromImageEnabled);
            DECLARE_PROPERTY_A(bool, HeightFromImage, EnableSizeFromImage, IsSizeFromImageEnabled);
            
            DECLARE_PROPERTY_A(bool, HeightMaintain, EnableHeightMaintainingAspect, IsHeightMaintainingAspectEnabled);
            DECLARE_PROPERTY_A(bool, WidthMaintain, EnableWidthMaintainingAspect, IsWidthMaintainingAspectEnabled);
            
            DECLARE_PROPERTY_A(bool, FlipVertical, EnableVerticalFlip, IsVerticalFlipEnabled);
            DECLARE_PROPERTY_A(bool, FlipHorizontal, EnableHorizontalFlip, IsHorizontalFlipEnabled);
        };
    }
}

#endif